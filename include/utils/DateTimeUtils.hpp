#pragma once

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

#include "UtilityCommon.hpp"

/**
 * @brief This embedded class contains utilities in support of DateTimeUtils
 * Although the DRY is violated, this class can stay dependency free and be used
 * freely without needing external support
 *
 */
class DateTimeUtils_EmbeddedUtils
{

public:
    /**
    * @brief Converts string to upper-case
    * @param Reference to the string to convert
    */
    static void toUpperInPlace(std::string &str)
    {
        size_t count=str.size();
        for (size_t a1 = 0;a1<count;++a1)
        {
            str[a1] = static_cast<char>(toupper(str[a1]));
        }
    }


    /**
    * @brief Converts string to upper-case
    * @param The string to convert
    * @return The converted string
    */
    static std::string toUpper(const std::string &str)
    {
        std::string strOut = str;
        toUpperInPlace(strOut);
        return strOut;
    }


    /**
     * @brief Perfroms string replace (in-place)
     * @param inputString The string that contains oldString to replace
     * @param oldString The old string to replace
     * @param newString The new string to insert
     * @param isCaseSensitive Should oldString be caseSensitive
     * within inputString?
     * @return inputString with the replaced oldString.  If oldString
     * is not found within inputString then inputString is returned
     * unmodified.
     */
    static std::string replace(const std::string &inputString,
                                     const std::string &oldString,
                                     const std::string &newString,
                                     bool isCaseSensitive = c_DEFAULT_CASE_SENSITIVE)
    {

        if (isCaseSensitive)
        {
            std::string subject = inputString;
            std::string search  = oldString;
            std::string replace = newString;

                size_t pos = 0;
                while((pos = subject.find(search, pos)) != std::string::npos)
                {
                     subject.replace(pos, search.length(), replace);
                     pos += replace.length();
                }
                return subject;
        }
        else
        {
            std::string subject = (inputString);
            std::string search  = toUpper(oldString);
            std::string replace = toUpper(newString);

                size_t pos = 0;
                while((pos = subject.find(search, pos)) != std::string::npos)
                {
                     subject.replace(pos, search.length(), replace);
                     pos += replace.length();
                }
                return subject;
        }
    }


    /**
     * @brief contains Checks to see if subStr exists within srcStr
     * @param srcStr The original source string
     * @param subStr The substring to look for
     * @param caseSensitive
     * @return Return true if subStr exists within srcStr, false otherwise
     */
    static bool contains(const std::string &srcStr,
                         const std::string &subStr,
                         bool caseSensitive = c_DEFAULT_CASE_SENSITIVE)
    {
        if (caseSensitive)
        {
            if (srcStr.find(subStr) != std::string::npos)
            {
                return true;
            }
        }
        else
        {
            if (toUpper(srcStr).find(toUpper(subStr)) !=
                    std::string::npos)
            {
                return true;
            }
        }
        return false;
    }


    /**
     * @brief toNumber Converts string value to numeric type
     * @param value The string to convert
     * @return Returns numeric type of conversion
     */
    template<typename T>
    static T toNumber(const std::string & value)
    {
        std::istringstream in(value,std::istringstream::in);
        T returnValue;
        in>>returnValue;
        return returnValue;
    }
};

/**
 * @brief The DateEntity class represents a single date object
 * for use in DateTimeUtils and DateUtils
 */
class CBTEK_UTILITY_DLL DateEntity
{

public:
    /**
     * @brief DateEntity (Constructor) Default
     */
    DateEntity()
    {
        m_month = 1;
        m_day = 1;
        m_year = 1970;
    }

    /**
     * @brief DateEntity (Constructor) Send in integer components
     * for month, day and year
     * @param month The month to set
     * @param day The day to set
     * @param year The year to set
     */
    DateEntity(const size_t &month, const size_t &day, const size_t &year)
    {
        m_month = month;
        m_day = day;
        m_year = year;
    }

    /**
     * @brief DateEntity (Constructor) Send in integer containing date components in
     * form of YYYYMMDD
     * @param dateInteger The date components to set
     */
    DateEntity(const size_t &dateInteger)
    {
        std::string dateStr = std::to_string(dateInteger);
        if (dateStr.size() ==  8)
        {
            m_year = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(dateStr.substr(0,4));
            m_month = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(dateStr.substr(4,2));
            m_day = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(dateStr.substr(6,2));
        }
        else
        {
            m_month = 1;
            m_day = 1;
            m_year = 1970;
        }
    }

