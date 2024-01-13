/*
 * @Author: dubing.xiang
 * @Date: 2024-01-07 11:11:12
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2024-01-07 11:11
 * @Description: scheduling solver
 */
#ifndef LIGHT_OR_SCHEDULING_H_
#define LIGHT_OR_SCHEDULING_H_

#include <vector>
#include <memory>
#include <optional>

#include <light_or/model/model.h>
#include <light_or/model/solution_encodings/int_vector_solution.h>
#include <light_or/util/solver_template.h>

namespace light_or {
class Solver;
class AlgorithmParameters;
}  // namespace light_or

namespace light_or::scheduling {
class Task {
 public:
  Task(int id, int generate_time, int type, int max_response_time)
      : _id(id)
      , _generate_time(generate_time)
      , _type(type)
      , _max_response_time(max_response_time) {}
  int Id() const {
    return _id;
  }
  int generate_time() const {
    return _generate_time;
  }
  int type() const {
    return _type;
  }
  int max_response_time() const {
    return _max_response_time;
  }

 private:
  int _id;
  int _generate_time;
  int _type;
  int _max_response_time;
};
class Agent {
 public:
  Agent(int id, const std::vector<int>& process_duration)
      : _id(id), _process_task_duration(process_duration) {}
  int Id() const {
    return _id;
  }

  int ProcessDuration(int taskType) const;

 private:
  int _id;
  std::vector<int> _process_task_duration;
};

class SchedulingProblem : public light_or::Model {
 public:
  SchedulingProblem(std::vector<std::unique_ptr<Agent>> agents,
                    std::vector<std::unique_ptr<Task>> tasks);
  const std::vector<std::unique_ptr<Agent>>& Agents() const {
    return _agents;
  }
  const std::vector<std::unique_ptr<Task>>& Tasks() const {
    return _tasks;
  }

 public:
  inline static const int kMinTimeStep = 1;  // 决策的时间离散单元，单位为分钟
 private:
  std::vector<std::unique_ptr<Agent>> _agents;
  std::vector<std::unique_ptr<Task>> _tasks;
};
class SchedulingSolution : public light_or::IntVectorSolution {
 public:
  SchedulingSolution(const SchedulingProblem* problem);
  std::unique_ptr<IntVectorSolution> ChildClone() const override;
  const char* class_name() const override {
    return "SchedulingSolution";
  }
  const SchedulingProblem* problem() const;

  std::optional<int> AssignedProcessBeginTime(int agent, int task) const;
  std::optional<int> AssignedProcessDuration(int agent, int task) const;
  const std::unordered_map<int, int>& ProcessBeginTimes(int agent) const;
  const std::unordered_map<int, int>& AssignedProcessDurations(int agent) const;

  void Assign(int agent, int task, int process_begin_time, int process_duration);

 private:
  std::unordered_map<int, std::unordered_map<int, int>> _agent_to_task_process_begin_time;
  std::unordered_map<int, std::unordered_map<int, int>> _agent_to_task_process_duration;
};

class SchedulingSolver
    : public light_or::util::SolverTemplate<SchedulingProblem, SchedulingSolution> {
 public:
  SchedulingSolver(bool deterministic);
  void Modeling() override;
  void InitialSolution(bool use_random = false) override;
};

class SchedulingIOParser {
 public:
  void Parse(const std::string& data_dir);
  std::string Parse(const SchedulingSolution& solution);
  SchedulingProblem* problem() const {
    return _problem.get();
  }

 private:
  std::unique_ptr<SchedulingProblem> _problem = nullptr;
};

}  // namespace light_or::scheduling

#endif  // LIGHT_OR_SCHEDULING_H_