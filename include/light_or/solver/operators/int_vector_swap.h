/*
 * @Author: dubing.xiang
 * @Date: 2022-12-10 15:58:02
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-12-10 15:58
 * @Description:
 */
#ifndef LIGHT_OR_INT_VECTOR_SWAP_H_
#define LIGHT_OR_INT_VECTOR_SWAP_H_
#include <memory>
#include <deque>
#include <light_or/util/utils.h>
#include <light_or/model/solution_encodings/int_vector_solution.h>

namespace light_or::util {
class RandomGenerator;
}

namespace light_or {
class OpDef;
class Solver;

class IntVectorSwap {
 public:
  virtual ~IntVectorSwap() = default;
  struct Neighbor {
    virtual ~Neighbor()         = default;
    IntVectorSolution* solution = nullptr;
    int lhs_encoding_index      = 0;
    int rhs_encoding_index      = 0;
    int lhs_index               = 0;
    int rhs_index               = 0;
    double gain                 = 0.0;
    bool operator==(const IntVectorSwap::Neighbor& other) const {
      return light_or::util::IsDoubleEqual(this->gain, other.gain);
    }
    bool operator<(const IntVectorSwap::Neighbor& other) const {
      return light_or::util::IsDoubleLess(this->gain, other.gain);
    }
  };
  struct Maker {
    virtual ~Maker() = default;
    virtual std::vector<Neighbor> MakeNeighbors(const IntVectorSolution* solution,
                                                light_or::util::RandomGenerator& random,
                                                int size);
  };
  struct Evaluator {
    virtual ~Evaluator() = default;
    virtual void Evaluate(std::vector<Neighbor>& neighbors);
    virtual double Evaluate(const Neighbor& neighbor) = 0;
  };
  struct TabuItem {
    virtual ~TabuItem()    = default;
    int lhs_encoding_index = 0;
    int rhs_encoding_index = 0;
    int lhs_index          = 0;
    int rhs_index          = 0;
    virtual bool IsEquivalent(const TabuItem& other) const {
      bool forward = lhs_encoding_index == other.lhs_encoding_index &&
                     rhs_encoding_index == other.rhs_encoding_index &&
                     lhs_index == other.lhs_index && rhs_index == other.rhs_index;
      bool reversed = lhs_encoding_index == other.rhs_encoding_index &&
                      rhs_encoding_index == other.lhs_encoding_index &&
                      lhs_index == other.rhs_index && rhs_index == other.lhs_index;
      return forward || reversed;
    }
  };
  struct TabuItemValue {
    TabuItemValue(TabuItem tabu_item, int freq) : item(tabu_item), frequency(freq) {}
    TabuItem item;
    int frequency = 0;
  };
  struct TabuMonitor {
    TabuMonitor(Solver* solver, bool is_minimize, int tabu_tenure);
    virtual ~TabuMonitor() = default;
    virtual TabuItem MakeTabuItem(const Neighbor& neighbor);
    virtual void Push(const TabuItem& item);
    virtual bool IsTabu(const Neighbor& neighbor);
    virtual void UpdateTabuList();
    Solver* solver() {
      return _solver;
    }

   protected:
    Solver* _solver;
    bool _is_minimize;
    int _tabu_tenure;
    std::deque<TabuItemValue> _tabu_list;
  };
  struct Selector {
    friend class IntVectorSwapOp;
    Selector();
    virtual ~Selector() = default;
    virtual std::vector<Neighbor> Select(const std::vector<Neighbor>& evaluated_neighbors);
    TabuMonitor* tabu_monitor() {
      return _tabu_monitor;
    }

   protected:
    TabuMonitor* _tabu_monitor;
  };
  virtual void ApplyOperation(const Neighbor& neighbor);
};

template <typename Functor>
struct FunctionIntVectorSwapEvaluator : public IntVectorSwap::Evaluator {
  FunctionIntVectorSwapEvaluator(Functor&& func) : callback(std::forward<Functor>(func)) {}
  double Evaluate(const IntVectorSwap::Neighbor& neighbor) override {
    return callback(neighbor);
  }
  Functor callback;
};
template <typename Functor>
std::unique_ptr<FunctionIntVectorSwapEvaluator<Functor>> MakeFunctionIntVectorSwapEvaluator(
    Functor&& func) {
  std::unique_ptr<FunctionIntVectorSwapEvaluator<Functor>> evaluator =
      std::make_unique<FunctionIntVectorSwapEvaluator<Functor>>(std::forward<Functor>(func));
  return evaluator;
}

struct SwapParameters {
  int neighbor_size            = 1;
  int non_improve_limit        = 100;
  int expand_neighbor_limit    = 1;
  double expand_neighbor_ratio = 1.0;
};
OpDef* MakeSwapOp(Solver* solver,
                  IntVectorSwap::Maker* maker,
                  IntVectorSwap::Evaluator* evaluator,
                  IntVectorSwap::Selector* selector,
                  IntVectorSwap* swap,
                  SwapParameters swap_parameters,
                  IntVectorSwap::TabuMonitor* tabu_monitor = nullptr);

class IntVectorTwoOpt : public IntVectorSwap {
 public:
  void ApplyOperation(const IntVectorSwap::Neighbor& neighbor) override;
};
OpDef* MakeTwoOptOp(Solver* solver,
                    IntVectorSwap::Maker* maker,
                    IntVectorSwap::Evaluator* evaluator,
                    IntVectorSwap::Selector* selector,
                    IntVectorTwoOpt* two_opt,
                    SwapParameters swap_parameters,
                    IntVectorSwap::TabuMonitor* tabu_monitor = nullptr);

}  // namespace light_or

#endif