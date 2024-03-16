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
  // 任务产生时刻
  int generate_time() const {
    return _generate_time;
  }
  // 问题类型。不同执行体执行不同类型的任务的匹配成本（如时间、偏好等）存在差异
  int type() const {
    return _type;
  }
  // 最大响应时长
  int max_response_time() const {
    return _max_response_time;
  }

 private:
  int _id;
  int _generate_time;
  int _type;
  int _max_response_time;
};
// 执行体。可以是人、机器等。作为执行任务的对象
class Agent {
 public:
  Agent(int id, int input_id, const std::vector<int>& process_duration)
      : _id(id), _input_id(input_id), _process_task_duration(process_duration) {}
  int Id() const {
    return _id;
  }
  void SetId(int id) {
    _id = id;
  }
  // 输入的 id
  int input_id() const {
    return _input_id;
  }
  // 该 agent 处理 该类任务的时长
  int ProcessDuration(int taskType) const;

 private:
  int _id;
  std::vector<int> _process_task_duration;
  int _input_id;  // agent 会拷贝
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
  const Agent& GetAgent(int index) const {
    return *(_agents.at(index));
  }
  const Task& GetTask(int index) const {
    return *(_tasks.at(index));
  }

 public:
  inline static const int kMinTimeStep = 1;  // 决策的时间离散单元，单位为分钟
 private:
  std::vector<std::unique_ptr<Agent>> _agents;
  std::vector<std::unique_ptr<Task>> _tasks;
};
// 继承了 light_or::IntVectorSolution 类
// 编码为矩阵类的编码，即 light_or::IntVectorSolution 的 encodings 为一个二维数组（矩阵）
// 在该问题中，第一维表示任务（索引），第二维表示任务是否被分配给agent。若分配，则值为1，否则为0
// 例如：若 Encoding(1) == {1,0,0,1,0} 则表示一共存在5个agent，任务 1 被分配给了 agent 0 和 3
// 同时 SchedulingSolution 也反向存储了 agent 分配的任务集合
class SchedulingSolution : public light_or::IntVectorSolution {
 public:
  SchedulingSolution(const SchedulingProblem* problem);
  std::unique_ptr<IntVectorSolution> ChildClone() const override;
  const char* class_name() const override {
    return "SchedulingSolution";
  }
  const SchedulingProblem* problem() const;
  // agent 开始处理 task 的时刻
  // 若查找失败，则返回 std::nullopt
  std::optional<int> AssignedProcessBeginTime(int agent, int task) const;
  // agent 在 task 上分配的处理时长
  // 若查找失败，则返回 std::nullopt
  std::optional<int> AssignedProcessDuration(int agent, int task) const;
  // agent 所有分配的 task 的开始处理时刻
  const std::unordered_map<int, int>& ProcessBeginTimes(int agent) const;
  // agent 所有分配的 task 的处理时长
  const std::unordered_map<int, int>& AssignedProcessDurations(int agent) const;
  // 分配 task 给 agent，且开始处理时刻为 process_begin_time，处理时长为 process_duration
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