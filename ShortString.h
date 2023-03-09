#pragma once
#include<string>
#include<iostream>
using namespace std;

class shortString
{
public:
	char str[30];
	shortString()
	{
		for (int i = 0; i < 30; ++i)str[i] = 0;
	}
	shortString(string s)
	{
		strCopy(s);
	}
	shortString(const shortString& s)
	{
		for (int i = 0; i < 30; ++i) {
			str[i] = s.str[i];
		}
	}
	void strCopy(string s)
	{
		if (s.size() > 30) {
			throw new exception("input string is more than 30 symbols");
		}
		for (int i = 0; i < s.size(); ++i)
		{
			str[i] = s[i];
		}
	}
	friend ostream& operator<< (ostream& out, const shortString& s)
	{
		for (int i = 0; i < 30; ++i) {
			if (s.str[i] == 0)break;
			cout << s.str[i];
		}
		return out;
	}
	friend istream& operator>> (istream& in, shortString& s)
	{
		string S;
		cin >> S;
		s.strCopy(S);
		return in;
	}
};