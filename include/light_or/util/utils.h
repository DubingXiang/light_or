/*
 * @Author: dubing.xiang
 * @Date: 2022-06-26 22:12:19
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-26 22:12
 * @Description: utility functions
 */
#ifndef LIGHT_OR_UTILS_H_
#define LIGHT_OR_UTILS_H_
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include <stdint.h>

constexpr unsigned long long operator"" _ms(unsigned long long ms) {
  unsigned long long time_ms = ms;
  return time_ms;
}
constexpr unsigned long long operator"" _s(unsigned long long s) {
  unsigned long long time_s = s * 1e3;
  return time_s;
}
constexpr unsigned long long operator"" _mint(unsigned long long mint) {
  unsigned long long time_s = mint * 60 * 1e3;
  return time_s;
}

constexpr unsigned long long operator"" _h(unsigned long long hour) {
  unsigned long long time_s = hour * 60 * 60 * 1e3;
  return time_s;
}
namespace light_or::util {

using Time = uint64_t;

// this hash source code from The Annotated STL Source (using SGI STL) HouJie
template <typename T>
inline void hash_combine(size_t& seed, const T& val) {
  seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
template <typename T>
inline void hash_val(size_t& seed, const T& val) {
  hash_combine(seed, val);
}

template <typename T, typename... Types>
inline void hash_val(size_t& seed, const T& val, const Types&... args) {
  hash_combine(seed, val);
  hash_val(seed, args...);
}

template <typename... Types>
inline size_t hash_val(const Types&... args) {
  size_t seed = 0;
  hash_val(seed, args...);
  return seed;
}
inline void Split(const std::string& s, char delim, std::vector<std::string>& results) {
  std::string::const_iterator start = s.begin();
  std::string::const_iterator end   = s.end();
  std::string::const_iterator next  = std::find(start, end, delim);
  while (next != end) {
    results.emplace_back(std::string(start, next));
    start = next + 1;
    next  = std::find(start, end, delim);
  }
  results.emplace_back(std::string(start, next));
}
inline std::vector<std::string> Split(const std::string& s, char delim) {
  std::string elem;
  std::istringstream is(s);
  std::vector<std::string> elems;
  while (std::getline(is, elem, delim)) {
    elems.push_back(elem);
  }
  return elems;
}

inline bool IsDoubleEqual(double v1, double v2, double tolerance = 1e-6) {
  return std::abs(v1 - v2) < tolerance;
}
inline bool IsDoubleLess(double v1, double v2, double tolerance = 1e-6) {
  return v1 - v2 < -tolerance;
}
inline bool IsDoubleGreater(double v1, double v2, double tolerance = 1e-6) {
  return v1 - v2 > tolerance;
}

inline bool IsDoubleLessEqual(double v1, double v2, double tolerance = 1e-6) {
  return v1 - v2 < tolerance;
}
inline bool IsDoubleGreaterEqual(double v1, double v2, double tolerance = 1e-6) {
  return v1 - v2 > -tolerance;
}

}  // namespace light_or::util

#endif