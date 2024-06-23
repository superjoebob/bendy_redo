#pragma once
#include "fp_cplug.h"
#include "fp_def.h"
#include <vector>
#include <string>

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


class State;
class PluginGUI;
class Serializable;
struct PlugParameter;
class Plugin : public TCPPFruityPlug
{
public:
	Plugin(void* data);
	~Plugin();

	virtual void _stdcall SaveRestoreState(IStream* Stream, BOOL Save);

	virtual intptr_t _stdcall Dispatcher(intptr_t ID, intptr_t Index, intptr_t Value);
	virtual int _stdcall ProcessParam(int Index, int Value, int RECFlags);

	virtual void _stdcall Gen_Render(PWAV32FS DestBuffer, int& Length);

	virtual TVoiceHandle _stdcall TriggerVoice(PVoiceParams VoiceParams, int SetTag);
	virtual void _stdcall Voice_Release(TVoiceHandle Handle);
	virtual void _stdcall Voice_Kill(TVoiceHandle Handle);

	Serializable* getSerializable(std::wstring control);
	PlugParameter* getParameter(std::wstring control);
	PlugParameter* getParameter(int hash);
	int getCID(std::wstring control);
	std::wstring toString(std::wstring control);

protected:
	PluginGUI* _gui;
	State* _state;
};