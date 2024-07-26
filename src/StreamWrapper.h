#pragma once
#include <string>
class IStream;

class Stream
{
public:
	virtual void advance(int bytes) = 0;
	virtual int getPosition() = 0;
	virtual void setPosition(int bytes) = 0;

	virtual char readChar() = 0;
	virtual void writeChar(char value) = 0;

	virtual bool readBool() = 0;
	virtual void writeBool(bool value) = 0;

	virtual unsigned short readUShort() = 0;
	virtual void writeUShort(unsigned short value) = 0;

	virtual int readInt() = 0;
	virtual void writeInt(int value) = 0;

	virtual unsigned int readUInt() = 0;
	virtual void writeUInt(unsigned int value) = 0;

	virtual float readFloat() = 0;
	virtual void writeFloat(float value) = 0;

	virtual std::wstring readString() = 0;
	virtual void writeString(std::wstring s) = 0;
};



class StreamWrapper : public Stream
{
public:
	StreamWrapper(IStream* data);

	void advance(int bytes) override;
	int getPosition() override;
	void setPosition(int bytes) override;

	char readChar() override;
	void writeChar(char value) override;

	bool readBool() override;
	void writeBool(bool value) override;

	unsigned short readUShort() override;
	void writeUShort(unsigned short value) override;

	int readInt() override;
	void writeInt(int value) override;

	unsigned int readUInt() override;
	void writeUInt(unsigned int value) override;

	float readFloat() override;
	void writeFloat(float value) override;

	std::wstring readString() override;
	void writeString(std::wstring s) override;
protected:
	IStream* _stream;
};

class DataStream : public Stream
{
public:
	DataStream(const void* data, int size);
	DataStream(int startingSize = 256);
	~DataStream();

	void advance(int bytes) override;
	int getPosition() override;
	void setPosition(int bytes) override;

	char readChar() override;
	void writeChar(char value) override;

	bool readBool() override;
	void writeBool(bool value) override;

	unsigned short readUShort() override;
	void writeUShort(unsigned short value) override;

	int readInt() override;
	void writeInt(int value) override;

	unsigned int readUInt() override;
	void writeUInt(unsigned int value) override;

	float readFloat() override;
	void writeFloat(float value) override;

	std::wstring readString() override;
	void writeString(std::wstring s) override;


protected:
	char* _data;
	int _position;
	int _size;
	void ensure(int additionalSize);
};