#include <iostream>
#include "SongFile.h"

SongFile sFl = SongFile();
AlbumFile aFl = AlbumFile();
IndexFile iFl = IndexFile();
DeletedAlbums dAl = DeletedAlbums();
DeletedSongs dSongs = DeletedSongs();

bool intValidate()
{
	if (cin.fail())
	{
		cout << "Input is incorrect" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return 0;
	}
	else return 1;
}

bool inputInt(int& x)
{
	cin >> x;
	return intValidate();
}

Album inputAlbum()
{
	Album album;
	cout << "Input name of album: ";
	cin >> album.name;
	cout << "Input release date of album: ";
	cin >> album.date;
	cout << "Input recording studio of album: ";
	cin >> album.recordStudio;
	return album;
}

Song inputSong(string& error)
{
	Song song;
	cout << "Input name of song: ";
	cin >> song.name;
	cout << "Input duration of song: ";
	if (!inputInt(song.duration)) {
		error = "Input is incorrect";
		return Song();
	}
	cout << "Input genre of song: ";
	cin >> song.genre;
	return song;
}

void addAlbum()
{
	string error;
	Album album = inputAlbum();
	album.firstSongAdress = -1;
	album.isDeleted = 0;
	int key = aFl.addAlbum(album, error);
	if (error == "") {
		cout << "Album successfully added with key = " << key << endl;
	}
	else {
		cout << error << endl;
	}
}



void addSong()
{
	string error;
	int albumId;
	cout << "Input key of album: ";
	if (!inputInt(albumId))return;
	Song song = inputSong(error);
	if (error != "") {
		cout << error << endl;
		return;
	}
	song.albumId = albumId;
	song.isDeleted = 0;
	song.nextAdress = -1;
	int songKey = sFl.addSong(albumId, song, error);
	if (error == "") {
		cout << "Song successfully added with key = " << songKey << endl;
	}
	else {
		cout << error << endl;
	}
}

void updateAlbum()
{
	string error;
	int albumId;
	cout << "Input key of album: ";
	if (!inputInt(albumId))return;
	Album album = inputAlbum();
	Album oldAlbum = aFl.getAlbum(albumId, error);
	if (error != "") {
		cout << error << endl;
		return;
	}

	oldAlbum.name = album.name;
	oldAlbum.date = album.date;
	oldAlbum.recordStudio = album.recordStudio;
	aFl.updateAlbum(albumId, oldAlbum, error);

	if (error == "")
	{
		cout << "Album successfully updated" << endl;
	}
	else {
		cout << error << endl;
	}
}

void updateSong()
{
	string error;
	int albumId, songId;
	cout << "Input key of album: ";
	if (!inputInt(albumId))return;
	cout << "Input key of song: ";
	if (!inputInt(songId))return;
	Song song = inputSong(error);
	if (error != "") {
		cout << error << endl;
		return;
	}
	Song oldSong = sFl.getSong(albumId, songId, error);
	if (error != "") {
		cout << error << endl;
		return;
	}
	oldSong.genre = song.genre;
	oldSong.name = song.name;
	oldSong.duration = song.duration;

	sFl.updateSong(albumId, songId, oldSong, error);
	if (error == "") {
		cout << "Song successfully updated" << endl;
	}
	else {
		cout << error << endl;
	}
}

void deleteAlbum()
{
	string error;
	int albumId;
	cout << "Input key of album: ";
	if (!inputInt(albumId))return;
	sFl.deleteAlbum(albumId, error);
	if (error == "") {
		cout << "Album successfully deleted" << endl;
	}
	else {
		cout << error << endl;
	}
}

void deleteSong()
{
	string error;
	int albumId, songId;
	cout << "Input key of album: ";
	if (!inputInt(albumId))return;
	cout << "Input key of song: ";
	if (!inputInt(songId))return;
	sFl.deleteSong(albumId, songId, error);
	if (error == "") {
		cout << "Song successfully deleted" << endl;
	}
	else {
		cout << error << endl;
	}
}

void getAlbum()
{
	string error;
	int albumId;
	cout << "Input key of album: ";
	if (!inputInt(albumId))return;
	Album album = aFl.getAlbum(albumId, error);
	if (error == "") {
		cout << "Name of album: " << album.name << endl;
		cout << "Release date of album: " << album.date << endl;
		cout << "Recording studio of album: " << album.recordStudio << endl;
	}
	else {
		cout << error << endl;
	}
}

void getSong()
{
	string error;
	int albumId, songId;
	cout << "Input key of album: ";
	if (!inputInt(albumId))return;
	cout << "Input key of song: ";
	if (!inputInt(songId))return;
	Song song = sFl.getSong(albumId, songId, error);
	if (error == "") {
		cout << "Name of song: " << song.name << endl;
		cout << "Duration of song: " << song.duration << endl;
		cout << "Genre of song: " << song.genre << endl;
	}
	else {
		cout << error << endl;
	}
}

void writeDatabase()
{
	string error;
	sFl.writeAlbumAndSongsInfo(error);
	if (error != "") {
		cout << error << endl;
	}
}

int main()
{
	iFl.createFile();
	aFl.createFile();
	sFl.createFile();
	dAl.createFile();
	dSongs.createFile();
	while (true) {
		cout << "Choose option:" << endl;
		cout << "1. Add album " << endl;
		cout << "2. Delete album" << endl;
		cout << "3. Modify album" << endl;
		cout << "4. Get album" << endl;
		cout << "5. Add song " << endl;
		cout << "6. Delete song" << endl;
		cout << "7. Modify song" << endl;
		cout << "8. Get song" << endl;
		cout << "9. Get all database" << endl;
		int x;
		cin >> x;
		if (!intValidate())continue;
		if (x < 1 || x>9) {
			cout << "Option is incorrect" << endl;
		}
		else {
			if (x == 1) {
				addAlbum();
			}
			else if (x == 2) {
				deleteAlbum();
			}
			else if (x == 3) {
				updateAlbum();
			}
			else if (x == 4) {
				getAlbum();
			}
			else if (x == 5) {
				addSong();
			}
			else if (x == 6) {
				deleteSong();
			}
			else if (x == 7) {
				updateSong();
			}
			else if (x == 8) {
				getSong();
			}
			else {
				writeDatabase();
			}
		}
	}
}


