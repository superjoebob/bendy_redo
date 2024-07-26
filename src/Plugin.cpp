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
	_state = new State();

	FruityPlugInfo* info = static_cast<FruityPlugInfo*>(data);
	PlugInfo.NumParams = _state->all.size();
	Info = &PlugInfo;
	HostTag = info->Tag;
	EditorHandle = 0;

	_pulseIndex = 0;

	for (int i = 0; i < 5; i++)
		letters[i] = 0;


	_scales =
	{
		Scale(L"Major", {2, 2, 1, 2, 2, 2}),
		Scale(L"Minor", {2, 1, 2, 2, 1, 2}),
		Scale(L"Harmonic Minor", { 2, 1, 2, 2, 1, 3}),
		Scale(L"Melodic Minor", {2, 1, 2, 2, 2, 2, 1})
	};

	Preset* def = new Preset();
	def->presetName = L"default";
	def->asdrCCName.value = L"ATK REL";
	def->asdrCC[0].value = 73;
	def->asdrCCValue[0].value = 64;
	def->asdrCC[3].value = 72;
	def->asdrCCValue[3].value = 64;

	def->midiKnobCC[0].value = 74;
	def->midiKnobName[0].value = L"Cutoff";
	def->midiKnobCC[1].value = 71;
	def->midiKnobName[1].value = L"Reso";

	def->midiKnobCC[6].value = 91;
	def->midiKnobName[6].value = L"Reverb";
	def->midiKnobCC[7].value = 93;
	def->midiKnobName[7].value = L"Chorus";

	_presets.push_back(def);

	Preset* nexus = new Preset();
	nexus->presetName = L"Nexus Controller";
	nexus->asdrCCName.value = L"Filter";
	nexus->asdrCC[0].value = 28;
	nexus->asdrCCValue[0].value = 64;
	nexus->asdrCC[1].value = 30;
	nexus->asdrCCValue[1].value = 64;
	nexus->asdrCC[2].value = 29;
	nexus->asdrCCValue[2].value = 64;
	nexus->asdrCC[3].value = 31;
	nexus->asdrCCValue[3].value = 64;

	nexus->midiKnobCC[0].value = 44;
	nexus->midiKnobName[0].value = L"FLT Env";
	nexus->midiKnobCC[1].value = 45;
	nexus->midiKnobName[1].value = L"FLT Cut";
	nexus->midiKnobCC[2].value = 46;
	nexus->midiKnobName[2].value = L"FLT Reso";
	nexus->midiKnobCC[3].value = 73;
	nexus->midiKnobName[3].value = L"AMP Atk";

	nexus->midiKnobCC[4].value = 74;
	nexus->midiKnobName[4].value = L"Cutoff";
	nexus->midiKnobCC[5].value = 71;
	nexus->midiKnobName[5].value = L"Reso";
	nexus->midiKnobCC[6].value = 91;
	nexus->midiKnobName[6].value = L"Reverb";
	nexus->midiKnobCC[7].value = 94;
	nexus->midiKnobName[7].value = L"Delay";
	_presets.push_back(nexus);

	Preset* sylenth = new Preset();
	sylenth->presetName = L"Sylenth Controller";
	sylenth->asdrCCName.value = L"AMP ENV A";
	sylenth->asdrCC[0].value = 76;
	sylenth->asdrCCValue[0].value = 64;
	sylenth->asdrCC[1].value = 78;
	sylenth->asdrCCValue[1].value = 64;
	sylenth->asdrCC[2].value = 77;
	sylenth->asdrCCValue[2].value = 64;
	sylenth->asdrCC[3].value = 79;
	sylenth->asdrCCValue[3].value = 64;

	sylenth->midiKnobCC[0].value = 47;
	sylenth->midiKnobName[0].value = L"Cutoff";
	sylenth->midiKnobCC[1].value = 72;
	sylenth->midiKnobName[1].value = L"A Cut";
	sylenth->midiKnobCC[2].value = 67;
	sylenth->midiKnobName[2].value = L"A Drive";
	sylenth->midiKnobCC[3].value = 69;
	sylenth->midiKnobName[3].value = L"A Reso";
	sylenth->midiKnobCC[4].value = 71;
	sylenth->midiKnobName[4].value = L"Reso";
	sylenth->midiKnobCC[5].value = 73;
	sylenth->midiKnobName[5].value = L"B Cut";
	sylenth->midiKnobCC[6].value = 68;
	sylenth->midiKnobName[6].value = L"B Drive";
	sylenth->midiKnobCC[7].value = 70;
	sylenth->midiKnobName[7].value = L"B Reso";
	_presets.push_back(sylenth);

	Preset* shreddage2 = new Preset();
	shreddage2->presetName = L"Shreddage 2 Controller";
	shreddage2->noteNames[30] = L"Powerchord Toggle";
	shreddage2->noteNames[29] = L"Full Choke";
	shreddage2->noteNames[26] = L"Fret Noise";
	shreddage2->noteNames[19] = L"Normal Sustain";
	shreddage2->noteNames[18] = L"Harmonic";
	shreddage2->noteNames[17] = L"Special FX";
	shreddage2->noteNames[16] = L"Tremolo";
	//shreddage2.noteNames[15] = "Tremolo";
	shreddage2->noteNames[15] = L"Powerchord Staccato";
	shreddage2->noteNames[14] = L"Single Staccato";
	shreddage2->noteNames[13] = L"Powerchord Mute";
	shreddage2->noteNames[12] = L"Powerchord Sustain";
	shreddage2->noteNames[7] = L"String Pref Reset";
	shreddage2->noteNames[6] = L"String Pref 7";
	shreddage2->noteNames[5] = L"String Pref 6";
	shreddage2->noteNames[4] = L"String Pref 5";
	shreddage2->noteNames[3] = L"String Pref 4";
	shreddage2->noteNames[2] = L"String Pref 3";
	shreddage2->noteNames[1] = L"String Pref 2";
	shreddage2->noteNames[0] = L"String Pref 1";
	_presets.push_back(shreddage2);

	Preset* sc = new Preset();
	sc->presetName = L"Sound Canvas Controller";
	sc->asdrCC[0].value = 73;
	sc->asdrCCValue[0].value = 64;
	sc->asdrCC[2].value = 80;
	sc->asdrCCValue[2].value = 64;
	sc->asdrCC[3].value = 72;
	sc->asdrCCValue[3].value = 64;
	sc->midiKnobCC[0].value = 74;
	sc->midiKnobName[0].value = L"Cutoff";
	sc->midiKnobCC[1].value = 71;
	sc->midiKnobName[1].value = L"Reso";
	sc->midiKnobCC[2].value = 91;
	sc->midiKnobName[2].value = L"Reverb";
	sc->midiKnobCC[3].value = 93;
	sc->midiKnobName[3].value = L"Chorus";
	sc->midiKnobCC[4].value = 94;
	sc->midiKnobName[4].value = L"Delay";
	sc->range.value = 12;
	_presets.push_back(sc);
}

