/*
 * @Author: dubing.xiang
 * @Date: 2022-11-11 15:56:18
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-11-11 15:56
 * @Description:
 */

#ifndef LIGHT_OR_SOLUTION_SELECTOR_H_
#define LIGHT_OR_SOLUTION_SELECTOR_H_
#include <vector>
#include <functional>
#include <string>
#include <light_or/util/factory.h>
#include <light_or/solver/solution_pool.h>

namespace light_or {

class SolutionSelector {
 public:
  virtual ~SolutionSelector()                                = default;
  virtual void Evaluating(const SolutionPool& solution_pool) = 0;
  virtual std::vector<int> SelectingIndices(const SolutionPool& solution_pool, int select_size) = 0;
  SolutionPool Selecting(const SolutionPool& solution_pool, int select_size);
};

// return: selected indices
// arg:fitness_list,select size
using FitnessBasedSelectorCallback =
    std::function<std::vector<int>(const std::vector<double>&, int)>;
class FitnessBasedSelector : public SolutionSelector {
 public:
  FitnessBasedSelector();
  FitnessBasedSelector(FitnessBasedSelectorCallback callback);
  void Evaluating(const SolutionPool& solution_pool) override {}
  std::vector<int> SelectingIndices(const SolutionPool& solution_pool, int select_size) override;
  void set_callback(FitnessBasedSelectorCallback callback);

 private:
  FitnessBasedSelectorCallback _callback;
};

using SolutionSelectorFactory = util::Factory<SolutionSelector>;

#define DECLARE_SOLUTION_SELECTOR_STRATEGY(strategy_name) \
  static constexpr std::string_view strategy_name = #strategy_name;
struct SolutionSelectStrategy {
  DECLARE_SOLUTION_SELECTOR_STRATEGY(NSGAIISelector);
  DECLARE_SOLUTION_SELECTOR_STRATEGY(FitnessBasedSelector);
};

#undef DECLARE_SOLUTION_SELECTOR_STRATEGY

}  // namespace light_or

#endif