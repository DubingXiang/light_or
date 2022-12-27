/*
 * @Author: dubing.xiang
 * @Date: 2022-06-25 16:37:50
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-25 16:37
 * @Description: define solution class
 */
#ifndef LIGHT_OR_SOLUTION_DEF_H_
#define LIGHT_OR_SOLUTION_DEF_H_
#include <vector>
#include <map>
#include <memory>
#include <atomic>

#include <light_or/base/base_object.h>
#include <light_or/util/kargs_container.h>
#include <light_or/model/objective_def.h>

namespace light_or {

class Model;
class SolutionDef : public BaseObject {
 public:
  using Id = int64_t;
  static std::atomic<Id> counter;
  struct SolutionInfo {
    int iteration       = -1;
    double search_time  = 0.0;
    std::string op_name = "";
  };

 public:
  SolutionDef(const Model* model);
  virtual ~SolutionDef();

  virtual std::unique_ptr<SolutionDef> Clone() const = 0;
  const char* class_name() const override {
    return "SolutionDef";
  }
  Id id() const;
  const Model* model() const {
    return _model;
  }
  const SolutionInfo& info() const;
  SolutionInfo* mutable_info();

  void ComputeObjective();
  bool CheckFeasibility(KArgsContainer* kargs = nullptr) const;
  const std::vector<ObjectiveData>& objective_data_list() const;
  ObjectiveData objective_data(const std::string& objective_name) const;
  /**
   * @brief
   *
   * @param weight_map
   * @param default_weight as wight of objective whose name is not in weight_map
   * @return double
   */
  double LinearSumObjectiveValue(const std::map<std::string, double>& weight_map = {},
                                 double default_weight                           = 1.0) const;

 protected:
  const std::atomic<Id> _id;
  const Model* _model;
  SolutionInfo _info;
  std::vector<ObjectiveData> _objective_data_list;
  mutable bool _updated_objective_value  = false;
  mutable double _cached_objective_value = 0.0;
};

}  // namespace light_or

#endif