#pragma once

#include <string>
#include <vector>
#include <cmath>

class Utils {
public:
	static int min(int a, int b) {

		return (b < a) ? b : a;
	}
	//unsigned int split
	//split a string <txt> everywhere a certain character <ch> is found
	//store the resulting substrings in a vector<srts>
	//return the size of the vector
	static unsigned int split(const std::string &text, std::vector<std::string> &strs, char ch) {
		int pos = text.find(ch); //first position of character
		int initialPos = 0;
		strs.clear();
		//npos = no match
		while (pos != std::string::npos) {
			strs.push_back(text.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;

			pos = text.find(ch, initialPos);
		}
		//Add the last one
		strs.push_back(text.substr(initialPos, min(pos, text.size() - (initialPos + 1))));
		return strs.size();
	
	}
	
};