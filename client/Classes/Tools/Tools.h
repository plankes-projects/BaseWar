/*
 * Tools.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <string>
#include <vector>
#include "../Model/DamageType.h"
#include "../Model/Difficulty.h"

class Tools {
public:
	static std::string toString(int i);
	static std::string toString(float f);
	static std::string toString(float f, int decimalPlaces);
	static std::string toString(DamageType d);
	static std::string toString(Difficulty d);
	static std::string toString(std::vector<char> *buffer);

	static std::string getRandomString(int length);
	static int random(int from_inclusive, int to_inclusive);
	static unsigned int getNumberOfLines(std::string str);
	static std::vector<std::string> splitStringBy(std::string string, char seperator);

private:
	Tools();
	virtual ~Tools();
};

#endif /* TOOLS_H_ */
