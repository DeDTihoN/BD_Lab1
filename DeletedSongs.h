#pragma once
#include <cstdio>
#include <string>
#include<iostream>
#include <vector>
using namespace std;
class DeletedSongs
{
private:
	const char deletedSongsFilePath[20] = "DeletedSongsFile.fl";
public:
	void createFile()// створення файлу
	{
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "w+b");
		fclose(deletedSongs);
		return;
	}
	bool checkFileExists(string& error) // перевірка файлу
	{
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "r+b");
		if (deletedSongs == NULL) {
			error = "Album file does not exist";
			createFile();
			return 0;
		}
		fclose(deletedSongs);
		return 1;
	}
	void addDeletedAdress(int adress) // додавання адреси до видалених
	{
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "a+b");
		fseek(deletedSongs, 0, SEEK_END);
		fwrite(&adress, sizeof(int), 1, deletedSongs);
		fclose(deletedSongs);
		return;
	}
	int readDeletedAdress(int adress) // отримує значення в файлі по вказаному місцю в файлі
	{
		int deletedAdress;
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "r+b");
		fseek(deletedSongs, adress, SEEK_SET);
		fread(&deletedAdress, sizeof(int), 1, deletedSongs);
		fclose(deletedSongs);
		return deletedAdress;
	}
	int getNumberOfAdresses()
	{
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "r+b");
		fseek(deletedSongs, 0, SEEK_END);
		int n = ftell(deletedSongs) / sizeof(int);
		fclose(deletedSongs);
		return n;
	}
	int getFirstDeletedAdress() // (повертає адресу першого видаленого альбому і видаляє цю адресу) або повертає -1 якщо видалених немає
	{
		int n = getNumberOfAdresses();
		if (n == 0) {
			return -1;
		}
		vector<int>adresses(n);
		for (int i = 0; i < n; ++i) {
			adresses[i] = readDeletedAdress(i * sizeof(int));
		}

		createFile();
		for (int i = 1; i < n; ++i) {
			addDeletedAdress(adresses[i]);
		}
		int firstAdress = adresses[0];
		return firstAdress;
	}
};

