#pragma once
#include "Album.h"
#include "indexFile.h"
#include <cstdio>
#include <string>
#include<iostream>
#include "DeletedAlbums.h"
using namespace std;

class AlbumFile
{
private:
	const char albumFilePath[13] = "AlbumFile.fl";
public:
	void createFile()// створення файлу
	{
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "w+b");
		fclose(albumFile);
		return;
	}

	bool checkFileExists(string& error) // перевірка файлу
	{
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "r+b");
		if (albumFile == NULL) {
			error = "Album file does not exist";
			createFile();
			return 0;
		}
		fclose(albumFile);
		return 1;
	}

	int getSeekEnd()
	{
		int seekEnd;
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "r+b");
		fseek(albumFile, 0, SEEK_END);
		seekEnd = ftell(albumFile);
		fclose(albumFile);
		return seekEnd;
	}

	Album readAlbum(int adress)
	{
		Album album;
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "r+b");
		fseek(albumFile, adress, SEEK_SET);
		fread(&album, sizeof(Album), 1, albumFile);
		fclose(albumFile);
		return album;
	}

	void changeAlbum(Album album, int adress)
	{
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "r+b");
		fseek(albumFile, adress, SEEK_SET);
		fwrite(&album, sizeof(Album), 1, albumFile);
		fclose(albumFile);
		return;
	}

	void writeAlbum(Album album)
	{
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "a+b");
		fseek(albumFile, 0, SEEK_END);
		fwrite(&album, sizeof(Album), 1, albumFile);
		fclose(albumFile);
		return;
	}

	Album getAlbum(int key, string& error) // отримує альбом по айді якщо він існує
	{
		IndexFile fl = IndexFile();
		if (!fl.checkKeyIsReal(key, error))return Album();
		int adress = fl.getAdressByKey(key, error);
		if (error != "")return Album();
		return readAlbum(adress);
	}

	int addAlbum(Album album, string& error) // додає альбом до файлу у звільнене місце або у кіньце якщо звільнених немає
	{
		DeletedAlbums dAl = DeletedAlbums();
		int adress = dAl.getFirstDeletedAdress();
		IndexFile iFl = IndexFile();
		album.id = iFl.getLastKey() + 1;
		if (adress == -1) {
			iFl.addIndex(getSeekEnd(), error);
			writeAlbum(album);
		}
		else {
			iFl.addIndex(adress, error);
			if (error != "")return 0;
			changeAlbum(album, adress);
		}
		return album.id;
	}


	void updateAlbum(int key, Album album, string& error) // оновлює поля альбома з зазначеним айді
	{
		IndexFile iFl = IndexFile();
		int adress = iFl.getAdressByKey(key, error);
		if (error != "")return;
		changeAlbum(album, adress);
		return;
	}

};

