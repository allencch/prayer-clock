/*************
Copyright (c) 2010, Allen Choong
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the project nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************/

/**
 * \file aceaster.h
 * \brief Easter Day
 *
 * \author Allen Choong
 * \date 2010-07-30
 * \version 0.0.1
 *
 * Changelog:
 * 2010-10-02 Add acGetTmToday()
 */

#include <string>
using namespace std;
 
#ifndef AC_EASTER_H
#define AC_EASTER_H

static long AC_ONE_DAY = 24 * 60 * 60; //24 hours * 60 minutes * 60 seconds

/**
 * \brief Date time using ctime
 */
class AcDateTime {
public:
	/**
	 * \brief Will get today
	 */
	AcDateTime();
	
	AcDateTime(int year,int month,int day);
	
	/**
	 * \brief Get today
	 */
	void getToday();
	
	/**
	 * \brief Convert date to string
	 */
	string toString();
	
	/**
	 * \brief Set a date
	 */
	void setDate(int year,int month,int day);
	
	/**
	 * \brief Compare 2 different dates
	 *
	 * If this object is earlier than the parameter, the return will be positive,
	 * else negative.
	 */
	int compare(AcDateTime& dt);
	
	int day,month,year;
	
protected:
	struct tm _tm;
};
 

class AcEaster : public AcDateTime {
public:
	AcEaster();
	
	/**
	 * \brief Get Easter Sunday base on the year
	 *
	 * From http://aa.usno.navy.mil/faq/docs/easter.php
	 * \param month	 Output of the month in range [1,12]
	 * \param day	Output of the day in range [1,31]
	 */
	void getEasterSunday(int year);
	
	/**
	 * \brief Relative day from Easter within the year
	 */
	int getDayFromEaster(int year,int month,int day);
	
	/**
	 * \brief Check is the date within Easter Season
	 * \param month	Month with the range [1,12]
	 * \param day	Day with the range [1,31]
	 */
	bool isEasterSeason(int year,int month,int day);
	
	bool isTodayEasterSeason();
	
};

//class AcEaster {
//public:
//	AcEaster();
//	
//	/**
//	 * \brief Get Easter Sunday base on the year
//	 *
//	 * From http://aa.usno.navy.mil/faq/docs/easter.php
//	 * \param month	 Output of the month in range [1,12]
//	 * \param day	Output of the day in range [1,31]
//	 */
//	void getEasterSunday(int year,int* month,int* day);
//	
//	/**
//	 * \brief Relative day from Easter within the year
//	 */
//	int getDayFromEaster(int year,int month,int day);
//	
//	/**
//	 * \brief Check is the date within Easter Season
//	 * \param month	Month with the range [1,12]
//	 * \param day	Day with the range [1,31]
//	 */
//	bool isEasterSeason(int year,int month,int day);
//	
//	bool isTodayEasterSeason();
//	
//private:
//	/**
//	 * \brief Get days between two date
//	 *
//	 * Based on Java version
//	 * @return Value is positive if day1 < day2, else negative
//	 */
//	int daysBetween(struct tm* d1, struct tm* d2);
//
//	/**
//	 * \brief Create the day based on the struct tm
//	 */
//	struct tm createDay(int year,int month,int day);
//	
//	/**
//	 * @brief Return today in struct tm
//	 */
//	struct tm getTmToday();
//};

#endif