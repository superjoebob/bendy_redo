#pragma once
#include "Preset.h"
#include <vector>
class Preset;
struct State : Serializable
{
	State();
	~State();
	virtual void serialize(StreamWrapper* s) override;
	virtual void deserialize(StreamWrapper* s) override;

	int versionMajor;
	int versionMinor;

	ParameterInt currentPresetIndex;
	ParameterLinkList assignedNoteControls;
	ParameterLinkList assignedPartialNoteControls;

	ParameterInt channel;
	ParameterInt port;
	ParameterBool enableNoteTrigger;
	ParameterBool enableNoteVelocity;
	ParameterBool enableNotePanning;
	ParameterBool enableNotePitch;
	ParameterBool expanded;

	ParameterBool harmonizerEnabled;
	ParameterStringSelection harmony;
	ParameterStringSelection harmonyKey;
	ParameterStringSelection harmonyType;

	ParameterStringSelection midiPatch;
	ParameterInt midiBankCoarse;
	ParameterInt midiBankFine;

	ParameterBool vibratoEnabled;
	ParameterFloat vibratoSpeed;
	ParameterFloat vibratoDepth;

	Preset** presets;
	Preset currentPreset;

private:
	void cleanup();
};