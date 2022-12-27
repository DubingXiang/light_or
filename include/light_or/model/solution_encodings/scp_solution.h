/*
 * @Author: dubing.xiang
 * @Date: 2022-07-23 15:00:55
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-07-23 15:00
 * @Description:
 */

#ifndef LIGHT_OR_SCP_SOLUTION_H_
#define LIGHT_OR_SCP_SOLUTION_H_
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <limits>
#include <functional>
#include <light_or/model/solution_def.h>
#include <light_or/util/utils.h>

namespace light_or {
class ColumnId {
 public:
  using ValueType = int64_t;

 public:
  ColumnId();
  ColumnId(const std::string& type_name, ValueType value);
  ValueType value() const {
    return _value;
  }
  const std::string& TypeName() const {
    return _type_name;
  }
  void SetValue(ValueType value) {
    _value = value;
  }
  void SetTypeName(const std::string& type_name) {
    _type_name = type_name;
  }
  bool operator==(const ColumnId& rhs) const;
  bool operator!=(const ColumnId& rhs) const;

 private:
  std::string _type_name;
  ValueType _value;
};

class NodeId {
 public:
  using ValueType = int64_t;

 public:
  NodeId();
  NodeId(const std::string& type_name, ValueType value);
  ValueType value() const {
    return _value;
  }
  const std::string& TypeName() const {
    return _type_name;
  }
  void SetValue(ValueType value) {
    _value = value;
  }
  void SetTypeName(const std::string& type_name) {
    _type_name = type_name;
  }
  bool operator==(const NodeId& rhs) const;
  bool operator!=(const NodeId& rhs) const;

 private:
  std::string _type_name;
  ValueType _value;
};

constexpr ColumnId::ValueType kInvalidColumnIndex = std::numeric_limits<ColumnId::ValueType>::min();
constexpr NodeId::ValueType kInvalidNodeIndex     = std::numeric_limits<NodeId::ValueType>::min();
inline bool IsValid(const ColumnId& column) {
  return column.value() != kInvalidColumnIndex;
}
inline bool IsValid(const NodeId& node) {
  return node.value() != kInvalidNodeIndex;
}

}  // namespace light_or

namespace std {

template <>
struct hash<::light_or::ColumnId> {
  std::size_t operator()(const ::light_or::ColumnId& column) const {
    return ::light_or::util::hash_val(column.TypeName(), column.value());
  }
};
template <>
struct hash<::light_or::NodeId> {
  std::size_t operator()(const ::light_or::NodeId& node) const {
    return ::light_or::util::hash_val(node.TypeName(), node.value());
  }
};

template <>
struct equal_to<::light_or::ColumnId> {
  bool operator()(const ::light_or::ColumnId& lhs, const ::light_or::ColumnId& rhs) const {
    return lhs.TypeName() == rhs.TypeName() && lhs.value() == rhs.value();
  }
};
template <>
struct equal_to<::light_or::NodeId> {
  bool operator()(const ::light_or::NodeId& lhs, const ::light_or::NodeId& rhs) const {
    return lhs.TypeName() == rhs.TypeName() && lhs.value() == rhs.value();
  }
};

template <>
struct less<::light_or::ColumnId> {
  bool operator()(const ::light_or::ColumnId& lhs, const ::light_or::ColumnId& rhs) const {
    return (lhs.TypeName() < rhs.TypeName()) ||
           (lhs.TypeName() == rhs.TypeName() && lhs.value() < rhs.value());
  }
};

template <>
struct less<::light_or::NodeId> {
  bool operator()(const ::light_or::NodeId& lhs, const ::light_or::NodeId& rhs) const {
    return (lhs.TypeName() < rhs.TypeName()) ||
           (lhs.TypeName() == rhs.TypeName() && lhs.value() < rhs.value());
  }
};

}  // namespace std

namespace light_or {
class Model;
class SCPSolutionStorage;
class SCPSolution : public SolutionDef {
 public:
  using NodeSequence = std::vector<NodeId>;
  using Position     = int;
  struct NodePositions {
    std::unordered_map<ColumnId, std::set<Position>> positions_in_column;
  };

