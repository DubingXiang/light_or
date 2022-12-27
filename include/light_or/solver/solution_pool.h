/*
 * @Author: dubing.xiang
 * @Date: 2022-11-07 22:50:28
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-11-07 22:50
 * @Description:
 */
#ifndef LIGHT_OR_SOLUTION_POOL_H_
#define LIGHT_OR_SOLUTION_POOL_H_

#include <vector>
#include <memory>

namespace light_or {
class SolutionDef;
class SolutionPool {
 public:
  SolutionPool(std::vector<SolutionDef*> solns = {});
  ~SolutionPool();
  const std::vector<SolutionDef*>& solutions() const;
  SolutionPool Copy(const std::vector<int>& indices) const;
  void Append(const SolutionPool& other);
  void Push(SolutionDef* solution);
  SolutionDef* Pop();
  void Clear();

 private:
  bool CheckIndex(int index) const;
  std::vector<SolutionDef*> _solutions;
};

class SolutionPoolHelper {
 public:
  static std::vector<int> solution_indices(const SolutionPool& solution_pool);
  static std::vector<std::vector<double>> solution_objective_value_matrix(
      const SolutionPool& solution_pool);
  static std::vector<bool> objective_minimize_list(const SolutionPool& solution_pool);
};

}  // namespace light_or

#endif