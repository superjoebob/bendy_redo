#pragma once
#include "Parameter.h"

struct Preset : Serializable
{
	Preset();
	virtual void serialize(Stream* s) override;
	virtual void deserialize(Stream* s) override;
	virtual void legacy_deserialize(Stream* s) override;

	std::wstring presetName;
	std::map<int, std::wstring> noteNames;
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