#include "StreamWrapper.h"
#include "fp_cplug.h"
#include "fp_def.h"
#include <vector>

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