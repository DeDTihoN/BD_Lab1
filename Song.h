#pragma once
#include<string>
#include "shortString.h"
using namespace std;

class Song
{
public:
	int duration;
	shortString name;
	shortString genre;
	bool isDeleted;
	int nextAdress;
	int albumId;
	int songId;
	Song(int duration, string name, string genre, bool isDeleted, int nextAdress, int albumId, int songId)
	{
		this->duration = duration;
		this->name = name;
		this->genre = genre;
		this->isDeleted = isDeleted;
		this->nextAdress = nextAdress;
		this->albumId = albumId;
		this->songId = songId;
	}
	Song()
	{
		duration = 0;
		isDeleted = 0;
		nextAdress = -1;
		albumId = 0;
		songId = 0;
	}
};

