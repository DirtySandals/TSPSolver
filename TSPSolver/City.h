#pragma once

#ifndef CITY_H
#define CITY_H

struct City {
	int index;
	float x;
	float y;
	City();
	City(int index, float x, float y);
	City& operator=(const City& other);
	bool is_Empty();
};

#endif