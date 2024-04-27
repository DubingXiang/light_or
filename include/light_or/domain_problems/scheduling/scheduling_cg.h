/*
 * @Author: zheming.zhang
 * @Date: 2024-03-23 11:11:12
 * @Last Modified by: zheming.zhang
 * @Last Modified time: 2024-03-23 11:11
 * @Description: scheduling column generation solver
 */
#ifndef LIGHT_OR_SCHEDULING_CG_H_
#define LIGHT_OR_SCHEDULING_CG_H_

#include <vector>
#include <memory>
#include <optional>

#include <light_or/domain_problems/scheduling/scheduling.h>


namespace light_or::scheduling {
class SchedulingCGSolver {
 public:
  SchedulingCGSolver(bool deterministic): _deterministic(deterministic) {}
  ~SchedulingCGSolver() = default;

  void Solve(SchedulingProblem* problem, const AlgorithmParameters& parameters);
  
  // bool Solve(SchedulingProblem* problem, const AlgorithmParameters& parameters);
  const SchedulingSolution* BestSolution() const { return nullptr; }
  
  enum ObjectiveStatsType { global_best, current_best };
  std::string ExportObjectiveStats(ObjectiveStatsType type) const { return "1";}
  // void ExportObjectiveStats(ObjectiveStatsType type, const std::string& file) const;

 private:
  // SchedulingProblem* _problem                        = nullptr;
  // std::unique_ptr<light_or::Solver> _solver    = nullptr;
  // std::unique_ptr<SchedulingSolution> _init_solution = nullptr;
  const SchedulingSolution* _best_solution           = nullptr;
  bool _deterministic = false;
  // util::RandomGenerator _random;
};
}  // namespace light_or::scheduling

#endif  // LIGHT_OR_SCHEDULING_CG_H_