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
	unsigned long magicNumber = 14928659832781827043L;
	StreamWrapper* s = new StreamWrapper(Stream);
	if (Save)
	{
		//Stream->Write(&ParamValue, NumParamsConst * 4, 0);
	}
	else
	{
		unsigned long written = 0;
		unsigned long magicNumberRead = 0;
		Stream->Read(&magicNumberRead, sizeof(unsigned long), &written);
		int streamSize = 0;
		Stream->Read(&streamSize, sizeof(int), &written);
		//char* data = new char[streamSize];
		//Stream->Read(data, streamSize, &written);

		if (_state != nullptr)
			delete _state;

		_state = new State();
		_state->deserialize(s);


		//unsigned int childID = s->readUInt();
		//while (childID != 0)
		//{
		//	unsigned int paramSize = s->readUInt();
		//	s->advance(paramSize); //ignore data for now, should figure out how to read it

		//	childID = s->readUInt();
		//}


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

int Plugin::getCID(std::wstring control)
{
	auto it = _state->namemap.find(control);
	if (it == _state->namemap.end())
		return -1;

	return (*it).second->hash;
}
float Plugin::getValue(std::wstring control)
{
	auto it = _state->namemap.find(control);
	if (it == _state->namemap.end())
		return -1;

	return (*it).second->getFloat();
}