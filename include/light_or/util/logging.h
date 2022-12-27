/*
 * @Author: dubing.xiang
 * @Date: 2022-12-06 17:21:20
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-12-06 17:21
 * @Description:
 */

#ifndef LIGHT_OR_LOGGING_H_
#define LIGHT_OR_LOGGING_H_
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

namespace light_or::util {
constexpr const char* TraceLevel() {
  return "trace";
}
constexpr const char* DebugLevel() {
  return "debug";
}
constexpr const char* InfoLevel() {
  return "info";
}
constexpr const char* WarnLevel() {
  return "warn";
}
constexpr const char* ErrLevel() {
  return "err";
}
constexpr const char* CriticalLevel() {
  return "critical";
}
constexpr const char* OffLevel() {
  return "off";
}

inline spdlog::level::level_enum LogLevel(const std::string& name) {
  if (name == TraceLevel()) {
    return spdlog::level::trace;
  }
  if (name == DebugLevel()) {
    return spdlog::level::debug;
  }
  if (name == InfoLevel()) {
    return spdlog::level::info;
  }
  if (name == WarnLevel()) {
    return spdlog::level::warn;
  }
  if (name == ErrLevel()) {
    return spdlog::level::err;
  }
  if (name == CriticalLevel()) {
    return spdlog::level::critical;
  }
  if (name == OffLevel()) {
    return spdlog::level::off;
  }
  return spdlog::level::n_levels;
}

}  // namespace light_or::util

#endif
