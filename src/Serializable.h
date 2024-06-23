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
	virtual void legacy_deserialize(StreamWrapper* s);
	void map(Serializable* child);

	std::wstring name;
	std::wstring id;
	unsigned int hash;
	bool root;

	std::vector<Serializable*> children;
	std::map<unsigned int, Serializable*> hashmap;
	std::map<std::wstring, Serializable*> namemap;
	std::vector<Serializable*> all;

protected:
	virtual void serialize_children(StreamWrapper* s);
};