#include "Parameter.h"
#include "StreamWrapper.h"

bool MidiCluster::shouldTrigger()
{
	return (enabled == nullptr || enabled->value > 0) && cc != nullptr && value != nullptr;
}

PlugParameter::PlugParameter(std::wstring name, std::wstring id):
	Serializable(name, id),
	canSetRange(false),
	midiTrigger(MidiTrigger::None),
	refreshCluster(false)
{

}

std::wstring PlugParameter::getName()
{
	if (midi.name != nullptr && midi.name->value != L"")
		return midi.name->value;
	return name;
}


ParameterLink::ParameterLink(std::wstring name, std::wstring id) :
	PlugParameter(name, id),
	linkHash(0),
	previousWrite(FLT_MIN)
{
}

void ParameterLink::serialize(Stream* s)
{
	s->writeUInt(linkHash);
	Serializable::serialize(s);
}

void ParameterLink::deserialize(Stream* s)
{
	linkHash = s->readUInt();
	Serializable::deserialize(s);
}

void ParameterLink::legacy_deserialize(Stream* s)
{
	linkHash = s->readUShort();
	Serializable::legacy_deserialize(s);
}


void split(std::wstring str, std::wstring splitBy, std::vector<std::wstring>& tokens)
{
	/* Store the original string in the array, so we can loop the rest
	 * of the algorithm. */
	tokens.push_back(str);

	// Store the split index in a 'size_t' (unsigned integer) type.
	size_t splitAt;
	// Store the size of what we're splicing out.
	size_t splitLen = splitBy.size();
	// Create a string for temporarily storing the fragment we're processing.
	std::wstring frag;
	// Loop infinitely - break is internal.
	while (true)
	{
		/* Store the last string in the vector, which is the only logical
		 * candidate for processing. */
		frag = tokens.back();
		/* The index where the split is. */
		splitAt = frag.find(splitBy);
		// If we didn't find a new split point...
		if (splitAt == std::string::npos)
		{
			// Break the loop and (implicitly) return.
			break;
		}
		/* Put everything from the left side of the split where the string
		 * being processed used to be. */
		tokens.back() = frag.substr(0, splitAt);
		/* Push everything from the right side of the split to the next empty
		 * index in the vector. */
		tokens.push_back(frag.substr(splitAt + splitLen, frag.size() - (splitAt + splitLen)));
	}
}


template<typename T> void ParameterList<T>::setFromString(std::wstring value)
{
	std::vector<std::wstring> tokens;
	split(value, L",", tokens);

	int idx = 0;
	for (auto it = tokens.begin(); it != tokens.end(); it++)
	{
		if (idx < _count)
			((PlugParameter*)&_list[idx])->setFromString(*it);
	}
}
template<typename T> std::wstring ParameterList<T>::toString()
{
	std::wstring ret = L"";
	for (int i = 0; i < _count; i++)
	{
		ret += ((PlugParameter*)&_list[i])->toString();
		if (i != _count - 1)
			ret += L",";
	}

	return ret;
}


ParameterLinkList::ParameterLinkList(std::wstring name, std::wstring id, int num) :
	ParameterList<ParameterLink>(name, id)
{
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		_list.push_back(ParameterLink(name, subID));
	}

	_count = num;
	initialize_map();
}

ParameterFloat::ParameterFloat(std::wstring name, std::wstring id, float value, float min, float max) :
	PlugParameter(name, id),
	value(value),
	_min(min),
	_max(max),
	_minMaxSet(false)
{
	canSetRange = true;
}

void ParameterFloat::serialize(Stream* s)
{
	if (_minMaxSet)
	{
		s->writeBool(true);
		s->writeFloat(_min);
		s->writeFloat(_max);
	}
	else
		s->writeBool(false);

	s->writeFloat(value);

	Serializable::serialize(s);
}

void ParameterFloat::deserialize(Stream* s)
{
	_minMaxSet = s->readBool();
	if (_minMaxSet)
	{
		_min = s->readFloat();
		_max = s->readFloat();
	}
	value = s->readFloat();

	Serializable::deserialize(s);
}

void ParameterFloat::legacy_deserialize(Stream* s)
{
	_minMaxSet = s->readBool();
	if (_minMaxSet)
	{
		_min = s->readFloat();
		_max = s->readFloat();
	}
	value = s->readFloat();

	Serializable::legacy_deserialize(s);
}


