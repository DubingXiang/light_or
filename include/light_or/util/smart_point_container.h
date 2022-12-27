/*
 * @Author: dubing.xiang
 * @Date: 2022-11-11 11:40:32
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-11-11 11:40
 * @Description:
 */

#ifndef LIGHT_OR_SMART_POINT_CONTAINER_H_
#define LIGHT_OR_SMART_POINT_CONTAINER_H_
#include <memory>
#include <vector>
#include <string>

namespace light_or::util {
template <typename T, typename SmartPtr = std::unique_ptr<T> >
class SmartPointContainer {
 public:
  using Element    = T;
  using ElementPtr = SmartPtr;

 public:
  SmartPointContainer() = default;

  bool Add(ElementPtr ele, const std::string& name) {
    if (ele == nullptr) return false;
    if (name == "") return false;
    if (has(name)) return false;
    _elements.insert({name, std::move(ele)});
    return true;
  }
  ElementPtr Remove(const std::string& name) {
    auto itor = ElementItor(name);
    if (itor == _elements.end()) {
      return nullptr;
    }
    auto removed_element = std::move(itor->second);
    _elements.erase(itor);
    return removed_element;
  }

  const Element* ElementOrNull(const std::string& name) const {
    auto itor = ElementItor(name);
    return itor == _elements.end() ? nullptr : itor->second.get();
  }
  Element* ElementOrNull(const std::string& name) {
    auto itor = ElementItor(name);
    return itor == _elements.end() ? nullptr : itor->second.get();
  }
  bool has(const std::string& name) const {
    return ElementItor(name) != _elements.end();
  }
  std::vector<const Element*> elements() const {
    std::vector<const Element*> elements;
    elements.reserve(_elements.size());
    for (const auto& [_, element] : _elements) {
      elements.push_back(element.get());
    }
    return elements;
  }
  std::vector<std::string> names() const {
    std::vector<std::string> names;
    names.reserve(_elements.size());
    for (const auto& [name, _] : _elements) {
      names.push_back(name);
    }
    return names;
  }
  int Size() const { return _elements.size(); }
 private:
  typename std::map<std::string, ElementPtr>::const_iterator ElementItor(
      const std::string& name) const {
    return _elements.find(name);
  }
  typename std::map<std::string, ElementPtr>::iterator ElementItor(const std::string& name) {
    return _elements.find(name);
  }

 private:
  std::map<std::string, ElementPtr> _elements;
};

}  // namespace light_or::util

#endif
