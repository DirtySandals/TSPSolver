#pragma once

#ifndef CITY_H
#define CITY_H

// City is a struct for storing the 2D coordinates of city and its index in the problem instance
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