 public:
  SCPSolution(const Model* model);
  virtual ~SCPSolution();

  // 会调用子类实现的 ChildClone
  std::unique_ptr<SolutionDef> Clone() const override;
  // TODO clone后类型不是业务子类，所以转型失败
  // 子类需要实现
  virtual std::unique_ptr<SCPSolution> ChildClone() const = 0;
  const char* class_name() const override {
    return "SCPSolution";
  }

  std::pair<int, bool> Insert(ColumnId column, NodeId node, int index);
  std::pair<int, bool> Remove(ColumnId column, int index);

  const NodeSequence& FindNodeSequence(const ColumnId& column) const;
  const NodePositions& PositionsInColumn(const NodeId& node) const;
  const std::set<Position>& PositionsInColumn(const ColumnId& column, const NodeId& node) const;

  template <typename DomainColumn, typename DomainNode>
  std::pair<int, bool> Insert(DomainColumn* column, DomainNode* node, int index) {
    return Insert(column->Id(), node->Id(), index);
  }
  template <typename DomainColumn>
  std::pair<int, bool> Remove(DomainColumn* column, int index) {
    return Remove(column->Id(), index);
  }

  std::unordered_set<ColumnId> GetNonEmptyColumns(const std::string& column_type) const;

  std::vector<NodeId> GetUncoveredNodes() const;

  template <typename DomainNode, typename P>
  std::vector<DomainNode*> DomainNodeSequence(const ColumnId& column, P pred) const {
    std::vector<DomainNode*> result;
    result.reserve(FindNodeSequence(column).size());
    for (const auto& node : FindNodeSequence(column)) {
      result.push_back(pred(node));
    }
    return result;
  }
  template <typename P>
  std::vector<double> NodeSeqSingleFeatureValues(const ColumnId& column, P pred) const {
    std::vector<double> result;
    const auto& nodes = FindNodeSequence(column);
    for (const auto& node : nodes) {
      result.push_back(pred(node));
    }
    return result;
  }
  template <typename P>
  double NodeSeqSumFeatureValue(const ColumnId& column, P pred) const {
    double result     = 0.0;
    const auto& nodes = FindNodeSequence(column);
    for (const auto& node : nodes) {
      result + pred(node);
    }
    return result;
  }
  template <typename P>
  std::vector<double> NodeSeqConnectionFeatureValues(const ColumnId& column, P pred) const {
    std::vector<double> result;
    const auto& nodes = FindNodeSequence(column);
    for (int i = 0; i + 1 < nodes.size(); ++i) {
      result.push_back(pred(nodes[i + 1], nodes[i]));
    }
    return result;
  }
  template <typename P>
  double NodeSeqSumConnectionFeatureValue(const ColumnId& column, P pred) const {
    double result;
    const auto& nodes = FindNodeSequence(column);
    for (int i = 0; i + 1 < nodes.size(); ++i) {
      result += pred(nodes[i + 1], nodes[i]);
    }
    return result;
  }

 private:
  std::unique_ptr<SCPSolutionStorage> _storage;

  SCPSolutionStorage* storage() const {
    return _storage.get();
  }

 public:
  using UnCoverInfoCallback =
      std::function<std::map<std::string, int>(const SCPSolutionStorage&, const NodeId&)>;
  void RegisterUncoverInfoCallback(UnCoverInfoCallback callback);
};

#define DECLARE_COLUMNID_PROPERTY()                            \
 public:                                                       \
  const ::light_or::ColumnId& Id() const { return _id; }       \
  void SetId(::light_or::ColumnId id) { _id = std::move(id); } \
                                                               \
 private:                                                      \
  ::light_or::ColumnId _id;

#define DECLARE_NODEID_PROPERTY()                            \
 public:                                                     \
  const ::light_or::NodeId& Id() const { return _id; }       \
  void SetId(::light_or::NodeId id) { _id = std::move(id); } \
                                                             \
 private:                                                    \
  ::light_or::NodeId _id;

}  // namespace light_or

#endif