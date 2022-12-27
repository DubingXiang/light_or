/*
 * @Author: dubing.xiang
 * @Date: 2022-06-30 08:34:35
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-30 08:34
 * @Description: how to collect solution in the searing process
 */
#ifndef LIGHT_OR_SOLUTION_COLLECTOR_H_
#define LIGHT_OR_SOLUTION_COLLECTOR_H_

#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <memory>
#include <light_or/util/macros.h>

namespace light_or {
class SolutionDef;
class Solver;

// 搜索过程中，收集解的策略，如多目标下，定义哪些解会被收集
class SolutionCollector {
 public:
  SolutionCollector(const Solver* solver);
  virtual ~SolutionCollector();
  const Solver* solver() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(SolutionCollector);
  const Solver* _solver;
};

}  // namespace light_or

#endif  // LIGHT_OR_SOLUTION_COLLECTOR_H_