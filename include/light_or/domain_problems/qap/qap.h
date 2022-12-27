/*
 * @Author: dubing.xiang
 * @Date: 2022-11-25 12:05:35
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-11-25 12:05
 * @Description:
 */

#ifndef LIGHT_OR_QAP_H_
#define LIGHT_OR_QAP_H_
#include <unordered_map>
#include <memory>
#include <memory>
#include <iostream>
#include <vector>
#include <string>

#include <light_or/model/model.h>
#include <light_or/model/solution_encodings/int_vector_solution.h>
#include <light_or/util/solver_template.h>

namespace light_or {
class Solver;
class AlgorithmParameters;
}  // namespace light_or

namespace light_or::qap {

class QAPProblem : public light_or::Model {
 public:
  QAPProblem(int n,
             const std::vector<std::vector<double>>& flow_matrix,
             const std::vector<std::vector<double>>& distance_matrix,
             const std::vector<double>& build_cost_matrix = {});

  int N() const {
    return _n;
  }
  double Flow(int v1, int v2) const;
  double Distance(int v1, int v2) const;
  double BuildCost(int v1) const;
  const std::vector<std::vector<double>>& FlowMatrix() const {
    return _flow_matrix;
  }
  const std::vector<std::vector<double>>& DistanceMatrix() const {
    return _distance_matrix;
  }
  const std::vector<double>& BuildCostMatrix() const {
    return _build_cost_matrix;
  }

 private:
  bool CheckIndex(int v) const;
  int _n;
  std::vector<std::vector<double>> _flow_matrix;
  std::vector<std::vector<double>> _distance_matrix;
  std::vector<double> _build_cost_matrix;
};

class QAPSolution : public light_or::IntVectorSolution {
 public:
  QAPSolution(const QAPProblem* problem);
  std::unique_ptr<IntVectorSolution> ChildClone() const override;

  const QAPProblem* problem() const;
  const char* class_name() const override {
    return "QAPSolution";
  }

  int Location(int facility) const;
  int Facility(int location) const;
  bool CheckIndex(int facility) const;
  void Assign(int facility, int location);
};

class QAPSolver : public light_or::util::SolverTemplate<QAPProblem, QAPSolution> {
 public:
  QAPSolver(bool deterministic);
  bool Solve(QAPProblem* problem, const AlgorithmParameters& parameters) override;
  void Modeling() override;
  void InitialSolution(bool use_random = false) override;
};

class QAPIOParser : public light_or::util::IOParser<QAPProblem, QAPSolution> {
 public:
  void Parse(const std::string& input_file) override;
  std::string Parse(const SolutionType& solution) override;
};

}  // namespace light_or::qap

#endif