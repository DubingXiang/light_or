/*
 * @Author: dubing.xiang
 * @Date: 2022-06-25 16:40:43
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-25 16:40
 * @Description: define constrint class
 */
#ifndef LIGHT_OR_CONSTRAINT_DEF_H_
#define LIGHT_OR_CONSTRAINT_DEF_H_

#include <light_or/util/macros.h>
#include <light_or/util/kargs_container.h>
#include <light_or/model/solution_def.h>

namespace light_or {

class ConstraintDef {
 public:
  ConstraintDef()                                      = default;
  virtual ~ConstraintDef()                             = default;
  virtual const char* class_name() const               = 0;
  virtual std::unique_ptr<ConstraintDef> Clone() const = 0;

  virtual bool Check(const SolutionDef* solution, KArgsContainer* kargs = nullptr) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ConstraintDef);
};
template <class Functor>
class FunctionConstraint : public ConstraintDef {
 public:
  FunctionConstraint() = default;
  FunctionConstraint(Functor&& func) : _callback(std::move(func)) {}
  void SetFunction(Functor&& func) {
    _callback = std::move(func);
  }
  const char* class_name() const override {
    return "FunctionConstraint";
  }
  std::unique_ptr<ConstraintDef> Clone() const override {
    return std::make_unique<FunctionConstraint>(std::move(Functor(_callback)));
  }

  bool Check(const SolutionDef* solution, KArgsContainer* kargs = nullptr) const override {
    return _callback(solution, kargs);
  }

 private:
  Functor _callback;
};
template <class Functor>
std::unique_ptr<FunctionConstraint<Functor>> MakeFunctionConstraint(Functor&& func) {
  std::unique_ptr<FunctionConstraint<Functor>> ct =
      std::make_unique<FunctionConstraint<Functor>>(std::forward<Functor>(func));
  return ct;
}

}  // namespace light_or

#endif