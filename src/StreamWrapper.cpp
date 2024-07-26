#include "StreamWrapper.h"
#include "fp_cplug.h"
#include "fp_def.h"
#include <vector>
#include <cassert>

StreamWrapper::StreamWrapper(IStream* data)
{
	_stream = data;
}

void StreamWrapper::advance(int bytes)
{
	LARGE_INTEGER li;
	li.QuadPart = bytes;
	_stream->Seek(li, STREAM_SEEK_CUR, 0);
}

int StreamWrapper::getPosition()
{
	LARGE_INTEGER li;
	li.QuadPart = 0;

	ULARGE_INTEGER pos;
	_stream->Seek(li, STREAM_SEEK_CUR, &pos);
	return (int)pos.QuadPart;
}

void StreamWrapper::setPosition(int bytes)
{
	LARGE_INTEGER li;
	li.QuadPart = bytes;
	_stream->Seek(li, STREAM_SEEK_SET, 0);
}

char StreamWrapper::readChar()
{
	char ret = 0;
	if (_stream->Read(&ret, sizeof(char), 0) != S_OK)
		return CHAR_MAX;

	return ret;
}

void StreamWrapper::writeChar(char value)
{
	_stream->Write(&value, sizeof(char), 0);
}

bool StreamWrapper::readBool()
{
	bool ret = 0;
	if (_stream->Read(&ret, 1, 0) != S_OK)
		return false;

	return ret;
}
void StreamWrapper::writeBool(bool value)
{
	_stream->Write(&value, 1, 0);
}

unsigned short StreamWrapper::readUShort()
{
	unsigned short ret = 0;
	if (_stream->Read(&ret, sizeof(unsigned short), 0) != S_OK)
		return USHRT_MAX;

	return ret;
}

void StreamWrapper::writeUShort(unsigned short value)
{
	_stream->Write(&value, sizeof(unsigned short), 0);
}


int StreamWrapper::readInt()
{
	int ret = 0;
	if (_stream->Read(&ret, sizeof(int), 0) != S_OK)
		return INT32_MAX;

	return ret;
}

void StreamWrapper::writeInt(int value)
{
	_stream->Write(&value, sizeof(int), 0);
}

unsigned int StreamWrapper::readUInt()
{
	unsigned int ret = 0;
	if (_stream->Read(&ret, sizeof(unsigned int), 0) != S_OK)
		return UINT32_MAX;

	return ret;
}

void StreamWrapper::writeUInt(unsigned int value)
{
	_stream->Write(&value, sizeof(unsigned int), 0);
}

float StreamWrapper::readFloat()
{
	float ret = 0;
	if (_stream->Read(&ret, sizeof(float), 0) != S_OK)
		return FLT_MAX;

	return ret;
}
void StreamWrapper::writeFloat(float value)
{
	_stream->Write(&value, sizeof(float), 0);
}

std::wstring StreamWrapper::readString()
{
	int length = readInt();
	wchar_t* buffer = new wchar_t[length + 1];
	_stream->Read(buffer, sizeof(wchar_t) * length, 0);
	buffer[length] = L'\0';

	std::wstring ret = buffer;
	delete[] buffer;
	return ret;
}

void StreamWrapper::writeString(std::wstring s)
{
	writeInt(s.length());
	_stream->Write(s.c_str(), sizeof(wchar_t) * s.length(), 0);
}

DataStream::DataStream(const void* data, int size)
{
	_data = new char[size];
	memcpy(_data, data, size);
	_position = 0;
	_size = size;
}

DataStream::DataStream(int startingSize)
	:_position(0)
{
	_data = new char[startingSize];
	memset(_data, 0, startingSize);
	_size = startingSize;
}

DataStream::~DataStream()
{
	if (_data != nullptr)
	{
		delete[] _data;
		_data = nullptr;
	}
}

void DataStream::advance(int bytes)
{
	_position += bytes;
	assert(_position >= 0 && _position <= _size);
}

int DataStream::getPosition()
{
	return _position;
}

void DataStream::setPosition(int bytes)
{
	_position = bytes;
	if (_position < 0)
		_position = 0;
	if (_position > _size)
		_position = _size;
}

char DataStream::readChar()
{
	assert(_position + sizeof(char) <= _size);
	char c = _data[_position];
	_position++;
	return c;
}

void DataStream::writeChar(char value)
{
	ensure(sizeof(value));
	_data[_position] = value;
	_position += sizeof(value);
}

bool DataStream::readBool()
{
	assert(_position + sizeof(bool) <= _size);
	bool* c = (bool*)(&_data[_position]);
	_position += sizeof(bool);
	return *c;
}
void DataStream::writeBool(bool value)
{
	ensure(sizeof(value));
	*((bool*)(_data + _position)) = value;
	_position += sizeof(value);
}

unsigned short DataStream::readUShort()
{
	assert(_position + sizeof(unsigned short) <= _size);
	unsigned short* c = (unsigned short*)(&_data[_position]);
	_position += sizeof(unsigned short);
	return *c;
}

void DataStream::writeUShort(unsigned short value)
{
	ensure(sizeof(value));
	*((unsigned short*)(_data + _position)) = value;
	_position += sizeof(value);
}


int DataStream::readInt()
{
	assert(_position + sizeof(int) <= _size);
	int* c = (int*)(&_data[_position]);
	_position += sizeof(int);
	return *c;
}

void DataStream::writeInt(int value)
{
	ensure(sizeof(value));
	*((int*)(_data + _position)) = value;
	_position += sizeof(value);
}

unsigned int DataStream::readUInt()
{
	assert(_position + sizeof(unsigned int) <= _size);
	unsigned int* c = (unsigned int*)(&_data[_position]);
	_position += sizeof(unsigned int);
	return *c;
}

void DataStream::writeUInt(unsigned int value)
{
	ensure(sizeof(value));
	*((unsigned int*)(_data + _position)) = value;
	_position += sizeof(value);
}

float DataStream::readFloat()
{
	assert(_position + sizeof(float) <= _size);
	float* c = (float*)(&_data[_position]);
	_position += sizeof(float);
	return *c;
}

void DataStream::writeFloat(float value)
{
	ensure(sizeof(value));
	*((float*)(_data + _position)) = value;
	_position += sizeof(value);
}

std::wstring DataStream::readString()
{
	assert(_position + sizeof(int) < _size);
	int length = readInt();
	assert(_position + sizeof(wchar_t) * length < _size);

	wchar_t* buffer = new wchar_t[length + 1];
	memcpy(buffer, _data + _position, sizeof(wchar_t) * length);
	buffer[length] = L'\0';
	_position += sizeof(wchar_t) * length;

	std::wstring ret = buffer;
	delete[] buffer;
	return ret;
}

void DataStream::writeString(std::wstring s)
{
	writeInt(s.length());
	ensure(sizeof(wchar_t) * s.length());
	memcpy(_data + _position, s.c_str(), sizeof(wchar_t) * s.length());
	_position += sizeof(wchar_t) * s.length();
}

void DataStream::ensure(int additionalSize)
{
	if (_position + additionalSize >= _size)
	{
		int newSize = _size * 2;
		char* resize = new char[newSize];
		memset(resize, 0, newSize);
		memcpy(resize, _data, _size);
		delete[] _data;
		_data = resize;
		_size = newSize;
	}
}