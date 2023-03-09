#pragma once
#include <cstdio>
#include <string>
#include<iostream>
#include <vector>
using namespace std;
class DeletedAlbums
{
private:
	const char deletedAlbumsFilePath[21] = "DeletedAlbumsFile.fl";
public:
	void createFile()// створення файлу
	{
		FILE* deletedAlbums;
		fopen_s(&deletedAlbums, deletedAlbumsFilePath, "w+b");
		fclose(deletedAlbums);
		return;
	}
	bool checkFileExists(string& error) // перевірка файлу
	{
		FILE* deletedAlbums;
		fopen_s(&deletedAlbums, deletedAlbumsFilePath, "r+b");
		if (deletedAlbums == NULL) {
			error = "Album file does not exist";
			createFile();
			return 0;
		}
		fclose(deletedAlbums);
		return 1;
	}
	void addDeletedAdress(int adress) // додавання адреси до видалених
	{
		FILE* deletedAlbumsFile;
		fopen_s(&deletedAlbumsFile, deletedAlbumsFilePath, "a+b");
		fseek(deletedAlbumsFile, 0, SEEK_END);
		fwrite(&adress, sizeof(int), 1, deletedAlbumsFile);
		fclose(deletedAlbumsFile);
		return;
	}
	int readDeletedAdress(int adress) // отримує значення в файлі по вказаному місцю в файлі
	{
		int deletedAdress;
		FILE* deleteAlbumsFile;
		fopen_s(&deleteAlbumsFile, deletedAlbumsFilePath, "r+b");
		fseek(deleteAlbumsFile, adress, SEEK_SET);
		fread(&deletedAdress, sizeof(int), 1, deleteAlbumsFile);
		fclose(deleteAlbumsFile);
		return deletedAdress;
	}
	int getNumberOfAdresses()
	{
		FILE* deletedAlbumsFile;
		fopen_s(&deletedAlbumsFile, deletedAlbumsFilePath, "r+b");
		fseek(deletedAlbumsFile, 0, SEEK_END);
		int n = ftell(deletedAlbumsFile) / sizeof(int);
		fclose(deletedAlbumsFile);
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

