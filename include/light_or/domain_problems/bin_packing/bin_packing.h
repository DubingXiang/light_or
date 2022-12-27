/*
 * @Author: dubing.xiang
 * @Date: 2022-12-20 20:49:39
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-12-20 20:49
 * @Description:
 */

#ifndef LIGHT_OR_BIN_PACKING_H_
#define LIGHT_OR_BIN_PACKING_H_
#include <vector>

#include <light_or/model/model.h>
#include <light_or/model/solution_encodings/int_vector_solution.h>
#include <light_or/util/solver_template.h>

namespace light_or {
class Solver;
class AlgorithmParameters;
}  // namespace light_or
namespace light_or::bin_packing {
struct Bin {
  Bin(std::size_t bin_type, double bin_capacity, std::size_t bin_id)
      : type(bin_type), capacity(bin_capacity), id(bin_id) {}
  std::size_t type;
  double capacity;
  std::size_t id;
};
struct Item {
  Item(double item_weight, double item_value, std::size_t item_id)
      : weight(item_weight), value(item_value), id(item_id) {}
  double weight;
  double value;
  std::size_t id;
};

class BinPackingProblem : public light_or::Model {
 public:
  BinPackingProblem(const std::vector<double>& capacities, const std::vector<Item>& items);

  double CapacityOfBinType(std::size_t bin_type) const;
  double Weight(std::size_t item_id) const;
  double Value(std::size_t item_id) const;
  const Item* ItemOrNull(std::size_t item_id) const;
  int item_num() const;
  int bin_type_num() const;

 private:
  std::vector<double> _capacities;  // 每种 bin 的 capacity
  std::vector<Item> _items;
};
class BinPackingSolution : public light_or::IntVectorSolution {
 public:
  BinPackingSolution(const BinPackingProblem* problem);
  std::unique_ptr<IntVectorSolution> ChildClone() const override;
  const char* class_name() const override {
    return "BinPackingSolution";
  }
  const BinPackingProblem* problem() const;
  //   int Bin(int item) const;
  const IntVector& Items(std::size_t bin) const;

  // void Pack(int bin, int item);
  void TryPack(const Bin& bin, std::size_t item);
  double CapacityOfBin(std::size_t bin_index) const;
  int bin_type(std::size_t bin) const;

 private:
  std::vector<int> _bin_types;
};

class BinPackingSolver
    : public light_or::util::SolverTemplate<BinPackingProblem, BinPackingSolution> {
 public:
  BinPackingSolver(bool deterministic);
  void Modeling() override;
  void InitialSolution(bool use_random = false) override;
};
class BinPackingIOParser : public light_or::util::IOParser<BinPackingProblem, BinPackingSolution> {
 public:
  void Parse(const std::string& input_file) override;
  std::string Parse(const SolutionType& solution) override;
};

class FeatureEvaluator {
 public:
  static double CalculateTotalWeight(const BinPackingSolution* solution, int bin);
};

}  // namespace light_or::bin_packing

#endif