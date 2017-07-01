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


#include "prayer.h"

Prayer::Prayer() {
    easter_f = false;
}

void Prayers::startListElement(void *userData, const char *name, const char **atts) {
    PrayerList* pl = (PrayerList*)userData;


    if(strcmp(name,"prayer") == 0) {
        Prayer* pr = new Prayer;
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

void Prayers::endListElement(void *userData, const char *name) {
    PrayerList* pl = (PrayerList*)userData;
    pl->back()->element = "";
}

void Prayers::characterListHandler(void* userData,const XML_Char* s,int len) {
    PrayerList* pl = (PrayerList*)userData;

    if(pl->size() && pl->back()->element == "time") {
        string t(s,len);
        pl->back()->time.push_back(t);
    }
    else if(pl->size() && pl->back()->element =="text") {
        string t(s,len);
        pl->back()->text += t;
    }
}


Prayers::Prayers(const char* filename) {
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

Prayers::~Prayers() {
    for(int i=0;i<list.size();i++) {
        delete list[i];
    }
}
