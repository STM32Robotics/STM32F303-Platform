#ifndef STRINGCLASS_H_
#define STRINGCLASS_H_

#include "ctype.h"
#include "string.h"

#define BufferSize 128
class TString
{
private:
	unsigned int StrSize = 0;
	char Buffer[BufferSize];
	char ErrorBuffer[1];
	void ResetBuffer();
public:
	TString(const char* str);
	TString(char* str);
	TString();
	void ToLower();
	void ToUpper();
	bool DoesWordEqualTo(unsigned int wordN, const char* word);
	bool DoesWordEqualTo(unsigned int wordN, TString word);
	void Clear();
	int GetLength();
	const char operator[](const unsigned int i) const;
	char& operator[](const unsigned int i) ;
	TString &operator=(const TString &lr);
	TString &operator=(const char* lr);
	TString &operator+=(const char &str);
	TString &operator+=(const TString &str);
	bool operator==(TString &other) const;
	bool operator==(const char* other) const;
	bool operator==(char* other) const;
};

#endif