#pragma once
#include "Preset.h"
#include <vector>
class Preset;
struct State : Serializable
{
	State();
	~State();
	virtual void serialize(Stream* s) override;
	virtual void deserialize(Stream* s) override;
	virtual void legacy_deserialize(Stream* s) override;

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

	Preset preset;

private:
	void cleanup();
	std::map<unsigned short, Serializable*> _legacyIndexMap;
};