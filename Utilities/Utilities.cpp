//////////////////////////////////////////////////////////////////////
// Utilities.cpp	:	Provides bunch of useful functions. Some	//
//						or all of them are always used in project	//
// Version			:	1.2											//
// -----------------------------------------------------------------//
// Copyright © Venkata Chekuri, 2016								//
// All rights granted provided that this notice is retained.		//
// -----------------------------------------------------------------//
// Language			:	C++ 11										//
// Platform			:	MSI GE62 2QD, Core-i7, Windows 10			//
// Application		:	XML											//
// -----------------------------------------------------------------//
// Author			:	Venkata Bharani Krishna Chekuri				//
//						vbchekur@syr.edu							//
//////////////////////////////////////////////////////////////////////

#include "Utilities.h"

#include <sstream> 
#include <iostream>
#include <algorithm>

using namespace Utilities;

/** Function To Print Source String Underlined With Default or
  * Character in 2nd Argument to Function.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void StringHelper::Title(const std::string &src, char under) {
	std::string out;
	int len = src.length();
	out.append(std::string("\n " + src + "\n "));
	for (int i = 0; i <= len; i++) {
		out.push_back(under);
	}
	std::cout << out;
}

/** Function to Print Source String in a Box With Default or
  * Character in 2nd Argument to Function.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
std::string StringHelper::PrintInBox(const std::string &src) {
	std::string out;
	int len = src.length() + 1;
	out.append("\n ");
	for (int i = 0; i <= len; i++) {
		out.push_back('-');
	}
	out.append("\n  " + src + "\n ");
	for (int i = 0; i <= len; i++) {
		out.push_back('-');
	}
	std::cout << out;
	return out;
}

/** Function To Remove Leading And Trailing WhiteSpaces From
  * Source(Parameter) String.
  *
  * Dependant Function(s) - N/A
  * 
  * Ver : 1.0
  *		- First Release.
  */
std::string StringHelper::Trim(const std::string & src) {
	std::size_t firstScan = src.find_first_not_of(' ');
	std::size_t first = firstScan == std::string::npos ? src.length() : firstScan;
	std::size_t last = src.find_last_not_of(' ');
	return src.substr(first, last - first + 1);
}

/** Function To Split a String Based on A Character Delimiter.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
std::vector<std::string> StringHelper::Split(const std::string &src, char delimiter) {
	std::vector<std::string> result;
	std::string tmp;
	for (char ch : src) {
		if (ch == delimiter && ch != '\n') {
			if (tmp.size() > 0) {
				result.push_back(tmp);
				tmp.clear();
			}
			else
				continue;
		}
		else if (ch == '\0') {
			result.push_back(tmp);
			break;
		}
		else {
			tmp.push_back(ch);
		}
	}
	if (!tmp.empty())
		result.push_back(tmp);
	return result;
}

/** Function to Find if A Specific Sub-String Exists Within A String. 
  * Parameter 1 is the String and Parameter 2 is the Sub String we are Searching For.
  * Function Will Return Index of Sub String if it Exists, else it'll Return -1.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
int StringHelper::FindSubStr(const std::string & str, const std::string & sub, int start) {
	int pos = 0;
	int pop = sub.size() - 1, move = 0;
	for (size_t counter = start; counter < str.size() - sub.size(); counter++) {
		if (str[counter] == sub[0]) {
			move = 0;
			for (size_t inner = counter; inner < str.size(); inner++) {
				if (str[inner] == sub[move]) {
					if (move == pop) {
						return counter;
					}
					move++;
				}
				else {
					break;
				}
			}
		}
	}
	return -1;
}

/** Function To Convert A String to Lower Case Characters.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void StringHelper::ToLower(std::string & str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

/** Function To Convert A String to Upper Case Characters.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void StringHelper::ToUpper(std::string & str) {
	std::transform(str.begin(), str.end(), str.begin(), _ALGORITHM_::toupper);
}

/** Function To Start High Resolution Clock.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void Timer::Begin() {
	reference = std::chrono::high_resolution_clock::now();
}

/** Function To Stop High Resolution Clock and Return
  * the Time Elapsed.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
double Timer::End() {
	std::chrono::time_point<std::chrono::steady_clock> elapsed = std::chrono::high_resolution_clock::now();
	std::chrono::duration<int64_t, std::nano> elapsed_secs = elapsed - reference;
	time_taken = (double)elapsed_secs.count();
	return time_taken;
}

/** Function to Return Elapsed Time.
  * 
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
double Timer::TimeTaken() {
	return time_taken;
}

/** Helper Function For TimeForTask Function.
  *
  * Dependant Function(s) - N/A
  * 
  * Ver : 1.0
  *		- First Release.
  */
void Timer::HelperTimeForTask(int &unit, double &time) {
	if (unit == 3)
		return;

	if (time > 1000) {
		time = time / 1000;
		unit++;
	}

	if (time > 1000)
		HelperTimeForTask(unit, time);
}

/** Function To Return Elapsed Time But Automatically
  * Change the Units So That it is Easier to Read.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
std::string Timer::TimeForTask() {
	double time = time_taken;
	int unit = 0;
	HelperTimeForTask(unit, time);
	std::stringstream res;
	res << StringHelper::PrintInBox(std::string("Time Taken : " + std::to_string(time) + " " + units[unit]));
	
	return res.str();
}

#ifdef TEST_UTILITIES
/** Function to Test StringHelper Class.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void TestStringHelper() {
	using Helper = Utilities::StringHelper;
	
	Helper::Title("Testing StringHelper Class", '=');

	std::string test_title = "Testing Title";
	std::string test_split = "Alpha, Beta, Chi, Delta, Epsilon Omega";

	Helper::Title(test_title);

	std::cout << "\n\n";
	Helper::Title("Testing String Split");
	std::cout << "\n Original String : \"" << test_split << " \"";
	std::cout << "\n Delimiter : ','";
	std::cout << "\n String Split -";
	std::vector<std::string> split_result = Helper::Split(test_split);
	for (std::string str : split_result) {
		std::cout << "\n \"" << str << "\"";
	}
}

/** Function to Test Timer Class.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void TestTimer() {
	using Helper = Utilities::StringHelper;
	
	Helper::Title("Testing Timer Class", '=');
	std::cout << "\n Task : Add numbers till 1000";
	int val = 0;
	Timer performance;
	performance.Begin();
	for (int i = 1; i <= 100; i++) {
		val += i;
	}
	double time = performance.End();
	std::cout << "\n Time Taken : " << time << " nanosecs";
}

/** Function to Test All Classes in Utilities
  * Project.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
int main() {
	Timer time;
	time.Begin();
	Utilities::StringHelper::Title("Testing Utilities Package", '=');
	std::cout << "\n";
	TestStringHelper();
	std::cout << "\n";
	TestTimer();
	std::cout << "\n";
	time.End();
	time.TimeForTask();
	std::cout << "\n\n";
}
#endif // TEST_UTILITIES
