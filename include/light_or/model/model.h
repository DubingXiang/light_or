/*
 * @Author: dubing.xiang
 * @Date: 2022-06-25 16:02:18
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-25 16:02
 * @Description: define model class
 */
#ifndef LIGHT_OR_MODEL_H_
#define LIGHT_OR_MODEL_H_

#include <memory>
#include <vector>
#include <type_traits>

#include <light_or/util/macros.h>
#include <light_or/base/base_object.h>
#include <light_or/base/traits.h>
#include <light_or/model/constraint_def.h>
#include <light_or/model/objective_def.h>

namespace light_or {

class Model : public BaseObject {
 public:
  Model();
  virtual ~Model();
  const char* class_name() const override {
    return "Model";
  }

  bool AddConstraint(std::unique_ptr<ConstraintDef> ct, std::string name);

  template <typename Functor>
  void AddConstraintCallable(Functor&& func, std::string name) {
    auto ct = MakeFunctionConstraint(std::forward<Functor>(func));
    this->AddConstraint(std::move(ct), name);
  }
  bool AddObjective(std::unique_ptr<ObjectiveDef> obj, std::string name);
  template <typename Functor>
  void AddObjectiveCallable(Functor&& func, std::string name) {
    auto obj = MakeFunctionObjective(std::forward<Functor>(func));
    this->AddObjective(std::move(obj), name);
  }
  const ConstraintDef* ConstraintOrNull(const std::string& name) const;
  const ObjectiveDef* ObjectiveOrNull(const std::string& name) const;

  bool CheckFeasibility(const SolutionDef* solution, KArgsContainer* kargs = nullptr) const;
  const MultiObjective* multi_objective() const;
  MultiObjective* mutable_multi_objective();

  void Minimize(std::unique_ptr<ObjectiveDef> obj, std::string name) {
    obj->set_direction(ObjectiveDef::Minimize);
    AddObjective(std::move(obj), std::move(name));
  }
  void Maximize(std::unique_ptr<ObjectiveDef> obj, std::string name) {
    obj->set_direction(ObjectiveDef::Maximize);
    AddObjective(std::move(obj), std::move(name));
  }

  std::unique_ptr<Model> Clone() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(Model);
  class Impl;
  std::unique_ptr<Impl> _impl;
  Impl* impl() const;
};

}  // namespace light_or

#endif