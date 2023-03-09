#pragma once
class Index
{
	public:
	int id, adress;
	bool isDeleted;
	Index(int id, int adress, bool isDeleted)
	{
		this->id = id;
		this->adress = adress;
		this->isDeleted = isDeleted;
	}
	Index()
	{
		id = 0;
		adress = 0;
		isDeleted = 0;
	}
};

