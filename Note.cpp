#include "Note.h"
#include <chrono>

void Note::PinUnpin()
{
	if (pinned)
		pinned = false;
	else
		pinned = true;
}

void Note::MakeRemoveSpecial()
{
	if (special)
		special = false;
	else
		special = true;
}

std::vector <int> Note::ReturnNow()
{
	using namespace std::chrono;
	time_point<system_clock> current_time = system_clock::now();
	auto floored = floor<days>(current_time);
	year_month_day ymd = year_month_day{ floored };
	hh_mm_ss hms = hh_mm_ss{ current_time - floored };
	int year = int(ymd.year()), 
		month = unsigned(ymd.month()),
		day = unsigned(ymd.day()),
		hour = hms.hours().count(),
		minute = hms.minutes().count(),
		second = hms.seconds().count();
	std::vector <int> time = { year, month, day, hour, minute, second };
	return time;
}
