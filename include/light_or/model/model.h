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
// 模型类。存储了构成问题的关键要素：目标、约束。
// 而决策变量则由用户依据实际问题的不同，需要自行定义。且通常属于 solution ，即解的编码过程
class Model : public BaseObject {
 public:
  Model();
  virtual ~Model();
  const char* class_name() const override {
    return "Model";
  }
  // 添加约束，并赋予一个唯一的名称
  bool AddConstraint(std::unique_ptr<ConstraintDef> ct, std::string name);
  // 添加约束：无需用户自定义约束实现类，而是通过 lambda 表达式、仿函数、function等callable对象的方式。
  // 但是需要保证括号运算符重载：返回值为： bool，输入为：const SolutionDef* solution, KArgsContainer* kargs
  // 如：model->AddConstraintCallable(
  //              [](const SolutionDef* solution, KArgsContainer* kargs){/*return isFeasible;*/}
  //              , "UDFCtName");
  template <typename Functor>
  void AddConstraintCallable(Functor&& func, std::string name) {
    auto ct = MakeFunctionConstraint(std::forward<Functor>(func));
    this->AddConstraint(std::move(ct), name);
  }
  // 添加目标函数，并赋予一个唯一的名称
  bool AddObjective(std::unique_ptr<ObjectiveDef> obj, std::string name);
  // 添加目标：无需用户自定义目标实现类，而是通过 lambda 表达式、仿函数、function等callable对象的方式。
  // 但是需要保证括号运算符重载：返回值为： double ，输入为：const SolutionDef* solution
  // 如：model->AddObjectiveCallable(
  //              [](const SolutionDef* solution){/*return objValue;*/}
  //              , "UDFObjName");
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
  // 最小化某个目标，若目标函数采用lambda表达式、仿函数、function等callable对象，通常结合 MakeFunctionObjective来调用
  // 如：model->Minimize(MakeFunctionObjective(callable), "UDFObjName");
  void Minimize(std::unique_ptr<ObjectiveDef> obj, std::string name) {
    obj->set_direction(ObjectiveDef::Minimize);
    AddObjective(std::move(obj), std::move(name));
  }
  // 最大化某个目标，若目标函数采用lambda表达式、仿函数、function等callable对象，通常结合 MakeFunctionObjective来调用
  // 如：model->Maximize(MakeFunctionObjective(callable), "UDFObjName");
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