    /**
     * @brief getMonth Gets the month value
     * @return Returns the month value
     */
    size_t getMonth() const
    {
        return m_month;
    }

    /**
     * @brief getDay Gets the day value
     * @return Returns the day value
     */
    size_t getDay() const
    {
        return m_day;
    }

    /**
     * @brief getYear Gets the year value
     * @return Returns the year value
     */
    size_t getYear() const
    {
        return m_year;
    }

    /**
     * @brief setMonth Sets the month value
     * @param month The month to set
     */
    void setMonth(const size_t &month)
    {
        m_month = month;
    }

    /**
     * @brief setDay Sets the day value
     * @param day The day to set
     */
    void setDay(const size_t &day)
    {
        m_day = day;
    }

    /**
     * @brief setYear Sets the year value
     * @param year The year to set
     */
    void setYear(const size_t &year)
    {
        m_year = year;
    }

    /**
     * @brief toDateInteger Converts date object into unique integer
     * @return Return date as integer YYYYMMDD
     */
    size_t toDateInteger() const
    {
        std::string dateStr = std::to_string(m_year)+
                (m_month<10?"0"+std::to_string(m_month):std::to_string(m_month))+
                (m_day<10?"0"+std::to_string(m_day):std::to_string(m_day));
        return DateTimeUtils_EmbeddedUtils::toNumber<size_t>(dateStr);
    }

    /**
     * @brief operator< Checks if date is less than or equal to (*this)
     * @param date The date to compare against
     * @return True if (*this) is less than date, false otherwise
     */
    bool operator <(const DateEntity &date) const
    {
       return toDateInteger() < date.toDateInteger();
    }

    /**
     * @brief operator<= Checks if date is less than or equal to (*this)
     * @param date The date to compare against
     * @return True if (*this) is less than or equal to date, false otherwise
     */
    bool operator<= (const DateEntity &date) const
    {
        return toDateInteger()<=  date.toDateInteger();
    }

    /**
     * @brief operator> Checks if date is greater than to (*this)
     * @param date The date to compare against
     * @return True if (*this) is greater than to date, false otherwise
     */
    bool operator >(const DateEntity &date) const
    {
        return toDateInteger()>date.toDateInteger();
    }

    /**
     * @brief operator>= Checks if date is greater than or equal to (*this)
     * @param date The date to compare against
     * @return True if (*this) is greater than or equal to date, false otherwise
     */
    bool operator>= (const DateEntity &date) const
    {
        return toDateInteger()>= date.toDateInteger();
    }

    /**
     * @brief operator!= Checks if date is equal to (*this)
     * @param date The date to compare against
     * @return True if dates are equal, false otherwise
     */
    bool operator  ==  (const DateEntity &date) const
    {
        return toDateInteger() ==  date.toDateInteger();
    }

    /**
     * @brief operator!= Checks if date is not equal to (*this)
     * @param date The date to compare against
     * @return True if dates are not equal, false otherwise
     */
    bool operator!= (const DateEntity &date) const
    {
        return toDateInteger()!= date.toDateInteger();
    }

    /**
     * @brief operator- Get number of days between two dates
     * @param date The second date to subtract from
     * @return Total number of days between this->m_day and date.getDay()
     */
    size_t operator-(const DateEntity &date) const
    {

        size_t lowYear = 0,highYear = 0;
        size_t days = 0;
        if (m_year<date.getYear())
        {
            lowYear = m_year;
            highYear = date.getYear();
        }

        else if (m_year>date.getYear())
        {
            lowYear = date.getYear();
            highYear = m_year;
        }

        for (size_t a1 = lowYear;a1<highYear;++a1)
        {
            days+= 365;
            if (DateEntity(1,1,a1).isLeapYear())
            {
                days++;
            }
        }
        DateEntity d1(date.getMonth(),date.getDay(),1901);
        DateEntity d2(getMonth(),getDay(),1901);
        size_t d1Days = d1.getDays();
        size_t d2Days = d2.getDays();
        if (d1Days>d2Days)
        {
            days +=  d1Days-d2Days;
        }
        else
        {
            days +=  d2Days-d1Days;
        }

        return days;
    }

    /**
     * @brief isLeapYear Determine if current year is leap year
     * @return Returns true if leap year, false otherwise
     */
    bool isLeapYear() const
    {
    //    The year is evenly divisible by 4;
    //    If the year can be evenly divided by 100, it is NOT a leap year, unless;
    //    The year is also evenly divisible by 400. Then it is a leap year.
        return ((m_year % 4  ==  0 && m_year % 100 !=  0) || m_year % 400  ==   0);
    }

