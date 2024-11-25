#include "Scale.h"


Scale::Scale(std::wstring name, std::vector<int> formula)
	:name(name)
{

    MapKey(Key::C, 0);
    Key lastKey = Key::C;
    for(int i = 0; i < formula.size(); i++)
    {
        lastKey = MapKey(lastKey, formula[i]);
    }
}

Key Scale::MapKey(Key pKey, int f)
{
    Key key = (Key)((pKey + f) % 12);
    _keyIndex[key] = _keys.size();
    _keys.push_back(key);
    return key;
}

Key Scale::Harmonize(Key key, Key myKey, int harmony)
{
    Key keyOffsetFromC = (Key)(((int)myKey - (int)key) % 12);
    if (keyOffsetFromC < 0)
        keyOffsetFromC = (Key)(keyOffsetFromC + 12);

    int keyIndex = 0;

    auto it = _keyIndex.find(keyOffsetFromC);
    if (it == _keyIndex.end())
        return key; //No harmony here!

    keyIndex = (*it).second;

    int newKeyIndex = ((keyIndex + harmony) % _keys.size());
    if (newKeyIndex < 0)
        newKeyIndex += _keys.size();

    Key harmonizedKey = (Key)(((int)_keys[newKeyIndex] + (int)key) % 12);
    return harmonizedKey;
}