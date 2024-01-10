/*
 * @Author: dubing.xiang
 * @Date: 2024-01-10 22:54:12
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2024-01-10 22:54
 * @Description: 图论相关算法
 */

#ifndef LIGHT_OR_GRAPH_ALGORITHMS_H_
#define LIGHT_OR_GRAPH_ALGORITHMS_H_

#include <vector>
#include <queue>
#include <numeric>
#include <limits>
#include <cstdint>
#include <memory>

namespace light_or::graph {

// 默认最大权重
class LinearSumAssignment {
 public:
  using NodeIndex                               = int64_t;
  using CostValue                               = int64_t;
  using Graph                                   = std::vector<std::vector<CostValue>>;
  inline static const NodeIndex kNodeIndexLimit = 1e6;
  inline static const NodeIndex kNoNode         = -1;
  inline static const CostValue kCostValueLimit = std::numeric_limits<CostValue>::max() / 2;

 public:
  LinearSumAssignment(const Graph& graph);
  ~LinearSumAssignment();

  void Matching();
  CostValue GetTotalCost() const {
    return _total_cost;
  }

  struct Match {
    Match(NodeIndex l, NodeIndex r) : left(l), right(r) {}
    NodeIndex left;
    NodeIndex right;
  };
  const std::vector<Match>& GetMatches() const {
    return _matches;
  }
  NodeIndex MatchedRight(NodeIndex left) const;
  NodeIndex MatchedLeft(NodeIndex right) const;

 private:
  class Impl;
  std::unique_ptr<Impl> _impl;
  CostValue _total_cost{};
  std::vector<Match> _matches;
};

}  // namespace light_or::graph

#endif