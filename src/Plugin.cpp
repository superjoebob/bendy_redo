#include "Plugin.h"
#include "PluginGUI.h"
#include "State.h"
#include "StreamWrapper.h"
extern "C" TFruityPlug * _stdcall CreatePlugInstance(TFruityPlugHost * Host, int Tag)
{
	FruityPlugInfo info;
	info.Host = Host;
	info.Tag = Tag;
	return (TFruityPlug*)new Plugin(&info);
};

TFruityPlugInfo PlugInfo =
{
	CurrentSDKVersion,
	"BENDY",
	"BENDY",
	FPF_Type_FullGen // the amount of parameters
};

void* hInstance; // used by VSTGUI
extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
	hInstance = hInst;
	return TRUE;
}

Plugin::Plugin(void* data)
	: TCPPFruityPlug(static_cast<FruityPlugInfo*>(data)->Tag, static_cast<FruityPlugInfo*>(data)->Host, 0)
{
	FruityPlugInfo* info = static_cast<FruityPlugInfo*>(data);
	PlugInfo.NumParams = 1;
	Info = &PlugInfo;
	HostTag = info->Tag;
	EditorHandle = 0;

	_gui = new PluginGUI(this);
	_state = new State();

}

Plugin::~Plugin()
{
	if (_gui != nullptr)
		delete _gui;
}

void _stdcall Plugin::SaveRestoreState(IStream* Stream, BOOL Save)
{
	unsigned long magicNumber = 13118359537711872022L;
	unsigned long legacyMagicNumber = 14928659832781827043L;
	StreamWrapper* s = new StreamWrapper(Stream);
	if (Save)
	{
		unsigned long written = 0;
		Stream->Write(&magicNumber, sizeof(unsigned long), &written);
		if(_state != nullptr)
			_state->serialize(s);
	}
	else
	{
		unsigned long written = 0;
		unsigned long magicNumberRead = 0;
		Stream->Read(&magicNumberRead, sizeof(unsigned long), &written);
		if (magicNumberRead == legacyMagicNumber)
		{
			//deserialize using old synthman logic
			int streamSize = 0;
			Stream->Read(&streamSize, sizeof(int), &written);
			if (_state != nullptr)
				delete _state;

			_state = new State();
			_state->legacy_deserialize(s);
		}
		else if (magicNumberRead == magicNumber)
		{
			if (_state != nullptr)
				delete _state;

			_state = new State();
			_state->deserialize(s);
		}

		ProcessAllParams();
	}
}

intptr_t _stdcall Plugin::Dispatcher(intptr_t ID, intptr_t Index, intptr_t Value)
{
	intptr_t r = TCPPFruityPlug::Dispatcher(ID, Index, Value);
	if (r != 0)
		return r;

	if (ID == FPD_ShowEditor)
	{
		if (_gui != nullptr)
		{
			if (Value == 0)
			{
				// close editor
				_gui->close();
				EditorHandle = 0;
			}
			else if (EditorHandle == 0)
			{
				// open editor
				_gui->open(reinterpret_cast<HWND>(Value));
				_gui->getFrame()->takeFocus();

				//_gui->getFrame()->getSystemWindow

				EditorHandle = static_cast<HWND>(_gui->getFrame()->getSystemWindow());
			}
			else
			{
				// change parent window ?
				::SetParent(EditorHandle, reinterpret_cast<HWND>(Value));
			}
		}
	}

	return 0;
}

enum class AutomationTypeFlags
{
	UpdateValue = 1,		// update the value
	GetValue = 2,			// retrieves the value
	ShowHint = 4,			// updates the hint (if any)
	UpdateControl = 16,		// updates the wheel/knob
	FromMIDI = 32,			// value from 0 to 65536 has to be translated (& always returned, even if REC_GetValue isn't set)
	NoLink = 1024,			// don't check if wheels are linked (internal to plugins, useful for linked controls)
	InternalCtrl = 2048,	// sent by an internal controller - internal controllers should pay attention to those, to avoid nasty feedbacks
	PlugReserved = 4096,    // free to use by plugins
	UpdateUIThreaded = 8192,
	GetValueUI = 524288,
};

