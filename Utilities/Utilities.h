//////////////////////////////////////////////////////////////////////
// Utilities.h		:	Provides bunch of useful functions. Some	//
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

/* 
 * PACKAGE OPERATIONS
 * ------------------
 * This Package Provides Bunch Of Functions Some or All of Which
 * are most definitely used in different projects. This package
 * consists of two public classes StringHelper and Timer.
 * 
 * StringHelper Class Provides Methods which help to perform
 * string manipulation operation(s). Also it provides some methods
 * which print output to console in a specific format.
 *
 * Timer Class helps to time execution of function(s), operation(s)
 * etc.
 *
 * DEPENDANT FILES
 * ---------------
 * Utilities.h, Utilities.cpp
 *
 * CHANGELOG
 * ---------
 * Ver 1.2 : 01/11/2017
 *	- Added ToUpper and ToLower Functions.
 *	- Added PrintInBox Function.
 *
 * Ver 1.1 : 01/09/2017
 *	- Added FindSubStr Function.
 *
 * Ver 1.0 : 01/07/2017
 *	- First Release.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <chrono>
#include <vector>
#include <string>

namespace Utilities {
	/** Class Containing Methods to Common Help With String or 
	  * Standard Out Related Operations.
	  * 
	  * Dependant Class(es) or Structure(s) - N/A
	  * 
	  * Ver : 1.2
	  *		- Added ToUpper and ToLower Functions.
	  *		- Added PrintInBox Function.
	  * Ver : 1.1
	  *		- Added FindSubStr Function.
	  * Ver : 1.0
	  *		- First Release.
	  */
	class StringHelper {
	public:
		static std::vector<std::string> Split(const std::string &src, char delimiter = ',');
		static void Title(const std::string &src, char under = '-');
		static std::string PrintInBox(const std::string &src);
		static std::string Trim(const std::string & src);
		static int FindSubStr(const std::string & str, const std::string & sub, int start = 0);
		static void ToLower(std::string & str);
		static void ToUpper(std::string & str);
	};

	/** Class Containing Methods To Measure Execution Time Of 
	  * Function(s) or Method(s).
	  * Intended to be used with Main Method to Measure Execution
	  * Time.
	  *
	  * Dependant Class(es) or Structure(s) - N/A
	  *
	  * Ver : 1.0
	  *		- First Release.
	  */
	class Timer {
	public:
		void Begin();
		double End();
		double TimeTaken();
		std::string TimeForTask();
		void HelperTimeForTask(int &unit, double &time);
	private:
		std::chrono::time_point<std::chrono::steady_clock> reference;
		double time_taken;
		std::vector<std::string> units = { "nanosec(s)", "microsec(s)", "millisec(s)", "sec(s)" };
	};
}
#endif // !UTILITIES_H
