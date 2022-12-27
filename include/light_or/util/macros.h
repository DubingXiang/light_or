
/**
 *  Copyright (C) 2021-2022 by Contributors
 *  @file     macros.h
 *  @brief    file description
 *
 *  @author   dubing xiang
 */

#ifndef LIGHT_OR_UTIL_MACROS_H_
#define LIGHT_OR_UTIL_MACROS_H_
#include <light_or/util/timer.h>

namespace light_or {
namespace util {

#define GLOBAL_RANDOM_SEED 314

#define DEFAULT_INT 0
#define DEFAULT_STRING ""
#define DEFAULT_BOOL false
#define DEFAULT_PTR nullptr
#define DEFAULT_LONG 0
#define DEFAULT_DOUBLE 0.0
#define DEFAULT_LONG_LONG 0

#define EPS 1e-6

// TypeName为类型名，如class A
// 调用RETURN_CLASS_STRING(A)
// 实际会编译为 "A"
#define STRING_NAME(var) #var
#define CLASS_NAME(TypeName) STRING_NAME(TypeName)

#define CONNECTION(text1, text2) text1##text2
#define CONNECT(text1, text2) CONNECTION(text1, text2)

#define DEFAULT_CONSTRUCTOR(TypeName) TypeName() = default;
#define DEFAULT_DESTRUCTOR(TypeName) ~TypeName() = default;
#define DECLARE_VIRTUAL_DESTRUCTOR(TypeName) virtual ~TypeName() = default;

#define INSTANCE(TypeName)      \
  static TypeName& Instance() { \
    static TypeName instance;   \
    return instance;            \
  }

#define DISALLOW_CONSTRUCTOR(TypeName) \
 private:                              \
  TypeName() = default;

#define DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete;
#define DISALLOW_ASSIGN(TypeName) TypeName& operator=(const TypeName&) = delete;

/**
 * @brief	禁止使用 copy 和 operator= 函数
 *			通常是声明为类的private，用于单例
 * @param	TypeName 类的名称
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  DISALLOW_COPY(TypeName)                  \
  DISALLOW_ASSIGN(TypeName)

/**
 * @brief	生成默认构造函数和默认析构函数
 * @param	TypeName 类的名称
 */
#define DEFAULT_CONSTRUCTOR_AND_DEFAULT_DESTRUCTOR(TypeName) \
  DEFAULT_CONSTRUCTOR(TypeName)                              \
  DEFAULT_DESTRUCTOR(TypeName)

#define DECLARE_TYPE_NAME_FUNC(TypeName) \
  const char* type_name() const {        \
    return RETURN_CLASS_NAME(TypeName)   \
  }

template <typename T>
struct argument_type;
template <typename T, typename U>
struct argument_type<T(U)> {
  using type = U;
};

#define DECLARE_GETTER(TypeName, field)                                        \
  ::simple_or::utils::argument_type<void(TypeName)>::type get##field() const { \
    return field;                                                              \
  }
#define DECLARE_SETTER(TypeName, field)                                          \
  void set##field(::simple_or::utils::argument_type<void(TypeName)>::type val) { \
    field = val;                                                                 \
  }
#define DECLARE_GETTER_SETTER(TypeName, field) \
  DECLARE_GETTER(TypeName, field);             \
  DECLARE_SETTER(TypeName, field)

// for debug
#define PrintFunction() \
  // SPDLOG_INFO("########## Function: file:{} :{} line:{}", __FILE__, __FUNCTION__, __LINE__);
#define StartPrintFunction() \
  // SPDLOG_INFO("########## Start Function: file:{} :{} line:{}", __FILE__, __FUNCTION__,
  // __LINE__);
#define EndPrintFunction() \
  // SPDLOG_INFO("########## End Function: file:{} :{} line:{}", __FILE__, __FUNCTION__, __LINE__);

#define START_TIMING()                                 \
  ::light_or::util::Timer CONNECT(__FUNCTION_, timer); \
  CONNECT(__FUNCTION_, timer).Start();
#define END_TIMING()                  \
  CONNECT(__FUNCTION_, timer).Stop(); \
  // SPDLOG_INFO("\t Elapsed Time:{} ms", CONNECT(__FUNCTION_, timer).GetMs());
}  // namespace util
}  // namespace light_or

#endif  // LIGHT_OR_UTIL_MACROS_H_