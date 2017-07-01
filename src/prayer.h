#ifndef PRAYER_H
#define PRAYER_H

#include <iostream>
#include <vector>
#include <string>

#ifndef Expat_INCLUDED
#include "expat.h"
#endif

using namespace std;

class Prayer;

typedef std::vector<Prayer*> PrayerList;

/**
 * \brief Prayers class
 */
class Prayers {
public:
    Prayers(const char* filename);
    virtual ~Prayers();

    PrayerList list;
    static void XMLCALL startListElement(void *userData, const char *name, const char **atts);
    static void XMLCALL endListElement(void *userData, const char *name);
    static void XMLCALL characterListHandler(void* userData,const XML_Char* s,int len);
};


/**
 * \brief Prayer struct
 */
class Prayer {
public:
    Prayer();
    string title;
    vector<string> time;
    string text;
    bool easter_f; /**< Has Easter if true */
    int easter; /**< Calcualation the day from/to Easter (For Divine Mercy Novena) */

    friend class Prayers;

private:
    string element; /**< Record the element while parsing XML */
};

#endif
