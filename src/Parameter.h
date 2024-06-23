#pragma once
#include "Serializable.h"
#include <vector>
struct PlugParameter : Serializable
{
	PlugParameter(std::wstring name, std::wstring id);
	virtual void setFromString(std::wstring value) = 0;
	virtual std::wstring toString() = 0;
	virtual std::wstring toReadable() { return toString(); }

	virtual float getFloat() { return 0.0f; };
	virtual void setFloat(float val) { };

	float getNormalized() { return (getFloat() - getMin()) / (float)(getMax() - getMin()); };
	void setNormalized(float value) { setFloat(getMin() + (value * (getMax() - getMin()))); };

	virtual float getInc() { return 1; };
	virtual float getMin() { return 0; }
	virtual float getMax() { return 1; }

	bool canSetRange;
};

struct ParameterLink : PlugParameter
{
	ParameterLink(std::wstring name, std::wstring id);

	virtual void setFromString(std::wstring value) { value = _wtoi(value.c_str()); }
	virtual std::wstring toString() { return std::to_wstring(index); }

	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);
	virtual void legacy_deserialize(StreamWrapper* s);

	virtual float getFloat() override { return this->index; };
	virtual void setFloat(float value) override { this->index = value; };

	unsigned short index;
};

template <typename T>
struct ParameterList : PlugParameter
{
	ParameterList(std::wstring name, std::wstring id);
	virtual void setFromString(std::wstring value);
	virtual std::wstring toString();
	T** list;
protected:
	int _count;
};

struct ParameterLinkList : ParameterList<ParameterLink>
{
	ParameterLinkList(std::wstring name, std::wstring id, int num);
	~ParameterLinkList();
};

struct ParameterFloat : PlugParameter
{
	ParameterFloat(std::wstring name, std::wstring id, float value, float min, float max);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);
	virtual void legacy_deserialize(StreamWrapper* s);
	virtual void setFromString(std::wstring value) 
	{
		value = _wtof(value.c_str()); 
		if (this->value > this->_max)
			this->value = this->_max;
		if (this->value < this->_min)
			this->value = this->_min;
	}
	virtual std::wstring toString() { return std::to_wstring(value); }

	virtual float getFloat() override { return this->value; };
	virtual void setFloat(float value) override { this->value = value; };
	virtual float getInc() override { return 0.1f; };

	virtual float getMin() override { return _min; }
	int setMin(int val)
	{
		if (_min != val)
			_minMaxSet = true;

		_min = val;
	}

	virtual float getMax() override { return _max; }
	int setMax(int val)
	{
		if (_max != val)
			_minMaxSet = true;

		_max = val;
	}

	float value;
private:
	bool _minMaxSet;
	float _min;
	float _max;

};

struct ParameterInt : PlugParameter
{
	ParameterInt(std::wstring name, std::wstring id, int value, int min, int max);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);
	virtual void legacy_deserialize(StreamWrapper* s);

	virtual float getFloat() override { return this->value; };
	virtual void setFloat(float value) override { this->value = value; };

	virtual void setFromString(std::wstring value) 
	{
		this->value = _wtoi(value.c_str());
		if (this->value > this->_max)
			this->value = this->_max;

		if (this->value < this->_min)
			this->value = this->_min;
	}
	virtual std::wstring toString() { return std::to_wstring(value); }

	int value;

	virtual float getMin() override { return _min; }
	int setMin(int val) 
	{
		if (_min != val)
			_minMaxSet = true;

		_min = val;
	}	
	
	virtual float getMax() override { return _max; }
	int setMax(int val)
	{
		if (_max != val)
			_minMaxSet = true;

		_max = val;
	}

private:
	bool _minMaxSet;
	int _min;
	int _max;
};

struct ParameterIntList : ParameterList<ParameterInt>
{
	ParameterIntList(std::wstring name, std::wstring id, int num, int value, int min, int max);
	~ParameterIntList();
};

struct ParameterMidiCC : ParameterInt
{
	ParameterMidiCC(std::wstring name, std::wstring id, int value);
};

struct ParameterMidiCCList : ParameterList<ParameterMidiCC>
{
	ParameterMidiCCList(std::wstring name, std::wstring id, int num, int value);
	~ParameterMidiCCList();
};

struct ParameterString : PlugParameter
{
	ParameterString(std::wstring name, std::wstring id, std::wstring value);
	virtual void setFromString(std::wstring value) { this->value = value; }
	virtual std::wstring toString() { return value; }

	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);
	virtual void legacy_deserialize(StreamWrapper* s);
	std::wstring value;
};

struct ParameterStringList : ParameterList<ParameterString>
{
	ParameterStringList(std::wstring name, std::wstring id, int num, std::wstring value);
	~ParameterStringList();
};

struct ParameterBool : PlugParameter
{
	ParameterBool(std::wstring name, std::wstring id, bool value);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);
	virtual void legacy_deserialize(StreamWrapper* s);

	virtual void setFromString(std::wstring value) 
	{
		if (value == L"1" || value == L"true" || value == L"t")
			this->value = true;
		else
			this->value = false;
	}
	virtual std::wstring toString() { return value ? L"1" : L"0"; }

	virtual float getFloat() override { return this->value ? 1.0f : 0.0f; };
	virtual void setFloat(float value) override { this->value = value > 0.5f ? true : false; };

	bool value;
};

struct ParameterBoolList : ParameterList<ParameterBool>
{
	ParameterBoolList(std::wstring name, std::wstring id, int num, bool value);
	~ParameterBoolList();
};

struct ParameterStringSelection : ParameterInt
{
	ParameterStringSelection(std::wstring name, std::wstring id, std::vector<std::wstring> strings, int value);
	virtual std::wstring toReadable() override { return toReadable(value, true); }
	std::wstring toReadable(int index, bool shorten);

	std::vector<std::wstring> strings;
};