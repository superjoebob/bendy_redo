#pragma once
#include <string>
#include <map>
#include <vector>

enum Key
{
	C = 0,
	Cs,
	D
	,Ds,
	E,
	F,
	Fs,
	G,
	Gs,
	A,
	As,
	B
};

class Scale
{
public:
	Scale(std::wstring name, std::vector<int> formula);
	Key Harmonize(Key key, Key myKey, int harmony);
	std::wstring name;

private:
	Key MapKey(Key pKey, int f);
	std::vector<Key> _keys;
	std::map<Key, int> _keyIndex;
};

