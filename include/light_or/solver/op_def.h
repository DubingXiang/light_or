/*
 * @Author: dubing.xiang
 * @Date: 2022-06-25 16:33:42
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-25 16:33
 * @Description: define operator class
 */
#ifndef LIGHT_OR_OP_DEF_H_
#define LIGHT_OR_OP_DEF_H_
#include <memory>
#include <string>
#include <light_or/model/solution_def.h>
#include <light_or/solver/solution_pool.h>

namespace light_or {
class Solver;
class OpDef {
 public:
  OpDef(Solver* const solver);
  virtual ~OpDef();
  virtual void BeforeSearch();
  virtual std::vector<std::unique_ptr<SolutionDef>> Searching(
      const SolutionPool& solution_pool) = 0;
  virtual void AfterSearch();

  virtual const char* class_name() const = 0;
  const std::string& name() const;
  void set_name(const std::string& name);
  Solver* const solver() const {
    return _solver;
  }

 protected:
  Solver* const _solver;
  std::string _name;
};

template <class Functor>
class FunctionOp : public OpDef {
 public:
  FunctionOp(Solver* const solver, Functor&& func) : OpDef(solver), _callback(std::move(func)) {}
  std::vector<std::unique_ptr<SolutionDef>> Searching(const SolutionPool& solution_pool) override {
    return _callback(solution_pool);
  }
  const char* class_name() const override {
    return "FunctionOp";
  }

  void SetFunction(Functor&& func) {
    _callback = std::move(func);
  }

 private:
  Functor _callback;
};

template <typename Functor>
std::unique_ptr<FunctionOp<Functor>> MakeFunctionOp(Solver* const solver,
                                                    Functor&& func,
                                                    std::string name) {
  std::unique_ptr<FunctionOp<Functor>> func_op(new FunctionOp<Functor>(solver, std::move(func)));
  func_op->set_name(name);
  return func_op;
}
}  // namespace light_or

#endif