ParameterInt::ParameterInt(std::wstring name, std::wstring id, int value, int min, int max):
	PlugParameter(name, id),
	value(value),
	_min(min),
	_max(max),
	_minMaxSet(false)
{
	canSetRange = true;
}

void ParameterInt::serialize(Stream* s)
{
	if (_minMaxSet)
	{
		s->writeBool(true);
		s->writeInt(_min);
		s->writeInt(_max);
	}
	else
		s->writeBool(false);

	s->writeInt(value);

	Serializable::serialize(s);
}

void ParameterInt::deserialize(Stream* s)
{
	_minMaxSet = s->readBool();
	if (_minMaxSet)
	{
		_min = s->readInt();
		_max = s->readInt();
	}
	value = s->readInt();

	Serializable::deserialize(s);
}

void ParameterInt::legacy_deserialize(Stream* s)
{
	_minMaxSet = s->readBool();
	if (_minMaxSet)
	{
		_min = s->readInt();
		_max = s->readInt();
	}
	value = s->readInt();

	Serializable::legacy_deserialize(s);
}

void ParameterInt::setValue(int v)
{
	if (v != value && midi.enabled != nullptr && midi.enabled->value == -1)
	{
		midi.enabled->setFloat(1);
		refreshCluster = true;
	}

	value = v;
}

template<typename T> ParameterList<T>::ParameterList(std::wstring name, std::wstring id)
	:PlugParameter(name, id)
{

}

ParameterIntList::ParameterIntList(std::wstring name, std::wstring id, int num, int value, int min, int max) :
	ParameterList<ParameterInt>(name, id)
{
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		_list.push_back(ParameterInt(name, subID, value, min, max));
	}	
	_count = num;
	initialize_map();
}

ParameterMidiCC::ParameterMidiCC(std::wstring name, std::wstring id, int value) :
	ParameterInt(name, id, value, 0, 127)
{
	canSetRange = false;
}

ParameterMidiCCList::ParameterMidiCCList(std::wstring name, std::wstring id, int num, int value) :
	ParameterList<ParameterMidiCC>(name, id)
{
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		_list.push_back(ParameterMidiCC(name, subID, value));
	}

	_count = num;
	initialize_map();
}


ParameterString::ParameterString(std::wstring name, std::wstring id, std::wstring value) :
	PlugParameter(name, id)
{
	this->value = value;
}

void ParameterString::serialize(Stream* s)
{
	s->writeString(value);
	Serializable::serialize(s);
}

void ParameterString::deserialize(Stream* s)
{
	value = s->readString();
	Serializable::deserialize(s);
}
void ParameterString::legacy_deserialize(Stream* s)
{
	value = s->readString();
	Serializable::legacy_deserialize(s);
}

ParameterStringList::ParameterStringList(std::wstring name, std::wstring id, int num, std::wstring value) :
	ParameterList<ParameterString>(name, id)
{
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		_list.push_back(ParameterString(name, subID, value));
	}

	_count = num;
	initialize_map();
}

ParameterBool::ParameterBool(std::wstring name, std::wstring id, int value) :
	PlugParameter(name, id)
{
	this->value = value;
}

void ParameterBool::serialize(Stream* s)
{
	s->writeChar((char)value);
	Serializable::serialize(s);
}

void ParameterBool::deserialize(Stream* s)
{
	value = s->readChar();
	Serializable::deserialize(s);
}

void ParameterBool::legacy_deserialize(Stream* s)
{
	value = s->readBool();
	Serializable::legacy_deserialize(s);
}

ParameterBoolList::ParameterBoolList(std::wstring name, std::wstring id, int num, int value) :
	ParameterList<ParameterBool>(name, id)
{
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		_list.push_back(ParameterBool(name, subID, value));
	}

	_count = num;
	initialize_map();
}
ParameterStringSelection::ParameterStringSelection(std::wstring name, std::wstring id, std::vector<std::wstring> strings, int value) :
	ParameterInt(name, id, value, 0, strings.size() - 1)
{
	this->strings = strings;
	canSetRange = false;
}

std::wstring ParameterStringSelection::toReadable(int index, bool shorten)
{
	if (index < strings.size())
	{
		std::wstring s = strings[index];
		std::vector<std::wstring> tokens;
		split(s, L"|", tokens);

		if (tokens.size() == 1)
			return s;
		else
			return shorten ? tokens[1] : tokens[0];
	}

	return L"null";
}

