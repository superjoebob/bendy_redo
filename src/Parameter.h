#pragma once
#include "Serializable.h"
#include <vector>

enum class MidiTrigger
{
	None = 0,
	BankChange = 1,
	PortChange = 2,
	MidiParameterChange = 4,
	BendRangeChange = 8
}; 

inline MidiTrigger operator|(MidiTrigger a, MidiTrigger b)
{
	return static_cast<MidiTrigger>(static_cast<int>(a) | static_cast<int>(b));
}

inline MidiTrigger operator&(MidiTrigger a, MidiTrigger b)
{
	return static_cast<MidiTrigger>(static_cast<int>(a) & static_cast<int>(b));
}

struct ParameterInt;
struct ParameterMidiCC;
struct ParameterString;
struct ParameterBool;
struct MidiCluster
{
	MidiCluster()
		:value(nullptr),
		cc(nullptr),
		name(nullptr),
		enabled(nullptr)
	{

	}
	ParameterInt* value;
	ParameterMidiCC* cc;
	ParameterString* name;
	ParameterBool* enabled;
	bool shouldTrigger();
};

struct PlugParameter : Serializable
{
	PlugParameter(std::wstring name, std::wstring id);
	virtual void setFromString(std::wstring value) = 0;
	virtual std::wstring toString() = 0;
	virtual std::wstring toReadable() { return toString(); }

	virtual float getFloat() { return 0.0f; };
	virtual void setFloat(float val) { };

	virtual void* get4ByteValue() { return &_useless4ByteValue; };
	virtual void set4ByteValue(void* val) {  };


	/// <summary>
	/// Converts a normalized value to be within the range of this paramter's values
	/// </summary>
	/// <param name="value">A value between 0.0f and 1.0f</param>
	/// <returns>That value expanded to be between getMin() and getMax()</returns>
	float normalizedToMidiRange(float value) { return getMidiMin() + (value * (getMidiMax() - getMidiMin())); };
	float normalizedToRange(float value) { return getMin() + (value * (getMax() - getMin())); };
	float rangeToNormalized(float value) { return (value - getMin()) / (float)(getMax() - getMin()); };
	float getNormalized() { return (getFloat() - getMin()) / (float)(getMax() - getMin()); }


	virtual float getInc() { return 1; };
	virtual float getMin() { return 0; }
	virtual float getMax() { return 1; }

	virtual void setMin(float val) { ; }
	virtual void setMax(float val) { ; }

	virtual int getMidiMin() { return getMin(); }
	virtual int getMidiMax() { return getMax(); }

	virtual bool isFloatingPoint() { return false; }

	std::wstring getName();

	bool refreshCluster;
	bool canSetRange;
	MidiTrigger midiTrigger;
	MidiCluster midi;

private:
	int _useless4ByteValue = 0;
};

struct ParameterLink : PlugParameter
{
	ParameterLink(std::wstring name, std::wstring id);

	virtual void setFromString(std::wstring value) { value = _wtoi(value.c_str()); }
	virtual std::wstring toString() { return std::to_wstring(linkHash); }

	virtual void serialize(Stream* s);
	virtual void deserialize(Stream* s);
	virtual void legacy_deserialize(Stream* s);

	virtual void* get4ByteValue() override { return &this->linkHash; };
	virtual void set4ByteValue(void* val) override { linkHash = *((unsigned int*)val); };
	virtual float getFloat() override { return *((float*)(&this->linkHash)); };
	virtual void setFloat(float value) override { this->linkHash = *((unsigned int*)&value); };

	unsigned int linkHash;
	float previousWrite;
};

template <typename T>
struct ParameterList : PlugParameter
{
	ParameterList(std::wstring name, std::wstring id);
	virtual void setFromString(std::wstring value);
	virtual std::wstring toString();

	T& operator [] (int i) { return _list[i]; }

protected:
	int _count;
	std::vector<T> _list;
	void initialize_map()
	{
		//Need to do this at the end, since references taken to vector elements become invalid
		//if the list changes size (ie if things are added later)
		for (int i = 0; i < _list.size(); i++)
		{
			map(&_list[i]);
		}
	}
};

struct ParameterLinkList : ParameterList<ParameterLink>
{
	ParameterLinkList(std::wstring name, std::wstring id, int num);
};

struct ParameterFloat : PlugParameter
{
	ParameterFloat(std::wstring name, std::wstring id, float value, float min, float max);
	virtual void serialize(Stream* s);
	virtual void deserialize(Stream* s);
	virtual void legacy_deserialize(Stream* s);
	virtual void setFromString(std::wstring value) 
	{
		value = _wtof(value.c_str()); 
		if (this->value > this->_max)
			this->value = this->_max;
		if (this->value < this->_min)
			this->value = this->_min;
	}
	virtual std::wstring toString() { return std::to_wstring(value); }

	virtual bool isFloatingPoint() override { return true; }