int _stdcall Plugin::ProcessParam(int index, int value, int RECFlags)
{
	AutomationTypeFlags type = (AutomationTypeFlags)RECFlags;
	bool fromMidi = ((int)type & (int)AutomationTypeFlags::FromMIDI);
	bool updateValue = (int)type & (int)AutomationTypeFlags::UpdateValue;

	bool needSetValue = updateValue || fromMidi;
	bool getValue = (int)type & (int)AutomationTypeFlags::GetValue;
	bool updateUI = (int)type & (int)AutomationTypeFlags::UpdateControl;
	bool getValueUI = ((int)type & (int)AutomationTypeFlags::GetValueUI);
	//if (!(needSetValue || updateValue || getValue || updateUI) /* || (parameterHash > kExtParamsEnd)*/)
	//	return -1;

	if ((int)type & (int)AutomationTypeFlags::UpdateValue)
	{
		PlugParameter* param = getParameter(index);
		int max = param->getMax();
		if (param != nullptr)
			param->setFloat(value);
	}
	
	return 0;
}

void _stdcall Plugin::Gen_Render(PWAV32FS DestBuffer, int& Length)
{
	//if (VoiceList.GetCount() == 0)
	//	Length = 0;       // nothing to render, so tell it
	//else
	//{
	//	for (int n = VoiceList.GetCount() - 1; n >= 0; n--)
	//	{
	//		PWaveT temp = (PWaveT)malloc(Length << 2);
	//		memset(temp, 0, Length << 2);

	//		POSITION pos = VoiceList.FindIndex(n);
	//		PPlugVoice voice = (PPlugVoice)(VoiceList.GetAt(pos));
	//		if (voice->State == -2)
	//			// let the host kill the voice
	//			PlugHost->Voice_Kill(voice->HostTag, true);
	//		else
	//		{
	//			// render it
	//			Voice_Render_Internal((int)voice, temp, Length);
	//			ApplyEnvelope(temp, Length, voice->State);
	//			CopyMonoToStereo(temp, DestBuffer, Length, voice->Params->FinalLevels.Pan);
	//		}

	//		free(temp);
	//	}
	//}
}


TVoiceHandle _stdcall Plugin::TriggerVoice(PVoiceParams VoiceParams, int SetTag)
{
	PPlugVoice Voice;

	// create & init
	Voice = new TPlugVoice;
	Voice->HostTag = SetTag;

	for (int n = 0; n < 2; n++)
		Voice->Pos[n] = 0;
	Voice->Params = VoiceParams;
	Voice->State = 1;   // start the attack envelope


	// add to the list
	//VoiceList.AddTail(Voice);
	return (TVoiceHandle)Voice;
}


void _stdcall Plugin::Voice_Release(TVoiceHandle Handle)
{
	//((PlugVoice)Handle)->State = -1;  // releasing
}


// free a voice
void _stdcall Plugin::Voice_Kill(TVoiceHandle Handle)
{
	//POSITION pos = VoiceList.Find((void*)Handle);
	//VoiceList.RemoveAt(pos);
	//delete (PPlugVoice)Handle;
}

Serializable* Plugin::getSerializable(std::wstring control)
{
	auto it = _state->namemap.find(control);
	if (it == _state->namemap.end())
		return nullptr;

	return (*it).second;
}

PlugParameter* Plugin::getParameter(std::wstring control)
{
	auto it = _state->namemap.find(control);
	if (it == _state->namemap.end())
		return nullptr;

	return dynamic_cast<PlugParameter*>((*it).second);
}

PlugParameter* Plugin::getParameter(int hash)
{
	auto it = _state->hashmap.find(hash);
	if (it == _state->hashmap.end())
		return nullptr;

	return dynamic_cast<PlugParameter*>((*it).second);
}

int Plugin::getCID(std::wstring control)
{
	auto it = _state->namemap.find(control);
	if (it == _state->namemap.end())
		return -1;

	return (*it).second->hash;
}

std::wstring Plugin::toString(std::wstring control)
{
	auto it = _state->namemap.find(control);
	if (it == _state->namemap.end())
		return L"";

	return ((PlugParameter*)(*it).second)->toString();
}