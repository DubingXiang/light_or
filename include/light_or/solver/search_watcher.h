/*
 * @Author: dubing.xiang
 * @Date: 2022-10-27 08:41:40
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-10-27 08:41
 * @Description:
 */

#ifndef _SEARCH_WATCHER_H_
#define _SEARCH_WATCHER_H_
#include <string>
#include <light_or/base/base_object.h>

namespace light_or {
class Solver;
class SearchWatcher : public BaseObject {
 public:
  SearchWatcher(Solver* solver, const std::string& name);
  virtual ~SearchWatcher() {}
  virtual const char* class_name() const = 0;
  virtual void BeforeSolve() {}
  virtual void AfterSolve() {}
  virtual void BeforeIteration() {}
  virtual void AfterIteration() {}

  void Install();
  Solver* solver() const;
  const std::string& name() const;

 protected:
  Solver* _solver;
  std::string _name;
};

class SearchAnalyzer;
SearchWatcher* MakeSearchAnalyzer(Solver* solver);

}  // namespace light_or

#endif