#ifndef __ELF_64_UTIL_TIME__
#define __ELF_64_UTIL_TIME__

#include <sys/time.h>
#include <string>

class Time {
public:
    Time ();
    Time (const Time& othertime);
    Time (const std::string& timestamp);
    Time (const struct timeval& tv);
    Time( const long& usecs);
    Time& operator=(const Time& rhs);
    Time& operator=(const std::string& timestamp);
    Time& operator=(const struct timeval& tv);
    Time& operator=(const long& usecs);

    Time& SetNow();

    // Components
    int Year()   const { MakeReady(); return time.tm_year + 1900;}
    int Month()  const { MakeReady(); return time.tm_mon + 1;}
    int MDay()   const { MakeReady(); return time.tm_mday;}
    int Hour()   const { MakeReady(); return time.tm_hour;}
    int Minute() const { MakeReady(); return time.tm_min;}
    int Second() const { MakeReady(); return time.tm_sec;}
    int MSec()   const { return tv.tv_usec / 1000;}
    int USec()   const { return tv.tv_usec;}

    // Diffs: Time since rhs: (this - rhs)
    int  DiffSecs (const Time& rhs) const;
    long DiffUSecs (const Time& rhs) const;

    // micro-seconds since the epoch
    long EpochUSecs() const;

    int EpochSecs() const;

    std::string Timestamp() const;
    std::string FileTimestamp() const;
private:
    void MakeReady() const;
    void SetTmFromTimeval() const;
    struct timeval tv;
    mutable struct tm time;
    bool   ready;
};

#endif
