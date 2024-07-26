#pragma once
#include <string>
#include <map>
#include <vector>
class Stream;
struct Serializable 
{
	Serializable(std::wstring readableName, std::wstring id);
	virtual void serialize(Stream* s);
	virtual void deserialize(Stream* s);
	virtual void legacy_deserialize(Stream* s);
	void map(Serializable* child);

	std::wstring name;
	std::wstring id;
	unsigned int hash;
	bool root;

	unsigned short legacyFixedIndex;
	int index;

	std::vector<Serializable*> children;
	std::map<unsigned int, Serializable*> hashmap;
	std::map<std::wstring, Serializable*> namemap;
	std::map<unsigned int, int> indexmap;
	std::vector<Serializable*> all;

protected:
	virtual void serialize_children(Stream* s);
	void assign_indexes();
};