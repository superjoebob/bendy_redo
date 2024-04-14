#include "Preset.h"
#include "StreamWrapper.h"
Preset::Preset() :
	Serializable(L"BendyPreset", L"preset"),
	name(L""),
	range(L"Pitch Bend Range (In Semitones)", L"range", 2, 1, 64),
	midiKnobValue(L"CC Knob", L"midiKnobValue", 8, 0, 0, 127),
	midiKnobCC(L"Midi Automation", L"midiKnobCC", 8, 0),
	midiKnobName(L"Midi Automation Name", L"midiKnobName", 8, L"NO NAME"),
	midiKnobEnabled(L"CC Enable ", L"midiKnobEnabled", 8, false),
	asdrCCEnable(L"Enable ASDR CC", L"asdrCCEnable", false),
	asdrCCName(L"ASDR CC Name", L"asdrCCName", L"ASDR CC"),
	asdrCCValue(L"Midi ASDR Automation", L"asdrCCValue", 4, 0, 0, 127),
	asdrCC(L"Midi ASDR Automation CC", L"asdrCC", 4, 0)
{
	map(&range);
	map(&midiKnobValue);
	map(&midiKnobCC);
	map(&midiKnobName);
	map(&midiKnobEnabled);

	map(&asdrCCEnable);
	map(&asdrCCName);

	map(&asdrCCValue);
	map(&asdrCC);
}

void Preset::serialize(StreamWrapper* s)
{
	s->writeString(name);
	Serializable::serialize(s);
	s->writeUShort(0);//no note names yet
}

void Preset::deserialize(StreamWrapper* s)
{
	name = s->readString();
	int numNoteNames = s->readUShort();
	for (int i = 0; i < numNoteNames; i++)
	{
		char key = s->readChar();
		std::wstring value = s->readString();
	}

	Serializable::deserialize(s);
}