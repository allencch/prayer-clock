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

#include <ctime>
#include <cstring>
#include <cstdio>
#include <iostream>

#include "easter.h"

using namespace std;

DateTime::DateTime() {
    getToday();
}

DateTime::DateTime(int year,int month,int day) {
    setDate(year,month,day);
}

void DateTime::getToday() {
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo;
    timeinfo = localtime(&rawtime);

    setDate(timeinfo->tm_year+1900,timeinfo->tm_mon + 1,timeinfo->tm_mday);
}

string DateTime::toString() {
    char today[50];
    strftime(today,19,"%d %B, %Y",&_tm);
    return string(today);
}

void DateTime::setDate(int year,int month,int day) {
    struct tm d;
    memset(&d,0,sizeof(struct tm));
    d.tm_year = year - 1900;
    d.tm_mon = month - 1;
    d.tm_mday = day;

    this->day = day;
    this->month = month;
    this->year = year;

    _tm = d;
}

int DateTime::compare(DateTime& dt) {
    time_t t1 = mktime(&_tm);

    struct tm temp = dt._tm;
    //temp.tm_hour += 1;
    time_t t2 = mktime(&temp);

    return (int)(difftime(t2,t1)/AC_ONE_DAY);
}



Easter::Easter() {}

void Easter::getEasterSunday(int year) {
    int month, day;
    int y,m,d,c,n,k,i,j,l;

    y = year;

    c = y / 100;
    n = y - 19 * ( y / 19 );
    k = ( c - 17 ) / 25;
    i = c - c / 4 - ( c - k ) / 3 + 19 * n + 15;
    i = i - 30 * ( i / 30 );
    i = i - ( i / 28 ) * ( 1 - ( i / 28 ) * ( 29 / ( i + 1 ) )
        * ( ( 21 - n ) / 11 ) );
    j = y + y / 4 + i + 2 - c + c / 4;
    j = j - 7 * ( j / 7 );
    l = i - j;
    m = 3 + ( l + 40 ) / 44;
    d = l + 28 - 31 * ( m / 4 );

    day = d;
    month = m;

    setDate(year,month,day);
}

int Easter::getDayFromEaster(int year,int month,int day) {
    //Get Easter Sunday
    getEasterSunday(year);

    //Pentecost
    /*int pm,pd;
    pm = this->month;
    pd = this->day + 49; //Do not include that day
    if(pm /4) { //April
        if(pd > 30) {
            pm+=1;
            pd-=30;
        }
    }
    else { //March
        if(pd > 31) {
            pm+=1;
            pd-=30;
        }
    }//*/

    DateTime target(year,month,day);

    return compare(target);
}

bool Easter::isEasterSeason(int year,int month,int day) {
    int compare = getDayFromEaster(year,month,day);

    if(compare>=0 && compare<49) {
        return true;
    }

    return false;
}

bool Easter::isTodayEasterSeason() {
    DateTime today;

    return isEasterSeason(today.year,today.month,today.day);
}
