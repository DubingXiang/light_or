/*
 * @Author: dubing.xiang
 * @Date: 2022-07-04 23:26:07
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-07-04 23:26
 * @Description:
 */
#ifndef LIGHT_OR_KARGS_CONTAINER_H_
#define LIGHT_OR_KARGS_CONTAINER_H_
#include <any>
#include <unordered_map>
#include <optional>
#include <string>

namespace light_or {
class KArgsContainer {
 public:
  template <typename T>
  bool Insert(const std::string& key, T value);
  template <typename T>
  std::optional<T> Get(const std::string& key) const;
  template <typename T>
  T GetOr(const std::string& key, T value) const;

 private:
  std::unordered_map<std::string, std::any> _meta_data;
};
template <typename T>
bool KArgsContainer::Insert(const std::string& key, T value) {
  auto [itor, is_existed] = _meta_data.try_emplace(key, value);
  return !is_existed;
}
// TODO: T 是引用类型
template <typename T>
std::optional<T> KArgsContainer::Get(const std::string& key) const {
  auto itor = _meta_data.find(key);
  return itor == _meta_data.end() ? std::nullopt : std::optional<T>(std::any_cast<T>(itor->second));
}
template <typename T>
T KArgsContainer::GetOr(const std::string& key, T value) const {
  auto itor = _meta_data.find(key);
  if (itor == _meta_data.end()) {
    return value;
  }
  auto p = std::any_cast<T>(&(itor->second));
  return p == nullptr ? value : *p;
}

}  // namespace light_or

#endif  // LIGHT_OR_KARGS_CONTAINER_H_