#pragma once
#include <string>
#include <map>
#include <vector>
class StreamWrapper;
struct Serializable 
{
	Serializable(std::wstring readableName, std::wstring id);
	virtual void serialize(StreamWrapper* s);
	virtual void deserialize(StreamWrapper* s);
	void map(Serializable* child);

	virtual float getFloat() { return 0.0f; };
	virtual void setFloat(float value) { };

	std::wstring name;
	std::wstring id;
	unsigned int hash;
	bool root;

	std::vector<Serializable*> children;
	std::map<unsigned int, Serializable*> hashmap;
	std::map<std::wstring, Serializable*> namemap;
	std::vector<Serializable*> all;
};