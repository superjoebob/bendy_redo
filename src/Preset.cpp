#include "Preset.h"
#include "StreamWrapper.h"
Preset::Preset() :
	Serializable(L"BendyPreset", L"preset"),
	range(L"Pitch Bend Range (In Semitones)", L"range", 2, 1, 64),
	midiKnobValue(L"CC Knob", L"midiKnobValue", 8, -1, 0, 127),
	midiKnobCC(L"Midi Automation", L"midiKnobCC", 8, 0),
	midiKnobName(L"Midi Automation Name", L"midiKnobName", 8, L"NO NAME"),
	midiKnobEnabled(L"CC Enable ", L"midiKnobEnabled", 8, -1),
	asdrCCEnable(L"Enable ASDR CC", L"asdrCCEnable", -1),
	asdrCCName(L"ASDR CC Name", L"asdrCCName", L"ASDR CC"),
	asdrCCValue(L"Midi ASDR Automation", L"asdrCCValue", 4, 0, 0, 127),
	asdrCC(L"Midi ASDR Automation CC", L"asdrCC", 4, 0)
{
	map(&range);
	range.midiTrigger = MidiTrigger::BendRangeChange;

	map(&midiKnobValue);
	map(&midiKnobCC);
	map(&midiKnobName);
	map(&midiKnobEnabled);

	map(&asdrCCEnable);
	map(&asdrCCName);

	map(&asdrCCValue);
	map(&asdrCC);

	MidiCluster c = MidiCluster();
	for (int i = 0; i < 8; i++)
	{
		c.cc = &midiKnobCC[i];
		c.value = &midiKnobValue[i];
		c.name = &midiKnobName[i];
		c.enabled = &midiKnobEnabled[i];

		midiKnobValue[i].midi = c;
		midiKnobValue[i].midiTrigger = MidiTrigger::MidiParameterChange;
		midiKnobCC[i].midi = c;
		midiKnobCC[i].midiTrigger = MidiTrigger::MidiParameterChange;
		midiKnobName[i].midi = c;
		midiKnobName[i].midiTrigger = MidiTrigger::MidiParameterChange;
		midiKnobEnabled[i].midi = c;
		midiKnobEnabled[i].midiTrigger = MidiTrigger::MidiParameterChange;
	}

	c.enabled = &asdrCCEnable;
	c.name = &asdrCCName;
	asdrCCEnable.midi = c;
	asdrCCName.midi = c;
	for (int i = 0; i < 4; i++)
	{
		c.cc = &asdrCC[i];
		c.value = &asdrCCValue[i];

		asdrCC[i].midi = c;
		asdrCC[i].midiTrigger = MidiTrigger::MidiParameterChange;
		asdrCCValue[i].midi = c;
		asdrCCValue[i].midiTrigger = MidiTrigger::MidiParameterChange;
	}

}

void Preset::serialize(Stream* s)
{
	s->writeString(name);
	s->writeUShort(noteNames.size());
	for (auto it = noteNames.begin(); it != noteNames.end(); it++)
	{
		s->writeChar((*it).first);
		s->writeString((*it).second);
	}
	Serializable::serialize(s);
}
void Preset::deserialize(Stream* s)
{
	name = s->readString();

	noteNames.clear();
	int numNoteNames = s->readUShort();
	for (int i = 0; i < numNoteNames; i++)
	{
		char key = s->readChar();
		std::wstring value = s->readString();
		noteNames[key] = value;
	}

	Serializable::deserialize(s);
}

void Preset::legacy_deserialize(Stream* s)
{
	name = s->readString();
	int numNoteNames = s->readUShort();
	for (int i = 0; i < numNoteNames; i++)
	{
		char key = s->readChar();
		std::wstring value = s->readString();
	}

	Serializable::legacy_deserialize(s);
}