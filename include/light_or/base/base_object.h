/*
 * @Author: dubing.xiang
 * @Date: 2022-06-25 16:02:50
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-25 16:02
 * @Description: define base object class
 */
#ifndef LIGHT_OR_BASE_OBJECT_H_
#define LIGHT_OR_BASE_OBJECT_H_

#include <memory>
#include <light_or/util/macros.h>

namespace light_or {
class BaseObject {
 public:
  virtual ~BaseObject() = default;

 public:
  virtual const char* class_name() const = 0;

  template <class Derived, bool NonConst>
  Derived* as() const;
  template <class Derived>
  Derived* as();
  template <class Derived>
  const Derived* as() const;
};

template <class Derived, bool NonConst>
Derived* BaseObject::as() const {
  return dynamic_cast<Derived*>(this);
}
template <class Derived>
Derived* BaseObject::as() {
  return dynamic_cast<Derived*>(this);
}

template <class Derived>
const Derived* BaseObject::as() const {
  return dynamic_cast<const Derived*>(this);
}

using ElementType = std::size_t;

}  // namespace light_or

#endif