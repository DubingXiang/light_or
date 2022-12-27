/*
 * @Author: dubing.xiang
 * @Date: 2022-06-25 16:40:02
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-25 16:40
 * @Description: define objective class
 */
#ifndef LIGHT_OR_OBJECTIVE_DEF_H_
#define LIGHT_OR_OBJECTIVE_DEF_H_

#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <light_or/util/macros.h>

namespace light_or {
class SolutionDef;
class ObjectiveDef {
 public:
  enum Direction { Minimize = 1, Maximize = -1 };

 public:
  ObjectiveDef()                         = default;
  virtual ~ObjectiveDef()                = default;
  virtual const char* class_name() const = 0;

  virtual double Compute(const SolutionDef* solution) const = 0;
  virtual std::unique_ptr<ObjectiveDef> Clone() const       = 0;
  void set_direction(Direction direction) {
    _direction = direction;
  }
  Direction direction() const {
    return _direction;
  }

 protected:
  Direction _direction = Direction::Minimize;

 private:
  DISALLOW_COPY_AND_ASSIGN(ObjectiveDef);
};

struct ObjectiveData {
  ObjectiveData(std::string obj_name, double obj_value, ObjectiveDef::Direction obj_direction)
      : name(std::move(obj_name)), value(obj_value), direction(obj_direction) {}
  std::string name                  = "UNKNOWN";
  double value                      = 0.0;
  ObjectiveDef::Direction direction = ObjectiveDef::Direction::Minimize;
};
class LinearSumObjective;
class MultiObjective {
 public:
  MultiObjective();
  virtual ~MultiObjective();

  std::vector<ObjectiveData> Compute(const SolutionDef* solution) const;

  bool Add(std::unique_ptr<ObjectiveDef> obj, std::string name);
  const ObjectiveDef* Objective(const std::string& name) const;
  double ObjectiveValue(const SolutionDef* solution, const std::string& name) const;
  int objective_count() const;
  std::vector<std::string> objective_names() const;

  std::unique_ptr<LinearSumObjective> ToLinearSumObjective(
      const std::map<std::string, double>& weight_map) const;

  std::unique_ptr<MultiObjective> Clone() const;

 protected:
  class Impl;
  std::unique_ptr<Impl> _impl;
  Impl* impl() const {
    return _impl.get();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(MultiObjective);
};
class LinearSumObjective : public ObjectiveDef, public MultiObjective {
 public:
  LinearSumObjective();

  const char* class_name() const override {
    return "LinearSumObjective";
  }
  double Compute(const SolutionDef* solution) const override;
  std::unique_ptr<ObjectiveDef> Clone() const override;
  void set_weight_map(const std::map<std::string, double>& weight_map) {
    _obj_weights = weight_map;
  }
  void UpdateWeight(const std::string& name, double weight) {
    _obj_weights[name] = weight;
  }
  double GetWeightOr(const std::string& name, double value = 0.0) const {
    auto pos = _obj_weights.find(name);
    return pos == _obj_weights.end() ? value : pos->second;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(LinearSumObjective);

  std::map<std::string, double> _obj_weights;
};

class Evaluator {
 public:
  virtual ~Evaluator() = default;
  class Result {
    double cost;
  };
  Result eval(SolutionDef* solution);
};

class ObjectiveEvaluator : public Evaluator {
  class Result {
    double obj_val;
    double coef;
    double primal_val;
  };
  ObjectiveDef* _obj;
};

template <typename Functor>
class FunctionObjective : public ObjectiveDef {
 public:
  FunctionObjective() = default;
  FunctionObjective(Functor&& func) : _callback(std::move(func)) {}
  void SetFunction(Functor&& func) {
    _callback = std::move(func);
  }

  const char* class_name() const override {
    return "FunctionObjective";
  }
  std::unique_ptr<ObjectiveDef> Clone() const override {
    Functor copy(_callback);
    return std::make_unique<FunctionObjective>(std::move(copy));
  };

  double Compute(const SolutionDef* solution) const override {
    return _callback(solution);
  }

 private:
  Functor _callback;
};

template <class Functor>
std::unique_ptr<FunctionObjective<Functor>> MakeFunctionObjective(Functor&& func) {
  std::unique_ptr<FunctionObjective<Functor>> obj =
      std::make_unique<FunctionObjective<Functor>>(std::forward<Functor>(func));
  return obj;
}

}  // namespace light_or

#endif