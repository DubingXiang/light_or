/*
 * @Author: dubing.xiang
 * @Date: 2022-11-11 12:21:02
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-11-11 12:21
 * @Description:
 */

#ifndef LIGHT_OR_ALGORITHMS_H_
#define LIGHT_OR_ALGORITHMS_H_
#include <vector>
#include <set>
#include <algorithm>
#include <string>
namespace light_or::util {
class RouletteWheelSelection {
 public:
  std::vector<int> Select(const std::vector<int>& elements,
                          const std::vector<double>& score_list,
                          int select_size,
                          bool replacement_sampling = false) const;
  int SelectOne(const std::vector<int>& elements, const std::vector<double>& score_list) const;
};

class NonDominatedSorter {
 public:
  void Compute(int solution_count,
               const std::vector<std::vector<double>>& solution_objective_values,
               const std::vector<bool>& is_minimize_list);
  enum Status { strong_dominate, strong_dominated, weak_dominate, weak_dominated, non_dominated };
  static Status Compare(const std::vector<double>& objective_values_1,
                        const std::vector<double>& objective_values_2,
                        const std::vector<bool>& is_minimize_list);

  const std::vector<std::vector<int>>& level_solution_indices() const {
    return _level_solution_indices;
  }

 private:
  std::vector<std::vector<int>> _level_solution_indices;
};
class CrowdingDistance {
 public:
  void Compute(int solution_count,
               const std::vector<std::vector<double>>& solution_objective_values,
               const std::vector<bool>& is_minimize_list);
  const std::vector<double>& crowding_distance_list() const {
    return _crowding_distance_list;
  }

 private:
  std::vector<double> _crowding_distance_list;
};

inline std::vector<std::vector<int>> Combination(int n, int k) {
  std::string bitmask(k, 1);  // k leading 1's
  bitmask.resize(n, 0);       // n-k trailing 0's
  std::vector<std::vector<int>> combinations;
  do {
    std::vector<int> comb(k);
    int j = 0;
    for (int i = 0; i < n; ++i) {
      if (bitmask[i]) {
        comb[j++] = i;
      }
    }
    combinations.push_back(comb);
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

  return combinations;
};

};  // namespace light_or::util

#endif