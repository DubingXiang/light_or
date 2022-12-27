/*
 * @Author: dubing.xiang
 * @Date: 2022-06-25 16:34:44
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-25 16:34
 * @Description: define solvef class
 */
#ifndef LIGHT_OR_SOLVER_H_
#define LIGHT_OR_SOLVER_H_

#include <light_or/base/traits.h>
#include <light_or/util/macros.h>
#include <light_or/util/timer.h>
#include <light_or/model/model.h>
#include <light_or/model/solution_def.h>
#include <light_or/model/objective_def.h>
#include <light_or/solver/solution_pool.h>
#include <light_or/solver/op_def.h>
#include <light_or/solver/parameters.h>

namespace light_or {
class SearchWatcher;


class Solver {
 public:
  Solver(Model* model, const AlgorithmParameters& parameters = DefaultAlgorithmParameters());
  ~Solver();
  Model* model() const;

  void Solve(const SolutionDef* input_solution);
  void Solve(const SolutionPool* input_solution_pool = nullptr);
  std::vector<SolutionDef*> AllSolutions() const;
  template <typename DomainSolution, typename... Args>
  DomainSolution* CreateSolution(Args&&... args) {
    auto soln           = std::make_unique<DomainSolution>(std::forward<Args>(args)...);
    DomainSolution* ret = soln.get();
    TakeOwnership(std::move(soln));
    return ret;
  }
  SolutionDef* TakeOwnership(std::unique_ptr<SolutionDef> solution);
  SearchWatcher* TakeOwnership(std::unique_ptr<SearchWatcher> search_watcher);

  void AddOp(std::unique_ptr<OpDef> op);
  template <typename Functor,
            std::enable_if_t<(!std::is_base_of_v<OpDef, Functor> &&
                              !light_or::is_smart_pointer<Functor>::value),
                             int> = 0>
  void AddOp(Functor&& func) {
    this->AddOp(MakeFunctionOp(std::forward<Functor>(func)));
  }
  const OpDef* Op(const std::string& name) const;
  OpDef* MutableOp(const std::string& name);
  std::vector<const OpDef*> OpList() const;

  bool AddWatcher(SearchWatcher* watcher);
  SearchWatcher* GetWatcher(const std::string& name) const;

  double RunDurationMs() const;
  // The number of iterations excuted since the start of the search
  int iterations() const;
  const AlgorithmParameters& parameters() const;

  const SolutionPool* current_solution_pool() const;
  const SolutionPool* searched_solution_pool() const;

  const SolutionDef* GetBestSolutionWithLinearSumObjectiveValue(
      const std::map<std::string, double>& weight_map = {}) const;

  //
  enum ObjectiveStatsType { global_best, current_best };
  std::string ExportObjectiveStats(ObjectiveStatsType type) const;
  void ExportObjectiveStats(ObjectiveStatsType type, const std::string& file) const;

 private:
  void Init();
  bool Modeling();

  // void MakePhase();
  // void InstallConstraints();
  // void InstallObjectives();
  // void InstallOps();
  // void InstallSolutionCollector();

 private:
  DISALLOW_COPY_AND_ASSIGN(Solver);

 private:
  class Impl;
  std::unique_ptr<Impl> _impl;

 public:
  Impl* impl() const;
};

}  // namespace light_or

#endif