    /**
     * @brief getDays() Gets number of days since beginning of year
     * @return Total number of days since beginning of year
     */
    size_t getDays() const
    {
        size_t daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        size_t days = 0;
        for (size_t a1 = 0;a1<m_month-1;++a1)
        {
            if (a1 < 12)
            {
                if(a1 ==  1 && isLeapYear())
                {
                    days+= daysInMonth[a1]+1;
                }
                else
                {
                    days+= daysInMonth[a1];
                }
            }
        }
        return days+(m_day);
    }

private:
    size_t m_month;
    size_t m_day;
    size_t m_year;
};

/**
 * @brief The TimeEntity class represents a single time object
 * for use in DateTimeUtils and TimeUtils
 */
class CBTEK_UTILITY_DLL TimeEntity
{
public:

    /**
     * @brief TimeEntity (Constructor) Default
     */
    TimeEntity()
    {
        m_hour = 0;
        m_minute = 0;
        m_second = 0;
        m_millisecond = 0;
    }

    /**
     * @brief TimeEntity (Constructor) Create time object with integer
     * containing time components in the form of HHMMSS
     * @param time Integer containing HHMMSS components
     */
    TimeEntity(const size_t &time)
    {

        std::string timeStr = std::to_string(time);
        if (timeStr.size() == 6)
        {
            m_hour = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(timeStr.substr(0,2));
            m_minute = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(timeStr.substr(2,2));
            m_second = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(timeStr.substr(4,2));
        }
        else if (timeStr.size() ==  5)
        {
            m_hour = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(timeStr.substr(0,1));
            m_minute = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(timeStr.substr(1,2));
            m_second = DateTimeUtils_EmbeddedUtils::toNumber<size_t>(timeStr.substr(3,2));
        }
        else
        {
            m_hour = 0;
            m_minute = 0;
            m_second = 0;

        }
        m_millisecond = 0;
    }

    /**
     * @brief TimeEntity (Constructor) Creates TimeEntity object
     * with hour, minute, second and optional millisecond components
     * @param hour The hour to set
     * @param minute The minute to set
     * @param second The second to set
     * @param millisecond The millisecond to set
     */
    TimeEntity(size_t hour,
               size_t minute,
               size_t second,
               size_t millisecond = 0)
    {
        m_hour = hour;
        m_minute = minute;
        m_second = second;
        m_millisecond = millisecond;
    }

    /**
     * @brief operator> Checks if the time object
     * is greater than the (*this) object
     * @param time The time object to compare against
     * @return True if (*this) is greater than time, false otherwise
     */
    bool operator>(const TimeEntity &time) const
    {
        return (this->getAsSeconds() > time.getAsSeconds());
    }


    /**
     * @brief operator< Checks if the time object
     * is less than the (*this) object
     * @param time The time object to compare against
     * @return True if (*this) is less than time, false otherwise
     */
    bool operator<(const TimeEntity &time) const
    {
        return (this->getAsSeconds() < time.getAsSeconds());
    }

    /**
     * @brief operator<= Checks if the time object
     * is less than or equal to (*this) object
     * @param time The time object to compare against
     * @return True if (*this) is less than or equal to time,
     * false otherwise
     */
    bool operator<= (const TimeEntity &time) const
    {
        return (this->getAsSeconds() <=  time.getAsSeconds());
    }

    /**
     * @brief operator>= Checks if the time object
     * is greater than or equal to (*this) object
     * @param time The time object to compare against
     * @return True if (*this) is greater than or equal to time,
     * false otherwise
     */
    bool operator>= (const TimeEntity &time) const
    {
        return (this->getAsSeconds() >=  time.getAsSeconds());
    }

    /**
     * @brief operator== Checks if the time object
     * is equal to (*this) object
     * @param time The time object to compare against
     * @return True if (*this) is equal to time, false otherwise
     */
    bool operator==  (const TimeEntity &time) const
    {
        return (time.getHour() ==  m_hour && time.getMinute() ==  m_minute && time.getSecond() ==  m_second && time.getMillisecond() ==  m_millisecond);
    }

    /**
     * @brief setHour Sets the hour value
     * @param hour The hour to set
     */
    void setHour(const size_t &hour)
    {
        m_hour = hour;
    }

    /**
     * @brief setMinute Sets the minute value
     * @param minute The minute to set
     */
    void setMinute(const size_t &minute)
    {
        m_minute = minute;
    }

