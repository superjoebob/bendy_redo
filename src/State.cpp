#include "State.h"
#include "StreamWrapper.h"
#include "Preset.h"

State::State():
    Serializable(L"State", L"state"),
	versionMajor(0),
	versionMinor(0),
    currentPresetIndex(L"Current Preset Index", L"currentPresetIndex", 0, 0, INT32_MAX),
    assignedNoteControls(L"Note Control Parameter Index", L"assignedNoteControls", 2),
    assignedPartialNoteControls(L"Note Control Upper Parameter Index", L"assignedPartialNoteControls", 4),
	channel(L"BENDY MIDI Channel", L"channel", 1, 1, 16),
	port(L"BENDY MIDI Port", L"port", 0, 0, 64),
	enableNoteTrigger(L"Trigger Midi Notes (Separate from Harmony)", L"enableNoteTrigger", true),
	enableNoteVelocity(L"Note Velocity Override", L"enableNoteVelocity", false),
	enableNotePanning(L"Note Panning Enable", L"enableNotePanning", true),
	enableNotePitch(L"Note Pitch Bend Enable", L"enableNotePitch", true),
	expanded(L"Advanced Options", L"expanded", false),
	harmonizerEnabled(L"Enable Harmonizer", L"harmonizerEnabled", true),
	harmony(L"Harmonize Note", L"harmony", { L"-Octave|-8", L"-7th|-7", L"-6th|-6", L"-5th|-5", L"-4th|-4", L"-3rd|-3", L"-2nd|-2", L"No Harmonizing|NO", L"2nd|2", L"3rd|3", L"4th|4", L"5th|5", L"6th|6", L"7th|7", L"Octave|8" }, 7),
	harmonyKey(L"Harmonize Key", L"harmonyKey", { L"C", L"C#", L"D", L"D#", L"E", L"F", L"F#", L"G", L"G#", L"A", L"A#", L"B" }, 0),
	harmonyType(L"Harmonize Scale", L"harmonyType", { L"Major|maj", L"Minor|min", L"Harmonic Minor|hMin", L"Melodic Minor|mMin" }, 0),
	midiPatch(L"MIDI Patch Number", L"midiPatch", {
            L"(No Patch Set)",
            L"Acoustic Grand Piano",
            L"Bright Acoustic Piano",
            L"Electric Grand Piano",
            L"Honky-tonk Piano",
            L"Electric Piano 1",
            L"Electric Piano 2",
            L"Harpsichord",
            L"Clavi",
            L"Celesta",
            L"Glockenspiel",
            L"Music Box",
            L"Vibraphone",
            L"Marimba",
            L"Xylophone",
            L"Tubular Bells",
            L"Dulcimer",
            L"Drawbar Organ",
            L"Percussive Organ",
            L"Rock Organ",
            L"Church Organ",
            L"Reed Organ",
            L"Accordion",
            L"Harmonica",
            L"Tango Accordion",
            L"Guitar(nylon)",
            L"Acoustic Guitar(steel)",
            L"Electric Guitar(jazz)",
            L"Electric Guitar(clean)",
            L"Electric Guitar(muted)",
            L"Overdriven Guitar",
            L"Distortion Guitar",
            L"Guitar harmonics",
            L"Acoustic Bass",
            L"Electric Bass(finger)",
            L"Electric Bass(pick)",
            L"Fretless Bass",
            L"Slap Bass 1",
            L"Slap Bass 2",
            L"Synth Bass 1",
            L"Synth Bass 2",
            L"Violin",
            L"Viola",
            L"Cello",
            L"Contrabass",
            L"Tremolo Strings",
            L"Pizzicato Strings",
            L"Orchestral Harp",
            L"Timpani",
            L"String Ensemble 1",
            L"String Ensemble 2",
            L"SynthStrings 1",
            L"SynthStrings 2",
            L"Choir Aahs",
            L"Voice Oohs",
            L"Synth Voice",
            L"Orchestra Hit",
            L"Trumpet",
            L"Trombone",
            L"Tuba",
            L"Muted Trumpet",
            L"French Horn",
            L"Brass Section",
            L"SynthBrass 1",
            L"SynthBrass 2",
            L"Soprano Sax",
            L"Alto Sax",
            L"Tenor Sax",
            L"Baritone Sax",
            L"Oboe",
            L"English Horn",
            L"Bassoon",
            L"Clarinet",
            L"Piccolo",
            L"Flute",
            L"Recorder",
            L"Pan Flute",
            L"Blown Bottle",
            L"Shakuhachi",
            L"Whistle",
            L"Ocarina",
            L"Lead 1 (square)",
            L"Lead 2 (sawtooth)",
            L"Lead 3 (calliope)",
            L"Lead 4 (chiff)",
            L"Lead 5 (charang)",
            L"Lead 6 (voice)",
            L"Lead 7 (fifths)",
            L"Lead 8 (bass+lead)",
            L"Pad 1 (new age)",
            L"Pad 2 (warm)",
            L"Pad 3 (polysynth)",
            L"Pad 4 (choir)",
            L"Pad 5 (bowed)",
            L"Pad 6 (metallic)",
            L"Pad 7 (halo)",
            L"Pad 8 (sweep)",
            L"FX 1 (rain)",
            L"FX 2 (soundtrack)",
            L"FX 3 (crystal)",
            L"FX 4 (atmosphere)",
            L"FX 5 (brightness)",
            L"FX 6 (goblins)",
            L"FX 7 (echoes)",
            L"FX 8 (sci-fi)",
            L"Sitar",
            L"Banjo",
            L"Shamisen",
            L"Koto",
            L"Kalimba",
            L"Bag pipe",
            L"Fiddle",
            L"Shanai",
            L"Tinkle Bell",
            L"Agogo",
            L"Steel Drums",
            L"Woodblock",
            L"Taiko drum",
            L"Melodic Tom",
            L"Synth Drum",
            L"Reverse Cymbal",
            L"Guitar Fret Noise",
            L"Breath Noise",
            L"Seashore",
            L"Bird Tweet",
            L"Telephone Ring",
            L"Helicopter",
            L"Applause",
            L"Gunshot"
        }, 0),
	midiBankCoarse(L"MIDI Bank Coarse", L"midiBankCoarse", 0, 0, 127),
	midiBankFine(L"MIDI Bank Fine", L"midiBankFine", 0, 0, 127),
	vibratoEnabled(L"Enable Vibrato", L"vibratoEnabled", true),
	vibratoSpeed(L"Vibrato Speed", L"vibratoSpeed", 0, 0, 8.0f),
	vibratoDepth(L"Vibrato Pitch Depth", L"vibratoDepth", 0, 0, 2.0f),
	presets(nullptr)
{
    map(&currentPresetIndex);
    map(&assignedNoteControls);
    map(&assignedPartialNoteControls);

    map(&channel);
    map(&port);
    map(&enableNoteTrigger);
    map(&enableNoteVelocity);
    map(&enableNotePanning);
    map(&enableNotePitch);
    map(&expanded);
    map(&harmonizerEnabled);
    map(&harmony);
    map(&harmonyKey);
    map(&harmonyType);
    map(&midiPatch);
    map(&midiBankCoarse);
    map(&midiBankFine);
    map(&vibratoEnabled);
    map(&vibratoSpeed);
    map(&vibratoDepth);

    map(&currentPreset);


    root = true;
}

State::~State() { cleanup(); }
void State::serialize(StreamWrapper* s)
{
	s->writeInt(versionMajor);
	s->writeInt(versionMinor);
	Serializable::serialize(s);
	s->writeInt(0);//no presets yet
}

void State::deserialize(StreamWrapper* s)
{
    cleanup(); //get rid of existing data
    versionMajor = s->readInt();
    versionMinor = s->readInt();

    Serializable::deserialize(s);

    int numPresets = s->readInt();
    presets = new Preset * [numPresets];
    for (int i = 0; i < numPresets; i++)
    {
        presets[i] = new Preset();
        presets[i]->deserialize(s);
    }
}

void State::legacy_deserialize(StreamWrapper* s)
{
	cleanup(); //get rid of existing data
	versionMajor = s->readInt();
	versionMinor = s->readInt();

	Serializable::legacy_deserialize(s);

	int numPresets = s->readInt();
	presets = new Preset * [numPresets];
	for (int i = 0; i < numPresets; i++)
	{
		presets[i] = new Preset();
		presets[i]->legacy_deserialize(s);
	}
}

void State::cleanup()
{
	if (presets != nullptr)
	{
		delete[] presets;
		presets = nullptr;
	}
}
