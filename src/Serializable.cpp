#include "Serializable.h"
#include "StreamWrapper.h"
#include "CRC32.h"
#include <locale>
#include <codecvt>
Serializable::Serializable(std::wstring readableName, std::wstring id) :
	name(readableName),
	id(id),
	root(false)
{
	//setup converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted_str = converter.to_bytes(id);

	hash = crc32buf((char*)converted_str.c_str(), sizeof(char) * id.length());
}

void Serializable::serialize(StreamWrapper* s)
{
	s->writeUInt(0); //ignore children for now
}

void Serializable::deserialize(StreamWrapper* s)
{
	unsigned int childID = s->readUInt();
	while (childID != 0)
	{
		unsigned int paramSize = s->readUInt();
		auto it = hashmap.find(childID);
		if(it == hashmap.end())
			s->advance(paramSize); //could not find child
		else
		{
			unsigned short fixedIndex = 0;
			if (root)
				fixedIndex = s->readUShort(); //unused??

			(*it).second->root = root;
			(*it).second->deserialize(s);
		}

		childID = s->readUInt();
	}
}

void Serializable::map(Serializable* child)
{
	all.push_back(child);
	children.push_back(child);
	hashmap[child->hash] = child;
	namemap[child->id] = child;
	for (auto it = child->all.begin(); it != child->all.end(); it++)
	{
		namemap[(*it)->id] = (*it);
		hashmap[(*it)->hash] = (*it);
		all.push_back((*it));
	}	
}
