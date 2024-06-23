#pragma once
#include "Parameter.h"
struct Preset : Serializable
{
	Preset();
	virtual void serialize(StreamWrapper* s) override;
	virtual void deserialize(StreamWrapper* s) override;
	virtual void legacy_deserialize(StreamWrapper* s) override;

	std::wstring name;
	ParameterInt range;
	ParameterIntList midiKnobValue;
	ParameterMidiCCList midiKnobCC;
	ParameterStringList midiKnobName;
	ParameterBoolList midiKnobEnabled;

	ParameterBool asdrCCEnable;
	ParameterString asdrCCName;

	ParameterIntList asdrCCValue;
	ParameterMidiCCList asdrCC;
};