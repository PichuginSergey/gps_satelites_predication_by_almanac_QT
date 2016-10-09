#include "sv_time.h"

/* Conversion from UTC to GPS time */
void utc_to_gps(const Time& t, int *wk, double *sec) {

	const unsigned int doy[12] ={ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	const int mounth = t.get_month();
	const int ye = (t.get_year())-1980;
	int lpdays = ye/4 + 1;
	if ( ((ye%4) == 0) && ((mounth) <= 2) ) --lpdays;
	int de = ye*365 + doy[(mounth)-1] + t.get_day() + lpdays - 6;
	/* GPS weeks */
    *wk = de / Time::NUM_DAYS_IN_WEEK;
	/* GPS seconds */
    *sec = (de%Time::NUM_DAYS_IN_WEEK)*Time::SECONDS_IN_DAY + (t.get_hour())*Time::SECONDS_IN_HOUR +
            (t.get_min())*Time::SECONDS_IN_MINUTE + (t.get_sec());

    /* GPS seconds must be in the range 0-SECONDS_IN_WEEK */
	while(*sec<0.0){
		*wk -= 1;
        *sec += Time::SECONDS_IN_WEEK;
	}
    while(*sec>=Time::SECONDS_IN_WEEK){
		*wk += 1;
        *sec -= Time::SECONDS_IN_WEEK;
	}	
}
