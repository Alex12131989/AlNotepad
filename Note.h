#pragma once
#include <string>
#include <vector>

class Note
{
public:
	std::string title = "New Note";
	std::string content;
	std::vector <int> date_of_creation = ReturnNow();
	std::vector <int> date_of_last_edition;
	bool pinned = false;
	bool special = false;

	void PinUnpin();
	void MakeRemoveSpecial();
	std::vector <int> ReturnNow();
};