    /**
     * @brief setSecond Sets the second value
     * @param second The second to set
     */
    void setSecond(const size_t &second)
    {
        m_second = second;
    }

    /**
     * @brief setMillisecond Sets the millisecond value
     * @param millisecond The millisecond to set
     */
    void setMillisecond(const size_t &millisecond)
    {
        m_millisecond = millisecond;
    }

    /**
     * @brief getHour Get the hour value
     * @return Value of m_hour
     */
    size_t getHour() const
    {
        return m_hour;
    }

    /**
     * @brief getMinute Get the minute value
     * @return Value of m_minute
     */
    size_t getMinute() const
    {
        return m_minute;
    }

    /**
     * @brief getSecond Get the second value
     * @return Value of m_second
     */
    size_t getSecond() const
    {
        return m_second;
    }

    /**
     * @brief getMillisecond Get the millisecond value
     * @return Value of m_millisecond
     */
    size_t getMillisecond() const
    {
        return m_millisecond;
    }

    /**
     * @brief toTimeInteger Returns integer of (*this) time value
     * in form of HHMMSS integer
     * @return Return integer of current time components
     */
    size_t toTimeInteger() const
    {
        std::string hourStr = std::to_string(m_hour);
        std::string minStr = std::to_string(m_minute);
        std::string secStr = std::to_string(m_second);
        std::string timeStr = (m_hour<10?"0"+hourStr:hourStr) + (m_minute<10?"0"+minStr:minStr) + (m_second<10?"0"+secStr:secStr);
        return DateTimeUtils_EmbeddedUtils::toNumber<size_t>(timeStr);
    }

    /**
     * @brief getAsMinutes Get total number of minutes
     * @return  Return integer of total number of minutes
     */
    size_t getAsMinutes() const
    {
        return (m_hour*60)+m_minute;
    }

    /**
     * @brief getAsSeconds Get total number of seconds
     * @return Return integer of total number of seconds
     */
    size_t getAsSeconds() const
    {
        return (getAsMinutes()*60)+m_second;
    }

    /**
     * @brief getAsMilliseconds Get total number of milliseconds
     * @return Return integer of total number of milliseconds
     */
    size_t getAsMilliseconds() const
    {
        return (getAsSeconds()*1000)+m_millisecond;
    }

    /**
     * @brief getAsMicroseconds Get total number of microseconds
     * @return Return integer of total number of microseconds
     */
    size_t getAsMicroseconds() const
    {
        return (getAsMilliseconds()*1000);
    }

private:
    size_t m_hour;
    size_t m_minute;
    size_t m_second;
    size_t m_millisecond;
};


/**
 * @brief Defining some typdefs for commonly used types
 */
typedef std::chrono::system_clock SystemClock;
typedef std::chrono::high_resolution_clock HighResolutionClock;
typedef HighResolutionClock::time_point HighResolutionTimePoint;
typedef SystemClock::time_point SystemTimePoint;
typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::microseconds Microseconds;
typedef std::chrono::nanoseconds Nanoseconds;
typedef std::chrono::seconds Seconds;

/**
 * @brief The TimeUtils class contains useful functions for dealing
 * the TimeEntity class
 */
class CBTEK_UTILITY_DLL TimeUtils
{

public:

    /**
     * @brief tick
     * @return
     */
    static double tick()
    {
        return  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    /**
     * @brief tock
     * @param tick_value
     * @return
     */
    static double tock(double tick_value)
    {
        return (tick() - tick_value);
    }

    /**
     * @brief toString
     * @param value
     * @return
     */
    template <typename T>
    static std::string toString(T value)
    {
        std::ostringstream out;
        out << value;
        return out.str();
    }

    /**
     * @brief getSecondsNow
     * @return
     */
   static uint64_t getSecondsNow()
    {
        return static_cast<std::uint64_t>(time(NULL));
    }

    /**
     * @brief getNanosecondsNow
     * @return
     */
    static double getNanosecondsNow()
    {
        HighResolutionTimePoint tp = HighResolutionClock::now();
        Seconds secs= std::chrono::duration_cast<Seconds>(tp.time_since_epoch());
        return static_cast<double>(secs.count());
    }

    /**
     * @brief getMicrosecondsNow
     * @return
     */
    static double getMicrosecondsNow()
    {
        HighResolutionTimePoint tp = HighResolutionClock::now();
        Microseconds secs= std::chrono::duration_cast<Microseconds>(tp.time_since_epoch());
        return static_cast<double> (secs.count());
    }

    /**
     * @brief getCurrentMilliseconds
     * @return
     */
    static std::uint64_t getCurrentMilliseconds()
    {
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch());
        return static_cast<std::uint64_t>(ms.count());
    }

