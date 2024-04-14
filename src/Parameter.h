#pragma once
#include "Serializable.h"
#include <vector>
struct Parameter : Serializable
{
	Parameter(std::wstring name, std::wstring id);
	bool canSetRange;
};
struct ParameterLink : Parameter
{
	ParameterLink(std::wstring name, std::wstring id);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);
	unsigned short index;
};

struct ParameterLinkList : Parameter
{
	ParameterLinkList(std::wstring name, std::wstring id, int num);
	~ParameterLinkList();
	ParameterLink** list;
};

struct ParameterFloat : Parameter
{
	ParameterFloat(std::wstring name, std::wstring id, float value, float min, float max);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);

	virtual float getFloat() override { return (this->value - min) / (float)(max - min); };
	virtual void setFloat(float value) override { this->value = min + (value * (max - min)); };

	float value;
	float min;
	float max;
};

struct ParameterInt : Parameter
{
	ParameterInt(std::wstring name, std::wstring id, int value, int min, int max);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);

	virtual float getFloat() override { return (this->value - min) / (float)(max - min); };
	virtual void setFloat(float value) override { this->value = min + (value * (max - min)); };

	int value;
	int min;
	int max;
};

struct ParameterIntList : Parameter
{
	ParameterIntList(std::wstring name, std::wstring id, int num, int value, int min, int max);
	~ParameterIntList();
	ParameterInt** list;
};

struct ParameterMidiCC : ParameterInt
{
	ParameterMidiCC(std::wstring name, std::wstring id, int value);
};

struct ParameterMidiCCList : Parameter
{
	ParameterMidiCCList(std::wstring name, std::wstring id, int num, int value);
	~ParameterMidiCCList();
	ParameterMidiCC** list;
};

struct ParameterString : Parameter
{
	ParameterString(std::wstring name, std::wstring id, std::wstring value);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);
	std::wstring value;
};

struct ParameterStringList : Parameter
{
	ParameterStringList(std::wstring name, std::wstring id, int num, std::wstring value);
	~ParameterStringList();
	ParameterString** list;
};

struct ParameterBool : Parameter
{
	ParameterBool(std::wstring name, std::wstring id, bool value);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);

	virtual float getFloat() override { return this->value ? 1.0f : 0.0f; };
	virtual void setFloat(float value) override { this->value = (value > 0.5f) ? true : false; };

	bool value;
};

struct ParameterBoolList : Parameter
{
	ParameterBoolList(std::wstring name, std::wstring id, int num, bool value);
	~ParameterBoolList();
	ParameterBool** list;
};

struct ParameterStringSelection : ParameterInt
{
	ParameterStringSelection(std::wstring name, std::wstring id, std::vector<std::wstring> strings, int value);
	std::vector<std::wstring> strings;
};