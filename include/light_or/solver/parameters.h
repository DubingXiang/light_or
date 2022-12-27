/*
 * @Author: dubing.xiang
 * @Date: 2022-12-06 20:10:02
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-12-06 20:10
 * @Description:
 */

#ifndef LIGHT_OR_PARAMETERS_H_
#define LIGHT_OR_PARAMETERS_H_
#include <string>
#include <nlohmann/json.hpp>
#include <light_or/util/logging.h>
#include <light_or/util/utils.h>
#include <light_or/solver/solution_selector.h>
#include <light_or/solver/op_selector.h>
#include <light_or/solver/search_stop_monitor.h>

namespace light_or {

struct AlgorithmParameters {
  // ms
  util::Time search_time_limit             = 1_s;
  int iterations_limit                     = 1;
  bool is_use_multi_objective_optimization = false;
  bool is_use_deterministic_optimization   = true;

  std::string search_stop_monitor_strategy =
      std::string{SearchStopMonitorStrategy::DefaultStopPolicy};

  std::string op_select_type             = std::string{OpSelectStrategy::RouletteWheelOpSelector};
  int activated_op_limit                 = 1;
  bool replacement_sampling_activated_op = false;

  std::string current_solution_select_strategy =
      std::string{SolutionSelectStrategy::NSGAIISelector};
  int current_solution_pool_capacity = 1;

  int global_solution_pool_capacity = 1;
  std::string global_solution_pool_keep_strategy =
      std::string{SolutionSelectStrategy::NSGAIISelector};

  int experience_replay_solution_capacity = 0;
  std::string experience_replay_solution_select_strategy =
      std::string{SolutionSelectStrategy::NSGAIISelector};

  // tabu
  int tabu_tenure_limit = 1;

  std::string log_level = util::InfoLevel();

  bool log_objective_results = false;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AlgorithmParameters,
                                   search_time_limit,
                                   iterations_limit,
                                   is_use_multi_objective_optimization,
                                   is_use_deterministic_optimization,
                                   search_stop_monitor_strategy,
                                   op_select_type,
                                   activated_op_limit,
                                   replacement_sampling_activated_op,
                                   current_solution_select_strategy,
                                   current_solution_pool_capacity,
                                   global_solution_pool_capacity,
                                   global_solution_pool_keep_strategy,
                                   experience_replay_solution_capacity,
                                   experience_replay_solution_select_strategy,
                                   log_level,
                                   log_objective_results);

inline AlgorithmParameters DefaultAlgorithmParameters() {
  return AlgorithmParameters();
}

}  // namespace light_or

#endif