    /**
     * @brief getMillisecondsNow
     * @return
     */
    static double getMillisecondsNow()
    {
        HighResolutionTimePoint tp = HighResolutionClock::now();
        Milliseconds secs= std::chrono::duration_cast<Milliseconds>(tp.time_since_epoch());
        return static_cast<double>(secs.count());
    }

    /**
     * @brief toTimeString
     * @param time
     * @return
     */
    static std::string toTimeString(const TimeEntity &time)
    {
        return (time.getHour()<10?"0":"")+toString(time.getHour())+":"+(time.getMinute()<10?"0":"")+toString(time.getMinute())+":"+(time.getSecond()<10?"0":"")+toString(time.getSecond());
    }

    /**
     * @brief to12HourTimeString
     * @param time
     * @return
     */
    static std::string to12HourTimeString(const TimeEntity &time)
    {
        std::string extension="AM";
        size_t hour=time.getHour();
        if (time.getHour()>11 )
        {
            extension="PM";
            if (time.getHour()!=12)
            {
                hour=time.getHour()-12;
            }
        }

        if (hour==0)
        {
            hour=12;
        }

        return (hour<10?"0":"")+toString(hour)+":"+(time.getMinute()<10?"0":"")+toString(time.getMinute())+":"+(time.getSecond()<10?"0":"")+toString(time.getSecond())+ " "+extension;
    }

    /**
     * @brief getCurrentTime
     * @return
     */
    static TimeEntity getCurrentTime()
    {
        time_t t = time(0);
        struct tm * now;
        #ifdef __STDC_WANT_LIB_EXT1__
                localtime_s(now, &t);
        #else
			#if (_MSC_VER >= 1900) 
				localtime_s(now, &t);
			#else
				now = localtime(&t);
			#endif                
        #endif

        return TimeEntity(static_cast<size_t>(now->tm_hour),
                          static_cast<size_t>(now->tm_min),
                          static_cast<size_t>(now->tm_sec));

    }

    /**
     * @brief getTimeFromSeconds
     * @param secs
     * @return
     */
    static TimeEntity getTimeFromSeconds(const uint64_t &secs)
    {
        TimeEntity tm;
        tm.setHour(static_cast<size_t>(secs/3600));
        tm.setMinute(static_cast<size_t>(((secs%3600)/60)));
        tm.setSecond(static_cast<size_t>(((secs%3600)%60)));
        return tm;
    }

    /**
     * @brief getTimeFromMilliseconds
     * @param ms
     * @return
     */
    static TimeEntity getTimeFromMilliseconds(const uint64_t &ms)
    {
        return getTimeFromSeconds(ms/1000);
    }

    /**
     * @brief toCurrentTimeString
     * @return
     */
    static std::string toCurrentTimeString()
    {
        return toTimeString(getCurrentTime());
    }

    /**
     * @brief toCurrent12HourTimeString
     * @return
     */
    static std::string toCurrent12HourTimeString()
    {
        return to12HourTimeString(getCurrentTime());
    }

    /**
     * @brief toHMSDisplayString
     * @param ms
     * @return
     */
    static std::string toHMSDisplayString(uint64_t milliseconds)
    {
        int seconds = (int) (milliseconds / 1000) % 60 ;
        int minutes = (int) ((milliseconds / (1000*60)) % 60);
        int hours   = (int) ((milliseconds / (1000*60*60)) % 24);
        std::ostringstream out;
        if (hours > 1)
        {
            out << hours << " hours, ";
        }
        else if (hours == 1)
        {
            out << "1 hour, ";
        }

        if (minutes > 1)
        {
            out << minutes << " minutes, ";
        }
        else if (minutes == 1)
        {
            out << "1 minute, ";
        }
        if (seconds > 1)
        {
            out << seconds << " seconds";
        }
        else if (seconds == 1)
        {
            out << "1 second";
        }
        else
        {
            out << milliseconds << " milliseconds";
        }
        return out.str();
    }
};

/**
 * @brief The DateUtils class
 */
