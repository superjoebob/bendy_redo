#include "Parameter.h"
#include "StreamWrapper.h"
Parameter::Parameter(std::wstring name, std::wstring id):
	Serializable(name, id),
	canSetRange(false)
{

}

ParameterLink::ParameterLink(std::wstring name, std::wstring id) :
	Parameter(name, id),
	index(0)
{
}

void ParameterLink::serialize(StreamWrapper* s)
{
	s->writeUShort(index);
	Serializable::serialize(s);
}

void ParameterLink::deserialize(StreamWrapper* s)
{
	index = s->readUShort();
	Serializable::deserialize(s);
}


ParameterLinkList::ParameterLinkList(std::wstring name, std::wstring id, int num) :
	Parameter(name, id)
{
	list = new ParameterLink * [num];
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		list[i] = new ParameterLink(name, subID);
		map(list[i]);
	}
}

ParameterLinkList::~ParameterLinkList()
{
	delete[] list;
}

ParameterFloat::ParameterFloat(std::wstring name, std::wstring id, float value, float min, float max) :
	Parameter(name, id),
	value(value),
	min(min),
	max(max)
{
	canSetRange = true;
}

void ParameterFloat::serialize(StreamWrapper* s)
{
	Serializable::serialize(s);
}

void ParameterFloat::deserialize(StreamWrapper* s)
{
	bool minMaxSet = s->readBool();
	min = minMaxSet ? s->readFloat() : 0;
	max = minMaxSet ? s->readFloat() : 0;
	value = s->readFloat();

	Serializable::deserialize(s);
}


ParameterInt::ParameterInt(std::wstring name, std::wstring id, int value, int min, int max):
	Parameter(name, id),
	value(value),
	min(min),
	max(max)
{
	canSetRange = true;
}

void ParameterInt::serialize(StreamWrapper* s)
{
	Serializable::serialize(s);
}

void ParameterInt::deserialize(StreamWrapper* s)
{
	bool minMaxSet = s->readBool();
	if (minMaxSet)
	{
		min = s->readInt();
		max = s->readInt();
	}
	value = s->readInt();

	Serializable::deserialize(s);
}

ParameterIntList::ParameterIntList(std::wstring name, std::wstring id, int num, int value, int min, int max) :
	Parameter(name, id)
{
	list = new ParameterInt*[num];
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		list[i] = new ParameterInt(name, subID, value, min, max);
		map(list[i]);
	}
}
ParameterIntList::~ParameterIntList()
{
	delete[] list;
}

ParameterMidiCC::ParameterMidiCC(std::wstring name, std::wstring id, int value) :
	ParameterInt(name, id, value, 0, 127)
{
	canSetRange = false;
}

ParameterMidiCCList::ParameterMidiCCList(std::wstring name, std::wstring id, int num, int value) :
	Parameter(name, id)
{
	list = new ParameterMidiCC*[num];
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		list[i] = new ParameterMidiCC(name, subID, value);
		map(list[i]);
	}
}
ParameterMidiCCList::~ParameterMidiCCList()
{
	delete[] list;
}

ParameterString::ParameterString(std::wstring name, std::wstring id, std::wstring value) :
	Parameter(name, id)
{
	this->value = value;
}

void ParameterString::serialize(StreamWrapper* s)
{
	s->writeString(value);
	Serializable::serialize(s);
}

void ParameterString::deserialize(StreamWrapper* s)
{
	value = s->readString();
	Serializable::deserialize(s);
}

ParameterStringList::ParameterStringList(std::wstring name, std::wstring id, int num, std::wstring value) :
	Parameter(name, id)
{
	list = new ParameterString *[num];
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		list[i] = new ParameterString(name, subID, value);
		map(list[i]);
	}
}
ParameterStringList::~ParameterStringList()
{
	delete[] list;
}

ParameterBool::ParameterBool(std::wstring name, std::wstring id, bool value) :
	Parameter(name, id)
{
	this->value = value;
}

void ParameterBool::serialize(StreamWrapper* s)
{
	s->writeBool(value);
	Serializable::serialize(s);
}

void ParameterBool::deserialize(StreamWrapper* s)
{
	value = s->readBool();
	Serializable::deserialize(s);
}

ParameterBoolList::ParameterBoolList(std::wstring name, std::wstring id, int num, bool value) :
	Parameter(name, id)
{
	list = new ParameterBool * [num];
	for (int i = 0; i < num; i++)
	{
		std::wstring subID = id + std::to_wstring(i);
		list[i] = new ParameterBool(name, subID, value);
		map(list[i]);
	}
}
ParameterBoolList::~ParameterBoolList()
{
	delete[] list;
}

ParameterStringSelection::ParameterStringSelection(std::wstring name, std::wstring id, std::vector<std::wstring> strings, int value) :
	ParameterInt(name, id, value, 0, strings.size() - 1)
{
	canSetRange = false;
}