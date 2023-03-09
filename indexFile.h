#pragma once
#include "Index.h"
#include <cstdio>
#include <string>
#include<iostream>
#include<vector>

using namespace std;

class IndexFile
{
private:
	const char indexFilePath[15] = "AlbumIndex.ind";
public:
	void createFile()// створення файлу
	{
		FILE* indexFile;
		fopen_s(&indexFile, indexFilePath, "w+b");
		fclose(indexFile);
		return;
	}
	bool checkFileExists(string& error)// перевірка файлу
	{
		FILE* indexFile;
		fopen_s(&indexFile, indexFilePath, "r+b");
		if (indexFile == NULL) {
			error = "Index file does not exist";
			createFile();
			return 0;
		}
		fclose(indexFile);
		return 1;
	}
	Index readIndex(int id)
	{
		Index index;
		FILE* indexFile;
		fopen_s(&indexFile, indexFilePath, "r+b");
		fseek(indexFile, (id - 1) * sizeof(Index), SEEK_SET);
		fread(&index, sizeof(Index), 1, indexFile);
		fclose(indexFile);
		return index;
	}

	void changeIndex(int id, Index index)
	{
		FILE* indexFile;
		fopen_s(&indexFile, indexFilePath, "r+b");
		fseek(indexFile, (id - 1) * sizeof(Index), SEEK_SET);
		fwrite(&index, sizeof(Index), 1, indexFile);
		fclose(indexFile);
		return;
	}

	bool addIndex(int adress, string& error) // додавання індексу повертає 1 якщо вдалось додати
	{
		int newKey = getLastKey() + 1;
		if (newKey == 0)return 0;
		Index index(newKey, adress, 0);
		FILE* indexFile;
		fopen_s(&indexFile, indexFilePath, "a+b");
		fseek(indexFile, 0, SEEK_END);
		fwrite(&index, sizeof(index), 1, indexFile);
		fclose(indexFile);
		return 1;
	}

	int getLastKey() // отримання значення останього доданого айді
	{
		FILE* indexFile;
		fopen_s(&indexFile, indexFilePath, "r+b");
		fseek(indexFile, 0, SEEK_END);
		int key = ftell(indexFile) / sizeof(Index);
		fclose(indexFile);
		return key;
	}
	bool checkKey(int id, string& error)
	{
		if (!checkFileExists(error))return 0;
		if (id > 0 && id <= getLastKey()) {
			return 1;
		}
		else {
			error = "No record with key = " + to_string(id);
			return 0;
		}
	}
	bool checkKeyIsReal(int id, string& error)// перевірка чи існує індекс з зазначеним айді
	{
		if (!checkKey(id, error))return 0;
		Index index = readIndex(id);
		if (index.isDeleted == 1) {
			error = "Record with key = " + to_string(id) + " was deleted";
			return 0;
		}
		else return 1;
	}
	vector<int> getRealKeys()
	{
		vector<int>res;
		int n = getLastKey();
		for (int i = 0; i < n; ++i) {
			Index index = readIndex(i + 1);
			if (!index.isDeleted)res.push_back(i + 1);
		}
		return res;
	}
	bool deleteIndex(int key, string& error)// видалення індексу з таблиці 
	{
		if (!checkKey(key, error)) {
			return 0;
		}
		Index index = readIndex(key);
		index.isDeleted = 1;
		changeIndex(key, index);
		return 1;
	}

	void writeIndexInfo() // виведення індексної таблиці
	{
		string error;
		if (!checkFileExists(error)) {
			cout << "File does not exist" << endl;
		}
		else {
			int n = getLastKey();
			cout << n << " indexes in index table" << endl;
			for (int i = 0; i < n; ++i) {
				Index index = readIndex(i + 1);
				cout << "Adress: " << index.adress << ' ' << "Key: " << index.id << ' ' << "Deleted: " << index.isDeleted << endl;
			}
		}
	}

	int getAdressByKey(int key, string error) // отримання місця запису альбома в файлі по індексу
	{
		if (!checkKey(key, error))return -1;
		Index index = readIndex(key);
		return index.adress;
	}
};

