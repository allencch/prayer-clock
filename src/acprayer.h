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
 * \file acprayer.h
 * \brief Prayer
 *
 * \author Allen Choong
 * \date 2010-03-30
 * \version 0.0.6
 */

#ifndef AC_PRAYER_H
#define AC_PRAYER_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#ifndef Expat_INCLUDED
#include "expat.h"
#endif


class AcPrayer;

typedef std::vector<AcPrayer*> AcPrayerList;

/**
 * \brief Prayers class
 */
class AcPrayers {
public:
	AcPrayers(const char* filename);
	virtual ~AcPrayers();
	
	AcPrayerList list;
	static void XMLCALL startListElement(void *userData, const char *name, const char **atts);
	static void XMLCALL endListElement(void *userData, const char *name);
	static void XMLCALL characterListHandler(void* userData,const XML_Char* s,int len);
};


/**
 * \brief Prayer struct
 */
class AcPrayer {
public:
	AcPrayer();
	string title;
	vector<string> time;
	string text;
	bool easter_f; /**< Has Easter if true */
	int easter; /**< Calcualation the day from/to Easter (For Divine Mercy Novena) */
	
	friend class AcPrayers;
	
private:
	string element; /**< Record the element while parsing XML */
};

#endif