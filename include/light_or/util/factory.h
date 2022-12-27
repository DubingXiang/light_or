/*
 * @Author: dubing.xiang
 * @Date: 2022-07-09 10:38:00
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-07-09 10:38
 * @Description:
 */

#ifndef LIGHT_OR_FACTORY_H_
#define LIGHT_OR_FACTORY_H_
#include <stdexcept>
#include <map>
#include <memory>
#include <light_or/util/macros.h>

namespace light_or::util {

/**
 * @brief 模板变参工厂类
 * 第一个参数 Base 为欲生产的基类
 * 第二个参数 Args 为变参，表示基类 Base 的构造函数的参数集合
 */
template <class Base, typename... Args>
struct Factory {
  /**
   * @brief 生产 Base 的函数指针
   * 这里用结构体包装了一层
   */
  struct FnType {
    using FunPtr = Base* (*)(Args&&...);
  };

  /**
   * @brief 注册辅助类
   * 注册时，构造该类的一个变量，在构造函数中进行产品的注册
   */
  template <class Impl>
  struct Register {
    Register(const std::string& key) {
      Factory::Instance()._creator_map.emplace(
          key, [](Args&&... args) -> Base* { return new Impl(std::forward<Args>(args)...); });
    }
  };

  inline Base* Produce(const std::string& key, Args&&... args) {
    if (_creator_map.find(key) == _creator_map.end()) {
      std::string error_msg = "the base product key[" + key + "] is not exist!";
      throw std::invalid_argument(error_msg);
    }
    return _creator_map[key](std::forward<Args>(args)...);
  }
  inline std::unique_ptr<Base> ProduceUnique(const std::string& key, Args&&... args) {
    return std::unique_ptr<Base>(Produce(key, std::forward<Args>(args)...));
  }

  std::shared_ptr<Base> ProduceShared(const std::string& key, Args&&... args) {
    return std::shared_ptr<Base>(Produce(key, std::forward<Args>(args)...));
  }

  inline static Factory& Instance() {
    static Factory instance;
    return instance;
  }

 private:
  Factory()                          = default;
  Factory(const Factory&)            = delete;
  Factory& operator=(const Factory&) = delete;
  Factory(Factory&&)                 = delete;
  std::map<std::string, typename FnType::FunPtr> _creator_map;
};

#define REGISTER_NAME(Base, Impl) \
  CONNECT(CONNECT(CONNECT(registry_line_, __LINE__), CONNECT(_base_, Base)), CONNECT(_impl_, Impl))
#define REGISTER(Base, Impl, key, ...)                                                       \
  static const ::light_or::util::Factory<Base, ##__VA_ARGS__>::Register<Impl> REGISTER_NAME( \
      Base, Impl)(key);

#define REGISTER_IMPL(Base, Impl, ...)                                        \
  static const ::light_or::util::Factory<Base, ##__VA_ARGS__>::Register<Impl> \
      registry_line_##__LINE__##_base_##Base##_impl_##Impl(#Impl);
}  // namespace light_or::util

#endif