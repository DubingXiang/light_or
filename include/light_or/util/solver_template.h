/*
 * @Author: dubing.xiang
 * @Date: 2022-12-22 19:53:55
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-12-22 19:53
 * @Description: solver_template
 */
#ifndef LIGHT_OR_SOLVER_TEMPLATE_H_
#define LIGHT_OR_SOLVER_TEMPLATE_H_

#include <light_or/util/random_generator.h>
#include <light_or/solver/solver.h>
#include <light_or/solver/parameters.h>

namespace light_or::util {
template <typename DomainProblem, typename DomainSolution>
class SolverTemplate {
 public:
  using ProblemType  = DomainProblem;
  using SolutionType = DomainSolution;

 public:
  SolverTemplate(bool deterministic) {
    _random.SetSeed(GetSeed(deterministic));
  }
  int32_t GetSeed(bool deterministic = true) {
    if (deterministic) {
      return 0;
    } else {
      return std::random_device()();
    }
  }
  virtual bool Solve(ProblemType* problem, const AlgorithmParameters& parameters) {
    _solver  = std::make_unique<light_or::Solver>(problem, parameters);
    _problem = problem;
    Modeling();
    InitialSolution();
    _solver->Solve(_init_solution.get());
    return true;
  }
  const SolutionType* BestSolution() const {
    return this->solver()
        ->GetBestSolutionWithLinearSumObjectiveValue()
        ->template as<SolutionType>();
  }
  virtual void Modeling()                               = 0;
  virtual void InitialSolution(bool use_random = false) = 0;
  light_or::Solver* solver() const {
    return _solver.get();
  }

 protected:
  ProblemType* _problem                        = nullptr;
  std::unique_ptr<light_or::Solver> _solver    = nullptr;
  std::unique_ptr<SolutionType> _init_solution = nullptr;
  const SolutionType* _best_solution           = nullptr;
  util::RandomGenerator _random;
};
template <typename DomainProblem, typename DomainSolution>
class IOParser {
 public:
  using ProblemType  = DomainProblem;
  using SolutionType = DomainSolution;

 public:
  virtual void Parse(const std::string& input_file)       = 0;
  virtual std::string Parse(const SolutionType& solution) = 0;
  ProblemType* problem() const {
    return _problem.get();
  }

 protected:
  std::unique_ptr<ProblemType> _problem = nullptr;
};

}  // namespace light_or::util

#endif  // LIGHT_OR_SOLVER_TEMPLATE_H_