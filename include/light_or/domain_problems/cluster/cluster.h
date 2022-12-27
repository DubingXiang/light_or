
/*
 * @Author: dubing.xiang
 * @Date: 2022-10-23 15:30:07
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-10-23 15:30
 * @Description:
 */
#ifndef LIGHT_OR_CLUSTER_SOLUTION_H_
#define LIGHT_OR_CLUSTER_SOLUTION_H_
#include <unordered_map>
#include <memory>
#include <light_or/model/solution_def.h>
#include <light_or/model/solution_encodings/scp_solution.h>
#include <light_or/model/model.h>
namespace light_or::clustering {
class Cluster {
 public:
  DECLARE_COLUMNID_PROPERTY();

 public:
  // std::vector<double> values;
};

class Node {
 public:
  DECLARE_NODEID_PROPERTY();

 public:
  std::vector<double> values;
};
class ClusterProblem : public light_or::Model {
  friend class ClusterSolution;

 public:
  Cluster* ClusterOrNull(const ColumnId& id) const {
    auto itor = _clusters.find(id);
    return itor == _clusters.end() ? nullptr : itor->second.get();
  }
  Node* NodeOrNull(const NodeId& id) const {
    auto itor = _nodes.find(id);
    return itor == _nodes.end() ? nullptr : itor->second.get();
  }
  bool AddCluster(std::unique_ptr<Cluster> cluster) {
    return _clusters.insert({cluster->Id(), std::move(cluster)}).second;
  }
  bool AddNode(std::unique_ptr<Node> node) {
    return _nodes.insert({node->Id(), std::move(node)}).second;
  }
  const std::unordered_map<NodeId, std::unique_ptr<Node>>& nodes() const {
    return _nodes;
  }
  const std::unordered_map<ColumnId, std::unique_ptr<Cluster>>& clusters() const {
    return _clusters;
  }

 private:
  std::unordered_map<ColumnId, std::unique_ptr<Cluster>> _clusters;
  std::unordered_map<NodeId, std::unique_ptr<Node>> _nodes;
};
class ClusterSolution : public SCPSolution {
 public:
  ClusterSolution(ClusterProblem* problem) : SCPSolution(problem), _problem(problem) {}
  std::unique_ptr<SCPSolution> ChildClone() const override {
    std::unique_ptr<ClusterSolution> copy(new ClusterSolution(this->_problem));
    copy->_cluster_locations = this->_cluster_locations;
    return copy;
  }

  Cluster* ClusterOrNull(const ColumnId& id) const {
    return _problem->ClusterOrNull(id);
  }
  Node* NodeOrNull(const NodeId& id) const {
    return _problem->NodeOrNull(id);
  }
  bool AddCluster(std::unique_ptr<Cluster> cluster) {
    return _problem->AddCluster(std::move(cluster));
  }
  bool AddNode(std::unique_ptr<Node> node) {
    return _problem->AddNode(std::move(node));
  }
  const std::unordered_map<NodeId, std::unique_ptr<Node>>& nodes() const {
    return _problem->nodes();
  }
  const std::unordered_map<ColumnId, std::unique_ptr<Cluster>>& clusters() const {
    return _problem->clusters();
  }
  using ClusterLocation = std::vector<double>;
  std::unordered_map<ColumnId, ClusterLocation>& cluster_locations() {
    return _cluster_locations;
  }
  const std::unordered_map<ColumnId, ClusterLocation>& cluster_locations() const {
    return _cluster_locations;
  }

 private:
  std::unordered_map<ColumnId, ClusterLocation> _cluster_locations;
  ClusterProblem* _problem;
};

}  // namespace light_or::clustering

#endif  // LIGHT_OR_CLUSTER_SOLUTION_H_