class CBTEK_UTILITY_DLL DateUtils
{

private:
    DateUtils();
    DateUtils& operator=(const DateUtils&);
    DateUtils(const DateUtils&);
    ~DateUtils();

public:
    /**
     * @brief toShortDateString
     * @param date
     * @param format
     * @return
     */
    static std::string toShortDateString(const DateEntity &date,
                                         const std::string &format="mm-dd-yyyy")
    {
        std::string dateStr=format;

        if (DateTimeUtils_EmbeddedUtils::contains(dateStr,"yyyy"))
        {
            dateStr = DateTimeUtils_EmbeddedUtils::replace(dateStr,"yyyy",std::to_string(date.getYear()));
        }

        if(DateTimeUtils_EmbeddedUtils::contains(dateStr,"mm"))
        {
            dateStr=DateTimeUtils_EmbeddedUtils::replace(dateStr,"mm",(date.getMonth()<10?"0"+std::to_string(date.getMonth()):std::to_string(date.getMonth())));
        }

        if(DateTimeUtils_EmbeddedUtils::contains(dateStr,"dd"))
        {
            dateStr=DateTimeUtils_EmbeddedUtils::replace(dateStr,"dd",(date.getDay()<10?"0"+std::to_string(date.getDay()):std::to_string(date.getDay())));
        }
        return dateStr;
    }

    /**
     * @brief toLongDateString
     * @param date
     * @return
     */
    static std::string toLongDateString(const DateEntity &date)
    {
        std::string months[12] =
        {
            "January","February","March","April",
            "May","June","July","August",
            "September","October","November","December"
        };

        if (date.getMonth() <13)
        {
            return months[date.getMonth()-1]+" "+std::to_string(date.getDay())+", "+std::to_string(date.getYear());
        }
        return "";
    }

    /**
     * @brief getCurrentDate
     * @return
     */
    static DateEntity getCurrentDate()
    {
        time_t t = time(0);
        struct tm * now;
		#ifdef __STDC_WANT_LIB_EXT1__
				localtime_s(now, &t);
		#else
			#if (_MSC_VER >= 1900) 
					localtime_s(now, &t);
			#else
					now = localtime(&t);
			#endif                
		#endif
        return DateEntity(static_cast<size_t>(now->tm_mon + 1),
                          static_cast<size_t>(now->tm_mday),
                          static_cast<size_t>(now->tm_year + 1900));
    }

    /**
     * @brief toCurrentLongDateString
     * @return
     */
    static std::string toCurrentLongDateString()
    {
        return toLongDateString(getCurrentDate());
    }

    /**
     * @brief toCurrentShortDateString
     * @return
     */
    static std::string toCurrentShortDateString()
    {
        return toShortDateString(getCurrentDate());
    }

};


class CBTEK_UTILITY_DLL DateTimeUtils
{

public:
    /**
     * @brief getDisplayTimeStamp
     * @param dateEntity
     * @param timeEntity
     * @return
     */
    static std::string getDisplayTimeStamp(const DateEntity& dateEntity,
                                           const TimeEntity& timeEntity)
    {
        return (DateUtils::toShortDateString(dateEntity,"mm/dd/yyyy")+" at "+TimeUtils::to12HourTimeString(timeEntity));
    }

    /**
     * @brief getDisplayTimeStamp
     * @return
     */
    static std::string getDisplayTimeStamp()
    {
        return getDisplayTimeStamp(DateUtils::getCurrentDate(),TimeUtils::getCurrentTime());
    }

    /**
     * @brief getTimeStampInteger
     * @param dateEntity
     * @param timeEntity
     * @return
     */
    static std::uint64_t getTimeStampInteger(const DateEntity& dateEntity=DateUtils::getCurrentDate(),
                                             const TimeEntity& timeEntity=TimeUtils::getCurrentTime())
    {
        return std::strtoull(((std::to_string(dateEntity.toDateInteger()) + std::to_string(timeEntity.toTimeInteger()))).c_str(), NULL,10);
    }

    /**
     * @brief getTimeStamp
     * @return
     */
    static std::string getTimeStamp()
    {
        return (DateUtils::toShortDateString(DateUtils::getCurrentDate(),"yyyymmdd")+"T"+std::to_string(TimeUtils::getCurrentTime().toTimeInteger())+"."+std::to_string(static_cast<std::uint64_t>(TimeUtils::getMillisecondsNow())));
    }

    /**
     * @brief getTimeStamp
     * @param dateEntity
     * @param timeEntity
     * @return
     */
    static std::string getTimeStamp(const DateEntity& dateEntity,
                                    const TimeEntity& timeEntity)
    {
        return (DateUtils::toShortDateString(dateEntity,"yyyymmdd")+"T"+std::to_string(timeEntity.toTimeInteger()));
    }


};