Plugin::~Plugin()
{
	if (_gui != nullptr)
	{
		delete _gui;
		_gui = nullptr;
	}

	if (_state != nullptr)
	{
		delete _state;
		_state = nullptr;
	}

	for (int i = 0; i < _presets.size(); i++)
	{
		delete _presets[i];
	}
	_presets.clear();
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

		PlugHost->Dispatcher(HostTag, FHD_SetNumPresets, 0, _presets.size());
		PlugHost->Dispatcher(HostTag, FHD_SetNumParams, 0, _state->all.size());
		ProcessAllParams();
	}
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

intptr_t _stdcall Plugin::Dispatcher(intptr_t ID, intptr_t Index, intptr_t Value)
{
	//if (ID == FPD_ConvertStringToValue)
	//{
	//	PConvertStringToValueData inf = (PConvertStringToValueData)Value;
	//	std::string s = inf->StringValue;
	//	std::wstring ws(s.begin(), s.end());

	//	PlugParameter* param = getParameterByIndex(inf->ParamIndex);
	//	if (param != nullptr)
	//	{

	//	}
	//}

	intptr_t r = TCPPFruityPlug::Dispatcher(ID, Index, Value);
	if (r != 0)
		return r;

	//if (ID == FPD_UseVoiceLevels)
	//	return 2; //Plugin supports voice levels for a custom function
	/*else */if (ID == FPD_ShowEditor)
	{
		if (Value == 0)
		{
			if (_gui != nullptr)
			{
				// close editor
				_gui->close();
				EditorHandle = 0;
				delete _gui;
				_gui = nullptr;
			}
		}
		else if (EditorHandle == 0)
		{
			PlugHost->Dispatcher(HostTag, FHD_SetNumPresets, 0, _presets.size());
			PlugHost->Dispatcher(HostTag, FHD_SetNumParams, 0, _state->all.size());
			PlugHost->Dispatcher(HostTag, FHD_NamesChanged, 0, FPN_Semitone);

			// open editor
			_gui = new PluginGUI(this);
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
	else if (ID == FPD_SetPreset)
	{
		if (Index < _presets.size())
		{
			Preset* p = _presets[Index];

			DataStream s2 = DataStream();
			p->serialize(&s2);
			s2.setPosition(0);

			_state->preset.deserialize(&s2);
			PlugHost->Dispatcher(HostTag, FHD_ShowEditor, 0, 0);
			PlugHost->Dispatcher(HostTag, FHD_ShowEditor, 0, 1);
			

			//if (_gui != nullptr)
			//{
			//	_gui->close();
			//	EditorHandle = 0;
			//	delete _gui;
			//	_gui = nullptr;

			//	// open editor
			//	_gui = new PluginGUI(this);
			//	_gui->open(reinterpret_cast<HWND>(Value));
			//	_gui->getFrame()->takeFocus();
			//	EditorHandle = static_cast<HWND>(_gui->getFrame()->getSystemWindow());
			//}

			//for (int i = 0; i < _state->all.size(); i++)
			//{
			//	PlugParameter* p = getParameterByIndex(i);
			//	if (p != nullptr)
			//	{
			//		float realValue = p->getFloat();
			//		ProcessParam(i, *((int*)(&realValue)), (int)AutomationTypeFlags::UpdateControl);
			//	}
			//}
		}
	}
	else if (ID == FPD_GetParamType)
	{
		int qq = 1;
		//const int PT_Default = 0;
		//const int PT_Db = 1;
		//const int PT_Hz = 2;
		//const int PT_Centered = 3;
		//const int PT_Ms = 4;
		//const int PT_Percent = 5;
		//const int PT_Time = 6;
		//const int PT_Value = 7;
		//const int PT_Number = 8;
		//const int PT_Text = 9;

		return PT_Text;
	}
	//else if (ID == FPD_GetParamInfo)
	//{
	//	int qq = 1;
	//	return PI_Float;
	//}
	//else if (ID == FPD_ConvertStringToValue)
	//{
	//	PConvertStringToValueData inf = (PConvertStringToValueData)Value;
	//	std::string s = inf->StringValue;
	//	std::wstring ws(s.begin(), s.end());

	//	PlugParameter* param = getParameterByIndex(inf->ParamIndex);
	//	if (param != nullptr)
	//	{

	//	}
	//}
	return 0;
}


inline AutomationTypeFlags operator|(AutomationTypeFlags a, AutomationTypeFlags b)
{
	return static_cast<AutomationTypeFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline AutomationTypeFlags operator&(AutomationTypeFlags a, AutomationTypeFlags b)
{
	return static_cast<AutomationTypeFlags>(static_cast<int>(a) & static_cast<int>(b));
}

int _stdcall Plugin::ProcessParam(int index, int value, int RECFlags)
{
	AutomationTypeFlags type = (AutomationTypeFlags)RECFlags;
	bool fromMidi = ((int)type & (int)AutomationTypeFlags::FromMIDI);
	bool updateValue = (int)type & (int)AutomationTypeFlags::UpdateValue;

	bool needSetValue = updateValue || fromMidi;
	bool getValue = (int)type & (int)AutomationTypeFlags::GetValue;
	bool updateUI = (int)type & (int)AutomationTypeFlags::UpdateControl;
	bool getValueUI = ((int)type & (int)AutomationTypeFlags::GetValueUI);

	PlugParameter* param = getParameterByIndex(index);
	if (param == nullptr)
		return 0;

	bool didSomething = false;
	if ((int)type & (int)AutomationTypeFlags::GetValue)
	{
		ParameterInt* intparam = dynamic_cast<ParameterInt*>(param);
		if (intparam != nullptr)
			value = intparam->value;
		else
		{
			float realVal = param->getFloat();
			value = *((int*)(&realVal));
		}
		didSomething = true;
	}

	if ((int)type & (int)AutomationTypeFlags::FromMIDI)
	{
		value = TranslateMidi(value, (int)param->getMin(), (int)param->getMax());
		didSomething = true;
	}

	if ((int)type & (int)AutomationTypeFlags::UpdateValue)
	{
		int max = param->getMax();
		if (param != nullptr)
		{
			param->setFloat(value);

			if ((param->midiTrigger & MidiTrigger::BankChange) != MidiTrigger::None)
				UpdateBankChange(_state->channel.value);

			if ((param->midiTrigger & MidiTrigger::PortChange) != MidiTrigger::None)
				_ccHistory.clear();	
			
			if ((param->midiTrigger & MidiTrigger::MidiParameterChange) != MidiTrigger::None)
			{
				if (param->midi.shouldTrigger())
				{
					byte port = (byte)_state->port.value;
					byte chan = (byte)_state->channel.value - 1;
					MIDIOut((byte)(176 | chan), param->midi.cc->value, param->midi.value->value, port);
				}
			}
		}
		didSomething = true;

	}

	if ((int)type & (int)AutomationTypeFlags::UpdateControl && _gui != nullptr)
	{
		_gui->setParameter(param->hash, value);
		didSomething = true;
	}

	if(!didSomething)
		return TCPPFruityPlug::ProcessParam(index, value, RECFlags);
	
	return value;
}

void _stdcall Plugin::GetName(int Section, int Index, int Value, char* Name)
{
	if (Section == FPN_Param)
	{
		PlugParameter* param = getParameterByIndex(Index);
		if (param != nullptr)
			wcstombs(Name, param->name.c_str(), param->name.length() + 1);
	}
	else if (Section == FPN_ParamValue)
	{
		PlugParameter* param = getParameterByIndex(Index);
		if (param != nullptr)
		{
			std::wstring str = param->toString();
			wcstombs(Name, str.c_str(), str.length() + 1);
		}
	}
	else if (Section == FPN_Preset)
	{
		if(Index < _presets.size())
			wcstombs(Name, _presets[Index]->presetName.c_str(), _presets[Index]->presetName.length() + 1);
	}
	else if (Section == FPN_Semitone)
	{
		auto it = _state->preset.noteNames.find(Index);
		if (it != _state->preset.noteNames.end())
			wcstombs(Name, (*it).second.c_str(), (*it).second.length() + 1);
	}
	else if (Section == FPN_VoiceLevel || Section == FPN_VoiceLevelHint)
	{
		//System::String* str = synthman::Plugin$_CE_GetNoteControlName(owner, Index);
		//if (str != nullptr)
		//	wcstombs(Name, &str->start_char, (str->length + 1) * 2);
	}
}

byte Plugin::CalculateHarmonic(byte note)
{
	int key = _state->harmonyKey.value;
	int offset = _state->harmony.value - 7;

	if (offset != 0)
	{
		int type = _state->harmonyType.value;
		Scale scale = _scales[type];
		//"-Octave|-8", "-7th|-7", "-6th|-6", "-5th|-5", "-4th|-4", "-3rd|-3", "-2nd|-2", "NO", "2nd|2", "3rd|3", "4th|4", "5th|5", "6th|6", "7th|7", "Octave|8"

		if (offset == -7)
			return (byte)(note - 12);
		else if (offset == 7)
			return (byte)(note + 12);

		Key unharmonizedNote = (Key)(note % 12);
		Key harmonizedNote = scale.Harmonize((Key)key, unharmonizedNote, offset);

		if (offset < 0)
		{
			if (harmonizedNote < unharmonizedNote)
				note -= (byte)((int)unharmonizedNote - (int)harmonizedNote);
			else
				note -= (byte)((int)(unharmonizedNote + 12) - (int)harmonizedNote);
		}
		else
		{
			if (harmonizedNote > unharmonizedNote)
				note += (byte)((int)harmonizedNote - (int)unharmonizedNote);
			else
				note += (byte)((int)(harmonizedNote + 12) - (int)unharmonizedNote);
		}

		return note;
	}
	return 0;
}

void _stdcall Plugin::Gen_Render(PWAV32FS DestBuffer, int& Length)
{
	for (int i = 0; i < _notes.size(); i++)
	{
		//Update note
		Note* n = _notes[i];
		n->pan = n->params->FinalLevels.Pan;
		n->volume = n->params->FinalLevels.Vol;
		n->pitch = n->params->FinalLevels.Pitch /* + thi$->_masterPitchInCents*/;
		n->control1 = n->params->FinalLevels.FCut;
		n->control2 = n->params->FinalLevels.FRes;

		int realChannel = _state->channel.value + n->channel;
		if (n->state == NoteState::Finished || n->state == NoteState::NoteOff)
		{
			if (_state->enableNoteTrigger.value)
				MIDIOut((byte)(128 | (realChannel - 1)), (byte)n->startingNote, (byte)min((n->volume) * 127, 127), (byte)_state->port.value);

			if (n->harmonic != 0 && _state->harmonizerEnabled.value)
				MIDIOut((byte)(128 | (realChannel - 1)), (byte)n->harmonic, (byte)min((n->volume) * 127, 127), (byte)_state->port.value);

			n->state = NoteState::Finished;
		}
		else if (n->state == NoteState::NoteOn)
		{
			//n->preset = presets[currentPresetIndex.value];

			letters[_pulseIndex] = n->hostTag;
			_pulseIndex = (_pulseIndex + 1) % 5;

			byte midiNote = (byte)((n->pitch / 100) + 60);
			n->startingNote = midiNote;
			n->lastPitchDif = 0;

			if (_state->harmonizerEnabled.value)
				n->harmonic = CalculateHarmonic(midiNote);

			//Trigger Note
			//---------------------------------------------------------------
			if (realChannel != _state->channel.value)
			{
				UpdateBankChange(realChannel);
				EnsureCCsForChannel(_state->channel.value, realChannel, (byte)_state->port.value);
			}

			UpdateNoteControls(n);

			WritePan(n);
			WriteVolume(n);
			WritePitch(n, true);

			if (_state->enableNoteTrigger.value)
				MIDIOut((byte)(144 | (realChannel - 1)), (byte)n->startingNote, (byte)min((n->volume) * 127, 127), (byte)_state->port.value);

			if (n->harmonic != 0 && _state->harmonizerEnabled.value)
				MIDIOut((byte)(144 | (realChannel - 1)), (byte)n->harmonic, (byte)min((n->volume) * 127, 127), (byte)_state->port.value);
			//---------------------------------------------------------------
		}
		else if (n->state == NoteState::Playing)
		{
			UpdateNoteControls(n);
			WritePan(n);
			WriteVolume(n);
			WritePitch(n);
		}




		if (n->state == NoteState::NoteOn)
			n->state = NoteState::Playing;

		//C_FinishUpdateNote(note);
		if (n->state == NoteState::Finished)
		{
			for (int i = 0; i < 5; i++)
			{
				if (letters[i] == n->hostTag)
					letters[i] = 0;
			}

			delete n;
			_notes.erase(_notes.begin() + i);
			i--;
		}
	}
}

void _stdcall Plugin::Idle_Public()
{
	if (_gui != nullptr)
		_gui->idle();
}

TVoiceHandle _stdcall Plugin::TriggerVoice(PVoiceParams VoiceParams, intptr_t SetTag)
{
	Note* n = new Note();
	n->hostTag = SetTag;
	n->state = NoteState::NoteOn;
	n->channel = PlugHost->Voice_ProcessEvent(SetTag, FPV_GetColor, 0, 0);
	n->params = VoiceParams;

	_notes.push_back(n);

	return (TVoiceHandle)n;
}

void _stdcall Plugin::Voice_Release(TVoiceHandle Handle)
{
	((Note*)Handle)->state = NoteState::NoteOff;
}

void _stdcall Plugin::Voice_Kill(TVoiceHandle Handle)
{
	((Note*)Handle)->state = NoteState::Finished;
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

PlugParameter* Plugin::getParameterByIndex(int index)
{
	if (index >= 0 && index < _state->all.size())
		return dynamic_cast<PlugParameter*>(_state->all[index]);

	return nullptr;
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

void Plugin::MIDIOut(unsigned char status, unsigned char data1, unsigned char data2, unsigned char port)
{
	TMIDIOutMsg msg;
	msg.Status = status;
	msg.Data1 = data1;
	msg.Data2 = data2;
	msg.Port = port;
	PlugHost->MIDIOut_Delayed(HostTag, *(int*)&msg);
	//PlugHost->MIDIOut(HostTag, *(int*)&msg);
}

void Plugin::UpdateNoteControls(Note* note)
{

	if (_updatedThisFrame.find(note->channel) == _updatedThisFrame.end())
	{
		_updatedThisFrame.emplace(note->channel);
		for (int i = 0; i < 2; i++)
		{
			if (_state->assignedNoteControls[i].linkHash != -1)
			{
				PlugParameter* param = getParameter(_state->assignedNoteControls[i].linkHash);
				if (param == nullptr)
					continue;

				ProcessParam(param->hash, param->normalizedToRange(((i == 0 ? note->control1 : note->control2) + 1.0f) / 2.0f), (int)(AutomationTypeFlags::UpdateValue | AutomationTypeFlags::UpdateControl));
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (_state->assignedPartialNoteControls[i].linkHash != -1)
			{
				PlugParameter* param = getParameter(_state->assignedPartialNoteControls[i].linkHash);
				if (param == nullptr)
					continue;

				NoteControlType type = (NoteControlType)((i % 2) + 1);
				NoteControlIndex idx = (NoteControlIndex)((int)(i / 2));
				float controlVal = (idx == NoteControlIndex::Control1 ? note->control1 : note->control2);
				if (type == NoteControlType::Upper)
					controlVal = max(controlVal, 0.0f);
				else
					controlVal = min(controlVal, 0.0f) * -1;

				ProcessParam(param->hash, param->normalizedToRange(controlVal), (int)(AutomationTypeFlags::UpdateValue | AutomationTypeFlags::UpdateControl));
			}
		}
	}
}

void Plugin::AssignNoteControl(int control, NoteControlIndex ncIndex, NoteControlType type)
{

}

void Plugin::UpdateBankChange(int channel)
{
	if (_state->midiPatch.value != 0)
	{
		int realValue = 0;
		int outHash = _state->port.value + channel;
		int valHash = _state->midiPatch.value + _state->midiBankCoarse.value + _state->midiBankFine.value;

		auto it = _midiPatchStatusCache.find(outHash);
		if (it == _midiPatchStatusCache.end())
			_midiPatchStatusCache[outHash] = realValue = 0;
		else
			realValue = (*it).second;

		if (realValue != valHash)
		{
			MIDIOut((byte)(176 | (channel - 1)), 0, (byte)_state->midiBankCoarse.value, (byte)_state->port.value);
			MIDIOut((byte)(176 | (channel - 1)), 32, (byte)_state->midiBankFine.value, (byte)_state->port.value);
			MIDIOut((byte)(192 | (channel - 1)), (byte)(_state->midiPatch.value - 1), 0, (byte)_state->port.value);
			_midiPatchStatusCache[outHash] = valHash;
		}
	}
}

void Plugin::EnsureCCsForChannel(int mainChannel, int subChannel, byte port)
{
	std::map<byte, byte>* pair1 = nullptr;
	std::map<byte, byte>* pair2 = nullptr;
	auto it = _ccHistory.find(mainChannel);
	if (it != _ccHistory.end())
	{
		pair1 = &(*it).second;
		it = _ccHistory.find(subChannel);
		if (it == _ccHistory.end())
		{
			_ccHistory[subChannel] = std::map<byte, byte>();
			pair2 = &_ccHistory[subChannel];
		}
		else
			pair2 = &(*it).second;

		for (auto pair = pair1->begin(); pair != pair1->end(); pair++)
		{
			byte secondary = 255;
			auto find2 = pair2->find((*pair).first);
			if (find2 == pair2->end() || (*pair).second != secondary)
			{
				MIDIOut((byte)(176 | (subChannel - 1)), (*pair).first, (*pair).second, port);
				(*pair2)[(*pair).first] = (*pair).second;
			}
		}
	}
}

void Plugin::WritePitch(Note* note, bool zeroPitch)
{
	if (!_state->enableNotePitch.value)
		return;

	byte portVal = (byte)_state->port.value;
	byte chan = (byte)((_state->channel.value + note->channel) - 1);

	//MIDIOut((byte)(176 | chan), 101, 0, port);
	//MIDIOut((byte)(176 | chan), 100, 0, port);
	//MIDIOut((byte)(176 | chan), 6, (byte)parms.range.value, port);
	//MIDIOut((byte)(176 | chan), 38, 0, port);


	float middlePitch = 8192;
	if (zeroPitch)
		MIDIOut((byte)(224 | chan), 0, (byte)((int)middlePitch >> 7), (byte)portVal);
	else
	{
		note->vibratoSin += note->startingNote - 120;

		int pitchDif = (int)note->pitch - ((note->startingNote - 60) * 100);
	
		int maxDif = _state->preset.range.value * 100;
		if (pitchDif < -maxDif)
			pitchDif = -maxDif;
		if (pitchDif > maxDif)
			pitchDif = maxDif;

		//pitchDif += (int)(_vibrato * 100);

		if (note->lastPitchDif != pitchDif /* && pNote.updatedThisFrame == false*/)
		{
			unsigned short pitchVal = (unsigned short)(middlePitch + (((float)pitchDif / maxDif) * (middlePitch - 1)));
			MIDIOut((byte)(224 | chan), (byte)(pitchVal & 127), (byte)(pitchVal >> 7), (byte)portVal);
		}
	}
}

void Plugin::WritePan(Note* note)
{
	if (!_state->enableNotePanning.value)
		return;

	byte pan = (byte)(((note->pan + 1.0f) / 2.0f) * 127);
	if (pan != note->lastSetPan)
	{
		byte portVal = (byte)_state->port.value;
		byte chan = (byte)((_state->channel.value + note->channel) - 1);

		note->lastSetPan = pan;
		MIDIOut((byte)(176 | chan), 10, pan, portVal);
	}
}

void Plugin::WriteVolume(Note* note)
{
	if (!_state->enableNoteVelocity.value)
		return;

	byte volume = (byte)min((note->volume * 1.45f) * 127, 127);
	if (volume != note->lastSetVolume)
	{
		byte portVal = (byte)_state->port.value;
		byte chan = (byte)((_state->channel.value + note->channel) - 1);

		note->lastSetVolume = volume;
		MIDIOut((byte)(176 | chan), 7, volume, portVal);
	}
}
