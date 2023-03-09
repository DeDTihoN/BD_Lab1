#pragma once
#include "Song.h"
#include <cstdio>
#include <string>
#include<iostream>
#include "AlbumFile.h"
#include "DeletedSongs.h"
#include <vector>
using namespace std;

class SongFile
{
private:
	const char songFilePath[12] = "SongFile.fl";
	int count = 0;
public:
	void createFile()// створення файлу
	{
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "w+b");
		fclose(songFile);
		return;
	}

	bool checkFileExists(string& error) // перевірка файлу
	{
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "r+b");
		if (songFile == NULL) {
			error = "Album file does not exist";
			createFile();
			return 0;
		}
		fclose(songFile);
		return 1;
	}

	Song readSong(int adress)
	{
		Song song;
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "r+b");
		fseek(songFile, adress, SEEK_SET);
		fread(&song, sizeof(Song), 1, songFile);
		fclose(songFile);
		return song;
	}
	void changeSong(Song song, int adress)
	{
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "r+b");
		fseek(songFile, adress, SEEK_SET);
		fwrite(&song, sizeof(Song), 1, songFile);
		fclose(songFile);
		return;
	}

	void writeSong(Song song)
	{
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "a+b");
		fseek(songFile, 0, SEEK_END);
		fwrite(&song, sizeof(Song), 1, songFile);
		fclose(songFile);
		return;
	}

	int getSeekEnd()
	{
		int seekEnd;
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "r+b");
		fseek(songFile, 0, SEEK_END);
		seekEnd = ftell(songFile);
		fclose(songFile);
		return seekEnd;
	}

	bool checkSongExists(int albumKey, int songKey, string& error)// повертає 0 якщо пісні не існує або була видалена і 1 в іншому випадку
	{
		AlbumFile aFl = AlbumFile();
		Album album = aFl.getAlbum(albumKey, error);
		if (error != "")return 0;
		int adress = album.firstSongAdress;
		while (adress != -1) {
			Song song = readSong(adress);
			if (song.songId == songKey) {
				if (song.isDeleted == 1) {
					error = "song with key = " + to_string(songKey) + " was deleted";
				}
				return !song.isDeleted;
			}
			adress = song.nextAdress;
		}
		error = "song with key = " + to_string(songKey) + " does not exist";
		return 0;
	}

	int getAdressOfSong(int albumKey, int songKey, string& error) // повертає адресу пісні або -1 якщо її не існує
	{
		if (!checkSongExists(albumKey, songKey, error))return -1;
		AlbumFile aFl = AlbumFile();
		Album album = aFl.getAlbum(albumKey, error);
		if (error != "")return -1;
		int adress = album.firstSongAdress;
		while (adress != -1) {
			Song song = readSong(adress);
			if (song.songId == songKey) {
				return adress;
			}
			adress = song.nextAdress;
		}
		return -1;
	}

	Song getSong(int albumKey, int songKey, string& error) // отримує пісню по айді
	{
		int adress = getAdressOfSong(albumKey, songKey, error);
		if (error != "")return Song();
		Song song = readSong(adress);
		return song;
	}

	void updateSong(int albumKey, int songKey, Song song, string& error) // оновлення полів пісні
	{
		int adress = getAdressOfSong(albumKey, songKey, error);
		if (error != "")return;
		changeSong(song, adress);
		return;
	}

	void addNewSongAdress(int albumKey, int addingAdress, string& error) // оновлення посилання на нову пісню
	{
		AlbumFile aFl = AlbumFile();
		Album album = aFl.getAlbum(albumKey, error);
		if (error != "")return;
		if (album.firstSongAdress == -1) {
			album.firstSongAdress = addingAdress;
			aFl.updateAlbum(albumKey, album, error);
			return;
		}
		else {
			int adress = album.firstSongAdress;
			Song song = readSong(adress);
			while (song.nextAdress != -1) {
				adress = song.nextAdress;
				song = readSong(adress);
			}
			song.nextAdress = addingAdress;
			changeSong(song, adress);
			return;
		}
	}


	int addSong(int albumKey, Song song, string& error) // додавання пісней
	{
		DeletedSongs dSongs = DeletedSongs();
		int adress = dSongs.getFirstDeletedAdress();
		song.songId = ++count;
		if (adress == -1) {
			addNewSongAdress(albumKey, getSeekEnd(), error);  // змінюємо посилання пісень
			if (error != "")return 0;
			writeSong(song);
		}
		else {
			addNewSongAdress(albumKey, adress, error);  // змінюємо посилання пісень
			if (error != "")return 0;
			changeSong(song, adress);
		}
		return song.songId;
	}

	void deleteSong(int albumKey, int songKey, string& error) // видалення пісні
	{
		if (!checkSongExists(albumKey, songKey, error))return;
		AlbumFile aFl = AlbumFile();
		Album album = aFl.getAlbum(albumKey, error);
		DeletedSongs dSongs = DeletedSongs();
		if (error != "")return;
		int adress = album.firstSongAdress;
		vector< pair<int, Song> > songs;
		while (adress != -1) {
			Song song = readSong(adress);
			songs.push_back({ adress,song });
			adress = song.nextAdress;
		}
		if (songs.size() == 1) {
			dSongs.addDeletedAdress(songs[0].first);
			songs[0].second.isDeleted = 1;
			changeSong(songs[0].second, songs[0].first);
			album.firstSongAdress = -1;
			aFl.updateAlbum(albumKey, album, error);
			return;
		}
		else {
			if (songs[0].second.songId == songKey) {
				dSongs.addDeletedAdress(songs[0].first);

				songs[0].second.isDeleted = 1;
				changeSong(songs[0].second, songs[0].first);

				album.firstSongAdress = songs[1].first;
				aFl.updateAlbum(albumKey, album, error);
				return;
			}
			else {
				for (int i = 1; i < songs.size(); ++i) {
					if (songs[i].second.songId == songKey) {
						dSongs.addDeletedAdress(songs[i].first);
						if (i == songs.size() - 1) {
							songs[i - 1].second.nextAdress = -1;
						}
						else {
							songs[i - 1].second.nextAdress = songs[i + 1].first;
						}
						changeSong(songs[i - 1].second, songs[i - 1].first);
						songs[i].second.isDeleted = 1;
						changeSong(songs[i].second, songs[i].first);
						return;
					}
				}
			}
		}
	}


	void deleteAlbum(int albumKey, string& error) // видалення альбому з зазначеним айді
	{
		AlbumFile aFl = AlbumFile();
		IndexFile fl = IndexFile();
		DeletedSongs dSongs = DeletedSongs();
		DeletedAlbums dAl = DeletedAlbums();
		Album album = aFl.getAlbum(albumKey, error);
		if (error != "")return;

		int deletedAlbumAdress = fl.getAdressByKey(albumKey, error);
		album.isDeleted = 1;
		dAl.addDeletedAdress(deletedAlbumAdress);
		aFl.updateAlbum(albumKey, album, error);
		fl.deleteIndex(albumKey, error);

		int adress = album.firstSongAdress;

		while (adress != -1) {
			Song song = readSong(adress);
			song.isDeleted = 1;
			changeSong(song, adress);
			dSongs.addDeletedAdress(adress);
			adress = song.nextAdress;
		}
	}

	void writeAlbumAndSongsInfo(string& error)
	{
		IndexFile fl = IndexFile();
		vector<int>albumKeys = fl.getRealKeys();
		cout << "There are " + to_string(albumKeys.size()) + " albums in database" << endl;
		AlbumFile aFl = AlbumFile();
		for (int i = 0; i < albumKeys.size(); ++i) {
			int albumKey = albumKeys[i];
			Album album = aFl.getAlbum(albumKey, error);
			cout << '\t' << "The album number " << i + 1 << " with key = " << album.id << ":" << endl;
			cout << '\t' << "Name: " << album.name << endl;
			cout << '\t' << "Release date: " << album.date << endl;
			cout << '\t' << "Recordind studio: " << album.recordStudio << endl;
			int adress = album.firstSongAdress;
			vector<Song>songs;
			while (adress != -1) {
				Song song = readSong(adress);
				songs.push_back(song);
				adress = song.nextAdress;
			}
			cout << '\t' << '\t' << "Number of songs: " << songs.size() << endl;
			for (int j = 0; j < songs.size(); ++j) {
				Song song = songs[j];
				cout << '\t' << '\t' << "The song number " << j + 1 << " with key = " << song.songId << endl;
				cout << '\t' << '\t' << "Name: " << song.name << endl;
				cout << '\t' << '\t' << "Duration: " << song.duration << endl;
				cout << '\t' << '\t' << "Genre: " << song.genre << endl;
				//cout << "----------------------------------------------------------" << endl;
			}
			//cout<<"<<<<<------------------------------------------------>>>>>" << endl;
		}
	}
};

