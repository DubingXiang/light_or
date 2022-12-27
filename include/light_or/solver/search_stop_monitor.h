/*
 * @Author: dubing.xiang
 * @Date: 2022-06-29 21:44:10
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-29 21:44
 * @Description: define class to control is time to stop the search
 */
#ifndef LIGHT_OR_SEARCH_STOP_POLICY_H_
#define LIGHT_OR_SEARCH_STOP_POLICY_H_
#include <memory>
#include <light_or/util/macros.h>
#include <light_or/util/factory.h>

namespace light_or {
class Solver;
class SearchStopMonitor {
 public:
  SearchStopMonitor(Solver* solver);
  virtual ~SearchStopMonitor();
  Solver* solver() const;
  virtual bool Stop()              = 0;
  virtual const char* name() const = 0;

 protected:
  Solver* _solver;
};

using SearchStopMonitorFactory = util::Factory<SearchStopMonitor, Solver*>;

#define DECLARE_SEARCH_STOP_MONITOR_SELECTOR_STRATEGY(strategy_name) \
  static constexpr std::string_view strategy_name = #strategy_name;
struct SearchStopMonitorStrategy {
  DECLARE_SEARCH_STOP_MONITOR_SELECTOR_STRATEGY(DefaultStopPolicy);
  DECLARE_SEARCH_STOP_MONITOR_SELECTOR_STRATEGY(IterationCountLimit);
  DECLARE_SEARCH_STOP_MONITOR_SELECTOR_STRATEGY(TimeLimit);
};

#undef DECLARE_SEARCH_STOP_MONITOR_SELECTOR_STRATEGY
extern void initialization();
}  // namespace light_or

#endif