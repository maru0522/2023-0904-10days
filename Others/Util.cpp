#include "Util.h"
#include <chrono>
#include <cmath>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std::chrono;

template<class TimeUnit>
inline int32_t Util::Timer::GetNowCount(void)
{
    // templateの型は[時間間隔をを表す型]でなければならない。
    // ref: https://cpprefjp.github.io/reference/chrono/duration_aliases.html

    return (int32_t)duration_cast<TimeUnit>(steady_clock::now().time_since_epoch()).count();
}

std::string Util::Timer::GetNowDateTime(void)
{
    std::time_t now = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &now);

    std::stringstream ss;
    ss << 1900 + tm.tm_year << ":" << std::setfill('0') << std::setw(2) << tm.tm_mon + 1 << ":" << tm.tm_mday << " "
        << std::setfill('0') << std::setw(2) << tm.tm_hour << ":" << std::setfill('0') << std::setw(2) << tm.tm_min
        << ":" << std::setfill('0') << std::setw(2) << tm.tm_sec;
    return ss.str();
}

void Util::Timer::Start(float endTime)
{
    startTime_ = GetNowCount<milliseconds>();
    endTime_ = endTime;
}

std::string Util::Convert::ToString(const std::wstring& wStr)
{
    int32_t necessarySize{ WideCharToMultiByte(CP_UTF8, 0, &wStr[0], (int32_t)wStr.size(), NULL, 0, NULL, NULL) };
    std::string toStr(necessarySize, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wStr[0], (int32_t)wStr.size(), &toStr[0], necessarySize, NULL, NULL);
    return toStr;
}

std::string Util::Convert::ToString(const char* cStr)
{
    int32_t necessarySize{ MultiByteToWideChar(CP_UTF8, 0, cStr, -1, NULL, 0) };
    std::wstring wStr(necessarySize, 0);
    MultiByteToWideChar(CP_UTF8, 0, cStr, -1, &wStr[0], necessarySize);
    int32_t utf8Size{ WideCharToMultiByte(CP_UTF8, 0, &wStr[0], -1, NULL, 0, NULL, NULL) };
    std::string toStr(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wStr[0], -1, &toStr[0], utf8Size, NULL, NULL);
    return toStr;
}


std::wstring Util::Convert::ToWString(const std::string& str)
{
    int32_t necessarySize{ MultiByteToWideChar(CP_UTF8, 0, &str[0], (int32_t)str.size(), NULL, 0) };
    std::wstring toWStr(necessarySize, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int32_t)str.size(), &toWStr[0], necessarySize);
    return toWStr;
}

std::wstring Util::Convert::ToWString(const char* cStr)
{
    int32_t necessarySize{ MultiByteToWideChar(CP_UTF8, 0, cStr, -1, NULL, 0) };
    std::wstring toWStr(necessarySize, 0);
    MultiByteToWideChar(CP_UTF8, 0, cStr, -1, &toWStr[0], necessarySize);
    return toWStr;
}


