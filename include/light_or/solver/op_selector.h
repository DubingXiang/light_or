/*
 * @Author: dubing.xiang
 * @Date: 2022-10-27 08:37:49
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-10-27 08:37
 * @Description:
 */

#ifndef LIGHT_OR_OP_SELECTOR_H_
#define LIGHT_OR_OP_SELECTOR_H_
#include <vector>
#include <memory>
#include <light_or/util/factory.h>

namespace light_or {
class OpDef;
class Solver;
class OpSelector {
 public:
  OpSelector(Solver* solver);
  virtual ~OpSelector();
  const std::vector<OpDef*>& ActivatedOps();
  virtual void Evaluating() = 0;
  virtual void Selecting()  = 0;
  Solver* solver() const;

 protected:
  Solver* _solver;
  std::vector<OpDef*> _activated_ops;
};

using OpSelectorFactory = util::Factory<OpSelector, Solver*>;

#define DECLARE_OP_SELECTOR_STRATEGY(strategy_name) \
  static constexpr std::string_view strategy_name = #strategy_name;
struct OpSelectStrategy {
  DECLARE_OP_SELECTOR_STRATEGY(RouletteWheelOpSelector);
};

#undef DECLARE_OP_SELECTOR_STRATEGY

}  // namespace light_or

#endif