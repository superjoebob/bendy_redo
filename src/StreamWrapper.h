#pragma once
#include <string>
class IStream;
class StreamWrapper 
{
public:
	StreamWrapper(IStream* data);

	void advance(int bytes);
	int getPosition();
	void setPosition(int bytes);

	char readChar();
	void writeChar(char value);

	bool readBool();
	void writeBool(bool value);

	unsigned short readUShort();
	void writeUShort(unsigned short value);

	int readInt();
	void writeInt(int value);

	unsigned int readUInt();
	void writeUInt(unsigned int value);

	float readFloat();
	void writeFloat(float value);

	std::wstring readString();
	void writeString(std::wstring s);
protected:
	IStream* _stream;
};