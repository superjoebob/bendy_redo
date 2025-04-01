#pragma once
#include "fp_cplug.h"
#include "fp_def.h"
#include "Scale.h"
#include <unordered_set>

struct FruityPlugInfo
{
	int Tag;
	TFruityPlugHost* Host;
};

// voice
struct TPlugVoice {
	int HostTag;
	PVoiceParams Params;
	int Pos[2];
	int State;
};
typedef TPlugVoice* PPlugVoice;

enum class NoteState
{
	None = 0,
	NoteOn = 1,
	Playing = 2,
	NoteOff = 3,
	Finished = 4
};

struct Note
{
	intptr_t hostTag = 0;
	NoteState state = NoteState::None;
	int channel = 0;
	PVoiceParams params = nullptr;

	int velocity = 0;
	byte midiVelocity = 0;
	int initialVelocity = 0;
	float channelVolume = 0.0f;
	float pan = 0.0f;
	float pitch = 0.0f;
	float control1 = 0.0f;
	float control2 = 0.0f;

	//Bendy specific
	char harmonic = 0;
	char startingNote = 255;
	int lastSetVolume = -1;
	float vibratoSin = 0.0f;
};

enum NoteControlIndex
{
	NoControl = -1,
	Control1 = 0,
	Control2 = 1,
};

enum NoteControlType
{
	Full = 0,
	Upper = 1,
	Lower = 2
};


class PluginGUI;
struct State;
struct Serializable;
struct Preset;
struct PlugParameter;
class Plugin : public TCPPFruityPlug
{
public:
	Plugin(void* data);
	~Plugin();

	virtual void _stdcall SaveRestoreState(IStream* Stream, BOOL Save);

	virtual intptr_t _stdcall Dispatcher(intptr_t ID, intptr_t Index, intptr_t Value);
	virtual int _stdcall ProcessParam(int Index, int Value, int RECFlags);
	virtual int _stdcall ProcessEvent(int EventID, int EventValue, int Flags);
	virtual void _stdcall GetName(int Section, int Index, int Value, char* Name);

	virtual void _stdcall Gen_Render(PWAV32FS DestBuffer, int& Length);
	virtual void _stdcall NewTick();

	void _stdcall Idle_Public();

	virtual TVoiceHandle _stdcall TriggerVoice(PVoiceParams VoiceParams, intptr_t SetTag);
	virtual void _stdcall Voice_Release(TVoiceHandle Handle);
	virtual void _stdcall Voice_Kill(TVoiceHandle Handle);

	Serializable* getSerializable(std::wstring control);
	PlugParameter* getParameter(std::wstring control);
	PlugParameter* getParameter(int hash);
	PlugParameter* getParameterByIndex(int index);
	int getCID(std::wstring control);
	std::wstring toString(std::wstring control);

	void MIDIOut(unsigned char status, unsigned char data1, unsigned char data2, unsigned char port);
	void UpdateNoteControls(Note* note);
	void AssignNoteControl(int control, NoteControlIndex ncIndex, NoteControlType type);
	bool HasNoteControl(int hash);

	void MakeChannelDirty(int channel);
	void UpdateBankChange(int channel);
	void EnsureCCsForChannel(int mainChannel, int subChannel, byte port);

	void WritePitch(Note* note, bool zeroPitch = false);
	void WritePan(Note* note);
	void WriteVolume(Note* note);

	void _SendBendRange();

	int letters[5];
protected:
	PluginGUI* _gui;
	State* _state;
	int _pulseIndex;

	std::vector<Preset*> _presets;
	std::vector<Note*> _notes;
	std::map<int, int> _midiPatchStatusCache;
	std::map<int, byte> _midiPanCache;
	std::map<int, unsigned short> _midiPitchCache;
	std::map<int, byte> _midiVolumeCache;
	std::map<int, std::map<byte, byte>> _ccHistory;
	std::vector<Scale> _scales;
	std::unordered_set<int> _updatedThisFrame;
	int _lastSetPan = -1;

	int _masterPitchCents;
	int _masterVolume = 100;
	int _masterPan = 64;

	byte CalculateHarmonic(byte note);
};