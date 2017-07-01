#include <string>
using namespace std;

#ifndef EASTER_H
#define EASTER_H

static long AC_ONE_DAY = 24 * 60 * 60; //24 hours * 60 minutes * 60 seconds

/**
 * \brief Date time using ctime
 */
class DateTime {
public:
    /**
     * \brief Will get today
     */
    DateTime();

    DateTime(int year,int month,int day);

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
    int compare(DateTime& dt);

    int day,month,year;

protected:
    struct tm _tm;
};


class Easter : public DateTime {
public:
    Easter();

    /**
     * \brief Get Easter Sunday base on the year
     *
     * From http://aa.usno.navy.mil/faq/docs/easter.php
     * \param month      Output of the month in range [1,12]
     * \param day   Output of the day in range [1,31]
     */
    void getEasterSunday(int year);

    /**
     * \brief Relative day from Easter within the year
     */
    int getDayFromEaster(int year,int month,int day);

    /**
     * \brief Check is the date within Easter Season
     * \param month     Month with the range [1,12]
     * \param day   Day with the range [1,31]
     */
    bool isEasterSeason(int year,int month,int day);

    bool isTodayEasterSeason();

};

#endif
