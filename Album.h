#pragma once
#include <string>
#include "shortString.h"
using namespace std;
class Album
{
public:
	int id;
	shortString name;
	shortString recordStudio;
	shortString date;
	int firstSongAdress;
	bool isDeleted;
	Album(int id, string name, string recordStudio, string date, int firstSongAdress, bool isDeleted)
	{
		this->id = id;
		this->name = name;
		this->recordStudio = recordStudio;
		this->date = date;
		this->firstSongAdress = firstSongAdress;
		this->isDeleted = isDeleted;
	}
	Album(string name, string recordStudio, string date)
	{
		this->name = name;
		this->recordStudio = recordStudio;
		this->date = date;
		firstSongAdress = -1;
		isDeleted = 0;
		id = -1;
	}
	Album()
	{
		id = 0;
		firstSongAdress = -1;
		isDeleted = 0;
	}
};