	virtual void* get4ByteValue() override 
	{
		_midiValue = normalizedToMidiRange(getNormalized());
		return &_midiValue;
	};
	virtual void set4ByteValue(void* val) override { value = normalizedToRange((*((int*)val)) / (double)(1 << 30)); };
	virtual float getFloat() override { return this->value; };
	virtual void setFloat(float value) override { this->value = value; };

	virtual float getInc() override { return 0.1f; };

	virtual float getMin() override { return _min; }
	virtual void setMin(float val) override
	{
		if (_min != val)
			_minMaxSet = true;

		_min = val;

		if (value < _min)
			value = _min;
	}

	virtual float getMax() override { return _max; }
	virtual void setMax(float val) override
	{
		if (_max != val)
			_minMaxSet = true;

		_max = val;

		if (value > _max)
			value = _max;
	}

	virtual int getMidiMin() { return 0; }
	virtual int getMidiMax() { return 1 << 30; }

	float value;
private:
	bool _minMaxSet;
	float _min;
	float _max;
	int _midiValue;
};

struct ParameterInt : PlugParameter
{
	ParameterInt(std::wstring name, std::wstring id, int value, int min, int max);
	virtual void serialize(Stream* s);
	virtual void deserialize(Stream* s);
	virtual void legacy_deserialize(Stream* s);

	virtual void* get4ByteValue() override { return &value; };
	virtual void set4ByteValue(void* val) override { setValue(*((int*)val)); };
	virtual float getFloat() override { return this->value; };
	virtual void setFloat(float value) override { setValue(value); };

	virtual void setFromString(std::wstring value) 
	{
		this->value = _wtoi(value.c_str());
		if (this->value > this->_max)
			setValue(this->_max);

		if (this->value < this->_min)
			setValue(this->_min);
	}
	virtual std::wstring toString() { return std::to_wstring(value); }

	void setValue(int v);
	int value;

	virtual float getMin() override { return _min; }
	virtual void setMin(float val) override
	{
		if (_min != val)
			_minMaxSet = true;

		_min = val;

		if (value < _min)
			value = _min;
	}	
	
	virtual float getMax() override { return _max; }
	virtual void setMax(float val) override
	{
		if (_max != val)
			_minMaxSet = true;

		_max = val;

		if (value > _max)
			value = _max;
	}

private:
	bool _minMaxSet;
	int _min;
	int _max;
};

struct ParameterIntList : ParameterList<ParameterInt>
{
	ParameterIntList(std::wstring name, std::wstring id, int num, int value, int min, int max);
};

struct ParameterMidiCC : ParameterInt
{
	ParameterMidiCC(std::wstring name, std::wstring id, int value);
};

struct ParameterMidiCCList : ParameterList<ParameterMidiCC>
{
	ParameterMidiCCList(std::wstring name, std::wstring id, int num, int value);
};

struct ParameterString : PlugParameter
{
	ParameterString(std::wstring name, std::wstring id, std::wstring value);
	virtual void setFromString(std::wstring value) { this->value = value; }
	virtual std::wstring toString() { return value; }

	virtual void serialize(Stream* s);
	virtual void deserialize(Stream* s);
	virtual void legacy_deserialize(Stream* s);
	std::wstring value;
};

struct ParameterStringList : ParameterList<ParameterString>
{
	ParameterStringList(std::wstring name, std::wstring id, int num, std::wstring value);
};

struct ParameterBool : PlugParameter
{
	ParameterBool(std::wstring name, std::wstring id, int value);
	virtual void serialize(Stream* s);
	virtual void deserialize(Stream* s);
	virtual void legacy_deserialize(Stream* s);

	virtual float getMin() { return -1; }
	virtual float getMax() { return 1; }

	virtual void setFromString(std::wstring value) 
	{
		if (value == L"-1")
			this->value = -1;
		else if (value == L"1" || value == L"true" || value == L"t")
			this->value = 1;
		else
			this->value = 0;
	}
	virtual std::wstring toString() { return value > 0 ? L"1" : value < 0 ? L"-1" :  L"0"; }

	virtual void* get4ByteValue() override { return &value; };
	virtual void set4ByteValue(void* val) override { value = *((int*)val); };
	virtual float getFloat() override 
	{
		if (this->value > 0)
			return 1.0f;
		else if (this->value < 0)
			return -1.0f;

		return 0.0f; 
	};
	virtual void setFloat(float value) override 
	{
		if (value > 0)
			this->value = 1;
		else if (value < 0)
			this->value = -1;
		else
			this->value = 0; 
	};

	int value;
};

struct ParameterBoolList : ParameterList<ParameterBool>
{
	ParameterBoolList(std::wstring name, std::wstring id, int num, int value);
};

struct ParameterStringSelection : ParameterInt
{
	ParameterStringSelection(std::wstring name, std::wstring id, std::vector<std::wstring> strings, int value);
	virtual std::wstring toReadable() override { return toReadable(value, true); }
	std::wstring toReadable(int index, bool shorten);

	std::vector<std::wstring> strings;
};
