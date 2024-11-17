//
// Created by gaetz on 05/11/2022.
//

#ifndef GLOG_LOG_HPP
#define GLOG_LOG_HPP

#include "Defines.hpp"
#include <sstream>
#include <fstream>
#include "LogLevel.hpp"

#include <unordered_map>
#include <functional>

using std::unordered_map;

/**
 * @brief Log class.
 * @example LOG(LogLevel::Info) \<\< "Hello World!";
 * @details This class is used to log messages to the console and to a file. It is used to log messages of different levels of gravity.
 * @attention Not thread safe for now.
 */
class Log {
public:
    GAPI Log();

    GAPI ~Log();

    const static str GAME_LOG_FILE;

    Log(const Log&) = delete;

    Log& operator=(const Log&) = delete;

    GAPI std::ostringstream& Get(LogLevel level = LogLevel::Info);

    GAPI static void Restart();

    template<class T, class U>
    GAPI static str LogMap(const str& name, const unordered_map<T, U>& map, std::function<str(T, U)> decoder) {
        std::stringstream stream;
        stream << "Map " << name << std::endl;
        for (const auto& pair: map) {
            stream << decoder(pair.first, pair.second) << std::endl;
        }
        return stream.str();
    }

private:
    std::ostringstream stream;
    LogLevel logLevel { LogLevel::Trace };
    char pad[4];

    static std::ofstream file;
    static str GetLabel(LogLevel level);
};

#ifdef GDEBUG
constexpr i32 MAX_LOG_LEVEL = static_cast<i32>(LogLevel::Trace);
#else
constexpr i32 MAX_LOG_LEVEL = static_cast<i32>(LogLevel::Error);
#endif

#define LOG(level)                                  \
    if constexpr (static_cast<i32>(level) > MAX_LOG_LEVEL)    \
        ;                                           \
    else                                            \
        Log().Get(level)


#endif //GLOG_LOG_HPP
