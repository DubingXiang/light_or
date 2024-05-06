/*
 * @Author: zheming.zhang
 * @Date: 2024-03-23 22:11:12
 * @Last Modified by: zheming.zhang
 * @Last Modified time: 2024-03-23 22:11
 * @Description: scheduling column generation solver
 */
#ifndef LIGHT_OR_SCHEDULING_CG_H_
#define LIGHT_OR_SCHEDULING_CG_H_

#include <vector>
#include <memory>
#include <optional>
#include <queue>
#include <light_or/domain_problems/scheduling/scheduling.h>


namespace light_or::scheduling {

  class AgentColumn {
 public:
  AgentColumn(int agent, const std::vector<int>& tasks) : agent_id(agent), task_ids(tasks) {}
  AgentColumn(int agent, std::vector<int>&& tasks) : agent_id(agent), task_ids(tasks) {}
  ~AgentColumn() { task_ids.clear();}

 public:
  int agent_id = 0;
  std::vector<int> task_ids;
};

class AgentColumnPool {
 public:
  AgentColumnPool()  = default;
  ~AgentColumnPool() = default;

  void MainTainColPool();
  inline void AddCols(std::shared_ptr<AgentColumn> col) { cols.push_back(col); }
  std::vector<std::shared_ptr<AgentColumn>> GetColsByAgentId(int agent_id);

public:
  std::vector<std::shared_ptr<AgentColumn>> cols;
};


struct NetRuleParam {
  int max_connect_time = 30;      // 先设置一个最大连接时间，防止网络过大
  int work_time_mean   = 8 * 60;  // 平均工作时间
  int process_time_upper = 999999; // 赛题中规定的处理时长上限（表示无法处理）
};

class Label{
  public:
    Label(int cur_node_id_, std::shared_ptr<Label> prev_label_, double cum_task_time_, double cum_pi_);
    ~Label() = default;

    int id = -1;
    int cur_node_id = -1;
    std::shared_ptr<Label> prev_label = nullptr; // 前继标签
    double cum_task_time = 0.0;
    double cum_pi = 0.0;
    double reduced_cost = 0.0; // cum_task_time - cum_pi

    // 重载 <= 运算符作为成员函数  
    bool operator<(const Label& other) const { return reduced_cost < other.reduced_cost; }  
    bool operator>(const Label& other) const { return reduced_cost > other.reduced_cost; }  
    bool operator<=(const Label& other) const { return reduced_cost <= other.reduced_cost; }  
    bool operator>=(const Label& other) const { return reduced_cost >= other.reduced_cost; }  
    bool operator==(const Label& other) const { return abs(reduced_cost - other.reduced_cost) < 1e-6; }  

     // 比较函数对象，用于最大优先队列。每次弹出reduced_cost最大的 Label
    struct PQCompare {  
        bool operator()(const std::shared_ptr<Label>& a, const std::shared_ptr<Label>& b) const {  
            return a->reduced_cost < b->reduced_cost;  
        }  
    };  

  private:
    static int _label_cnt; 
};

class Node {
 public:
  Node(int id_, int start_time_, int task_type_, Task* task_, std::string node_type_)
      : id(id_), start_time(start_time_), task_type(task_type_), task(task_), node_type(node_type_)  {}

  void AddLabel(std::shared_ptr<Label> label);
  void MaintainLabels(int remain_label_cnt = 10);

 public:
  int id = -1;
  int start_time = -1;
  int task_type = -1;
  Task* task = nullptr;
  std::string node_type = "task"; // source, task, sink
  std::shared_ptr<Label> best_label = nullptr; // 最优标签
  // std::unordered_map<int, std::shared_ptr<Label>> id_2_feasible_label; // 可行标签
  // 创建最大优先队列，存储Label指针，使用自定义比较对象  
  std::priority_queue<std::shared_ptr<Label>, std::vector<std::shared_ptr<Label>>, Label::PQCompare> feasible_labels_pq;

  
  // std::unordered_map<std::string, double> pi_name_2_vals;
  // const std::string cover_task_pi_name = "CoverTaskPi";
   

};

class Arc {
 public:
  Arc (std::shared_ptr<Node> start_node_, std::shared_ptr<Node> end_node_, std::string arc_type_)
      : start_node(start_node_), end_node(end_node_), arc_type(arc_type_) {}
 public:
  std::shared_ptr<Node> start_node = nullptr;
  std::shared_ptr<Node> end_node = nullptr;
  std::string arc_type = "task_task"; // source_task, task_task, task_sink
};

class Path{
  public:
    Path(const std::vector<std::shared_ptr<Node>>& nodes_) : nodes(nodes_) { cost = CalReduceCost();}
    ~Path() {nodes.clear();}

  public:
    std::vector<std::shared_ptr<Node>> nodes;
    double cost = 0.0;

    double CalReduceCost() const {
      //遍历nodes
      return 0.0;
    }
};

class Net {
  public:
    Net() = default;
    ~Net() = default;

    void AddNode(std::shared_ptr<Node> node);
    void AddArc(std::shared_ptr<Arc> arc);

  public:
    std::shared_ptr<Node> source = nullptr;
    std::shared_ptr<Node> sink = nullptr;
    std::vector<std::shared_ptr<Node>> nodes;    // 点
    std::vector<std::shared_ptr<Arc>> arcs;              // 弧
    std::unordered_map<int, std::shared_ptr<Node>> nid_2_node; 
    std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Arc>>> nid_2_nid_2_arcs;
    std::unordered_map<int, std::vector<std::shared_ptr<Arc>>> nid_2_out_arcs;  // 点出弧
    std::unordered_map<int, std::vector<std::shared_ptr<Arc>>> nid_2_in_arcs;   // 点入弧
    std::unordered_map<std::string, std::unordered_map<int, double>> pi_name_2_task_id_2_pi;
    const std::string cover_task_pi_name = "CoverTaskPi";
};


class SchedulingCGSolver {
 public:
  SchedulingCGSolver(bool deterministic): _deterministic(deterministic) {}
  ~SchedulingCGSolver() = default;

  void Solve(SchedulingProblem* problem, const AlgorithmParameters& parameters);
  
  // bool Solve(SchedulingProblem* problem, const AlgorithmParameters& parameters);
  const SchedulingSolution* BestSolution() const { return nullptr; }
  
  enum ObjectiveStatsType { global_best, current_best };
  std::string ExportObjectiveStats(ObjectiveStatsType type) const { return "1";}
  // void ExportObjectiveStats(ObjectiveStatsType type, const std::string& file) const;

 private:
  // SchedulingProblem* _problem                        = nullptr;
  // std::unique_ptr<light_or::Solver> _solver    = nullptr;
  // std::unique_ptr<SchedulingSolution> _init_solution = nullptr;
  const SchedulingSolution* _best_solution           = nullptr;
  bool _deterministic = false;
  // util::RandomGenerator _random;
};
}  // namespace light_or::scheduling

#endif  // LIGHT_OR_SCHEDULING_CG_H_