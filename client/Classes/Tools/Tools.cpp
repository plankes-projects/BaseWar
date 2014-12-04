/*
 * Tools.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Tools.h"
#include <sstream>
#include <math.h>       /* pow */
#include <stdlib.h>     /* srand, rand */
#include "BW_Time.h"

Tools::Tools() {

}

Tools::~Tools() {
}

std::vector<std::string> Tools::splitStringBy(std::string string, char seperator) {
	std::stringstream ss(string);
	std::string segment;
	std::vector < std::string > seglist;

	while (std::getline(ss, segment, seperator))
		seglist.push_back(segment);

	return seglist;
}

std::string Tools::toString(std::vector<char> *buffer) {
	std::string str((*buffer).begin(), (*buffer).end());
	return str;
}

std::string Tools::toString(int i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}

std::string Tools::toString(float f) {
	std::stringstream ss;
	ss << f;
	return ss.str();
}

std::string Tools::toString(float f, int decimalPlaces) {
	std::string ret = Tools::toString((int) f);
	if ((float) (int) f != f) //kommazahl?
			{
		int mult = pow(10, decimalPlaces);
		int damage_tmp = f * mult;
		ret += "." + Tools::toString(damage_tmp % mult);
	}
	return ret;
}

std::string Tools::toString(DamageType d) {
	switch (d) {
	case NORMAL:
		return "Normal";
	case MAGIC:
		return "Magic";
	case PIERCING:
		return "Piercing";
	case CHAOS:
		return "Chaos";
	default:
		return "Can't convert to string";
	}
}

std::string Tools::toString(Difficulty d) {
	switch (d) {
	case DIF_EASY:
		return "Easy";
	case DIF_NORMAL:
		return "Normal";
	case DIF_HARD:
		return "Hard";
	case DIF_VERYHARD:
		return "Very hard";
	case DIF_HELL:
		return "Survival";
	default:
		return "Can't convert to string";
	}
}

std::string Tools::getRandomString(int length) {
	std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int lastChar = chars.size() - 1;

	std::string str = "";
	for (int i = 0; i < length; ++i)
		str += chars[Tools::random(0, lastChar)];

	return str;
}

/*
 * inclusive from
 * inclusive to
 */
int Tools::random(int from_inclusive, int to_inclusive) {
	static int init = false;
	if (!init) {
		//if we did not init the random function, we do so now
		srand(BW_Time::getMilliSeconds());
		init = true;
	}

	// x is in [0,1[
	double x = rand() / static_cast<double>(RAND_MAX);

	to_inclusive++; //inclusive to_inclusive
	// [0,1[ * (max - min) + min is in [min,max[
	return from_inclusive + static_cast<int>(x * (to_inclusive - from_inclusive));
}

unsigned int Tools::getNumberOfLines(std::string str) {
	int nol = 0;
	for (unsigned int i = 0; i < str.size(); i++) {
		if (str[i] == '\n') {
			nol++;
		}
	}
	return nol;
}
