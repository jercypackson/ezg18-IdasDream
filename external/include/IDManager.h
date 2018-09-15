#pragma once

typedef unsigned int ID;

#define ID_INVALID -1

class IDManager
{
private:
	int _id;

	IDManager() {
		_id = 0;
	}

	~IDManager() { }
public:
	static IDManager& get() {
		static IDManager _manager;
		return _manager;
	}

	int getID() {
		return ++_id;
	}
};

