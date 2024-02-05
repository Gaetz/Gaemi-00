//
// Created by gaetz on 06/11/2022.
//

#ifndef GLOG_LOGLEVEL_HPP
#define GLOG_LOGLEVEL_HPP

/**
 * @brief Log type and level of gravity
 * @details This enum class is used to define the level of gravity of a log message. It is used to filter the log messages.
 */
enum class LogLevel {
    Fatal = 0,
    Error = 1,
    Warning = 2,
    Info = 3,
    Debug = 4,
    Trace = 5
};

#endif //GLOG_LOGLEVEL_HPP
