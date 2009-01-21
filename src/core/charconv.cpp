#include "charconv.h"
#include "debug.h"

void CharConv::toBuffer(std::string s, unsigned int size)
{
	char * data = new char[size];
	memset(data,0,size);
	memcpy(data,s.data(), std::min(size,s.size()));
	toBuffer(data, size);
	delete data;
}

void CharConv::fromBuffer(std::string& s, unsigned int size)
{
	char* data = new char[size];
	fromBuffer(data,size);
	s.assign(data,size);
	delete data;
}

void CharConv::toBuffer(std::string s)
{
	toBuffer<int>(s.size());
	toBuffer(s.data(), s.size());
}

void CharConv::fromBuffer(std::string& s)
{
	int len;
	fromBuffer<int>(len);
	char * data = new char[len];
	fromBuffer(data,len);
	s.assign(data,len);
	delete data;
}

