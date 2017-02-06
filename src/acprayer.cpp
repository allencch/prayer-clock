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

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "expat.h"

using namespace std;

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif


#include "acprayer.h"

AcPrayer::AcPrayer() {
	easter_f = false;
}

void AcPrayers::startListElement(void *userData, const char *name, const char **atts) {
	AcPrayerList* pl = (AcPrayerList*)userData;
	
	
	if(strcmp(name,"prayer") == 0) {
		AcPrayer* pr = new AcPrayer;
		pl->push_back(pr);	
	}
	else if(pl->size()) {
		pl->back()->element = name;
	}

	
	for(int i=0;atts[i];i+=2) {
		if(strcmp(atts[i],"title") == 0) {
			pl->back()->title = atts[i+1];
		}
		else if(strcmp(atts[i],"easter") == 0) {
			pl->back()->easter_f = true;
			pl->back()->easter = atoi(atts[i+1]);
		}
	}
}

void AcPrayers::endListElement(void *userData, const char *name) {
	AcPrayerList* pl = (AcPrayerList*)userData;
	pl->back()->element = "";
}

void AcPrayers::characterListHandler(void* userData,const XML_Char* s,int len) {
	AcPrayerList* pl = (AcPrayerList*)userData;
	
	if(pl->size() && pl->back()->element == "time") {
		string t(s,len);
		pl->back()->time.push_back(t);
	}
	else if(pl->size() && pl->back()->element =="text") {
		string t(s,len);
		pl->back()->text += t;
	}
}


AcPrayers::AcPrayers(const char* filename) {
	char buf[8192];
	XML_Parser parser = XML_ParserCreate(NULL);
	int done;

	XML_SetUserData(parser, &list);
	XML_SetElementHandler(parser, startListElement, endListElement);
	XML_SetCharacterDataHandler(parser,characterListHandler);
	
	FILE* file = fopen(filename,"rb");
	if(!file)
		fprintf(stderr,"Failed open file\n");
	
	do {
		int len = (int)fread(buf, 1, sizeof(buf), file);
		done = len < sizeof(buf);
		if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
			fprintf(stderr,
				"%s at line %" XML_FMT_INT_MOD "u\n",
				XML_ErrorString(XML_GetErrorCode(parser)),
				XML_GetCurrentLineNumber(parser));
			return;
		}
	} while (!done);
	XML_ParserFree(parser);
	fclose(file);
}

AcPrayers::~AcPrayers() {
	for(int i=0;i<list.size();i++) {
		delete list[i];
	}
}