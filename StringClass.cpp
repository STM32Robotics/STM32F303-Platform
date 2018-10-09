#include "StringClass.h"

void TString::ResetBuffer()
{
	ErrorBuffer[0] = 0;
	for (unsigned int i = 0; i < BufferSize; i++)
		Buffer[i] = 0;
	StrSize = 0;
}


TString::TString(const char* str)
{
	ResetBuffer();
	strcpy(this->Buffer, str);
	StrSize = strlen(str);
}

TString::TString(char* str)
{
	ResetBuffer();
	strcpy(this->Buffer, str);
	StrSize = strlen(str);
}

TString::TString()
{
	ResetBuffer();
}

void TString::ToLower()
{
	for (unsigned int i = 0; i < StrSize; i++)
		Buffer[i] = tolower(Buffer[i]);
}

void TString::ToUpper()
{
	for (unsigned int i = 0; i < StrSize; i++)
		Buffer[i] = toupper(Buffer[i]);
}

bool TString::DoesWordEqualTo(unsigned int wordN, const char* word)
{
	if (wordN == 0)
		return false;

	char Word[25] = { 0 };
	unsigned int WordIndex = 0;
	bool FoundWord = false;
	unsigned int CurrentIndex = 1;

	for (unsigned int i = 0; i < StrSize; i++)
	{
		if ((Buffer[i] == ' ' || i == StrSize - 1) && CurrentIndex == wordN)
		{
			FoundWord = true;
			if (i == StrSize - 1)
				Word[WordIndex] = Buffer[i];
			break;
		}
		else if(Buffer[i] == ' ')
		{
			for (unsigned int j = 0; j < 25; j++)
				Word[j] = 0;
			CurrentIndex++;
			WordIndex = 0;
		}
		else
		{
			Word[WordIndex] = Buffer[i];
			WordIndex++;
		}
	}

	if (!FoundWord)
		return false;
	if (strlen(Word) != strlen(word))
		return false;

	for (unsigned int i = 0; i < strlen(word); i++)
		if (Word[i] != word[i])
			return false;
	return true;
}

void TString::RemoveFirstWords(unsigned int wordCount)
{
	unsigned int cnt = 0;
	unsigned int i = 0;
	for(i = 0; i < StrSize; i++)
	{
		if(Buffer[i] == ' ')
		{
			cnt++;
			if(cnt >= wordCount)
			{
				i++;
				break;
			}
		}
	}
	
	for(unsigned int j = 0; j < StrSize - i; j++)
	{
		Buffer[j] = Buffer[j + i];
	}
	for(unsigned int j = 0; j < i; j++)
	{
		Buffer[StrSize - j - 1] = 0;
	}
	for(unsigned int j = 0;; j++)
	{
		if(Buffer[j] == 0)
		{
			StrSize = j;
			return;
		}
	}
}

bool TString::DoesWordEqualTo(unsigned int wordN, TString word)
{
	if (wordN == 0)
		return false;

	char Word[25] = { 0 };
	unsigned int WordIndex = 0;
	bool FoundWord = false;
	unsigned int CurrentIndex = 1;

	for (unsigned int i = 0; i < StrSize; i++)
	{
		if ((Buffer[i] == ' ' || i == StrSize - 1) && CurrentIndex == wordN)
		{
			FoundWord = true;
			if (i == StrSize - 1)
				Word[WordIndex] = Buffer[i];
			break;
		}
		else if(Buffer[i] == ' ')
		{
			for (unsigned int j = 0; j < 25; j++)
				Word[j] = 0;
			CurrentIndex++;
			WordIndex = 0;
		}
		else
		{
			Word[WordIndex] = Buffer[i];
			WordIndex++;
		}
	}

	if (!FoundWord)
		return false;
	if (strlen(Word) != word.GetLength())
		return false;

	for (unsigned int i = 0; i < word.GetLength(); i++)
		if (Word[i] != word[i])
			return false;
	return true;
}

void TString::Clear()
{
	ResetBuffer();
}

int TString::GetLength()
{
	return StrSize;
}

const char TString::operator[](const unsigned int i) const 
{
	if (i >= BufferSize)
		return 0;
	if (i >= StrSize)
		return 0;
	return Buffer[i];
}

char& TString::operator[](const unsigned int i) 
{ 
	if (i >= BufferSize)
		return ErrorBuffer[0];
	if (i >= StrSize)
		return ErrorBuffer[0];
	return Buffer[i]; 
}

TString &TString::operator=(const TString &lr)
{
	ResetBuffer();
	for (unsigned int i = 0; i < lr.StrSize; i++)
		this->Buffer[i] = lr.Buffer[i];
	this->StrSize = lr.StrSize;
	return *this;
}

TString &TString::operator=(const char* lr)
{
	ResetBuffer();
	strcpy(this->Buffer, lr);
	StrSize = strlen(lr);
	return *this;
}

TString &TString::operator+=(const char &str)
{
	Buffer[StrSize] = str;
	StrSize++;
	if(StrSize >= BufferSize)
	{
		StrSize = StrSize - 1;
		return *this;
	}
	return *this;
}

TString &TString::operator+=(const unsigned int &num)
{
	char str[128];
	snprintf(str, 128, "%d", num);
	for(unsigned int i = 0; i < strlen(str); i++)
	{
		*this += str[i];
	}
	return *this;
}

TString &TString::operator+=(const TString &str)
{
	for (unsigned int i = 0; i < str.StrSize; i++)
		*this += str[i];
	return *this;
}

bool TString::operator==(TString &other) const
{
	if (this->StrSize != other.StrSize)
		return false;
	for (unsigned int i = 0; i < this->StrSize; i++)
		if (this->Buffer[i] != other.Buffer[i])
			return false;
	return true;
}

bool TString::operator==(const char* other) const
{
	if (this->StrSize != strlen(other))
		return false;
	for (unsigned int i = 0; i < this->StrSize; i++)
		if (this->Buffer[i] != other[i])
			return false;
	return true;
}

bool TString::operator==(char* other) const
{
	if (this->StrSize != strlen(other))
		return false;
	for (unsigned int i = 0; i < this->StrSize; i++)
		if (this->Buffer[i] != other[i])
			return false;
	return true;
}