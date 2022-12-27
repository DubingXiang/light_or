/*
 * @Author: dubing.xiang
 * @Date: 2022-07-09 10:43:00
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-07-09 10:43
 * @Description:
 */
#ifndef LIGHT_OR_RANDOM_GENERATOR_H_
#define LIGHT_OR_RANDOM_GENERATOR_H_

#include <algorithm>
#include <map>
#include <random>
#include <thread>
#include <vector>

namespace light_or::util {
/**
 * @brief random class
 *
 **/
class RandomGenerator {
 public:
  RandomGenerator() {
    ResetDefaultSeed();
  }

  RandomGenerator(const RandomGenerator& other) {
    this->_seed      = other._seed;
    this->_generator = other._generator;
  }
  RandomGenerator& operator=(const RandomGenerator& other) {
    this->_seed      = other._seed;
    this->_generator = other._generator;
    return *this;
  }

  inline void SetSeed(long long seed) {
    _seed = seed;
    _generator.seed(_seed);
  }
  inline void ResetDefaultSeed() {
    _seed = 0;
    std::default_random_engine new_default;
    _generator = new_default;
  }

  /**
   * @brief	生成一个[0, 1)之间的double型数
   * @return	random double
   */
  inline double Random() {
    std::uniform_real_distribution<double> distribution;
    return distribution(_generator);
  }

  /**
   * @brief	生成一个[lb, ub)之间的double型数
   * @param	lb
   * @param	ub
   * @return	random double
   */
  inline double RandomDouble(const double lb, const double ub) {
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(_generator);
  }

  /**
   * @brief	生成一个[lb, ub)之间的int型数
   * @param	lb
   * @param	ub
   * @return	random int
   */
  inline int RandomInt(const int lb, const int ub) {
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(_generator);
  }
  // 不允许隐式转换
  inline int RandomInt(const double lb, const double ub) = delete;

  /**
   * @brief	生成一个所有元素在[lb, ub)之间的int型vector
   * @param	size 生成的vector的size
   * @param	lb
   * @param	ub
   * @return	random int vector
   */
  inline std::vector<int> RandomIntVec(const size_t size, const int lb, const int ub) {
    std::vector<int> out;
    out.reserve(size);
    for (size_t i = 0; i < size; ++i) {
      auto rnd = RandomInt(lb, ub);
      out.push_back(rnd);
    }
    return out;
  }
  // 不允许隐式转换
  inline std::vector<int> RandomIntVec(const size_t size,
                                       const double lb,
                                       const double ub) = delete;
  // random a double vector in which every element between [lb, ub)
  /**
   * @brief	生成一个所有元素在[lb, ub)之间的double型vector
   * @param	size 生成的vector的size
   * @param	lb
   * @param	ub
   * @return	random double vector
   */
  inline std::vector<double> RandomDoubleVec(const size_t size, const double lb, const double ub) {
    std::vector<double> out;
    out.reserve(size);
    for (size_t i = 0; i < size; ++i) {
      auto rnd = RandomDouble(lb, ub);
      out.push_back(rnd);
    }
    return out;
  }

  /**
   * @brief 生成一个所有元素在[0, size)之间的int型vector，vector内的所有元素不重复
   * @param size 生成的vector的size
   * @return random int vector
   */
  inline std::vector<int> RandomIntVecNoDuplicate(const size_t size) {
    return RandomIntVecNoDuplicate(size, 0, size);
  }

  /**
   * @brief 生成一个所有元素在[lb, ub) 大小为size的int型vector，vector内的所有元素不重复
   * @param
   * @return
   * @attention
   */
  inline std::vector<int> RandomIntVecNoDuplicate(const size_t size, const int lb, const int ub) {
    if (lb >= ub || static_cast<int>(size) > ub - lb) {
      return std::vector<int>();
    }

    std::vector<int> out(size, 0);

    std::vector<int> sequence(ub - lb, 0);
    for (int i = lb; i < ub; ++i) {
      sequence[i - lb] = i;
    }

    int last = sequence.size();
    last--;
    for (int i = 0; i < size; ++i) {
      const int rnd = RandomInt(0, last + 1);
      out[i]        = sequence[rnd];
      sequence[rnd] = sequence[last];
      --last;
    }
    return out;
  }

  /**
   * @brief	从vec中随机选择一个元素
   * @tparam	vec
   * @return	random T in vec
   */
  template <class T>
  inline T RandomChoice(const std::vector<T>& vec) {
    const int size = vec.size();
    const int idx  = RandomInt(0, size);
    return vec[idx];
  }

  /**
   * @brief	随机选择不重复的k个元素
   * @tparam	v const std::vector<T>&
   * @param	k const int
   * @return
   * @attention
   */
  template <typename T>
  inline std::vector<T> RandomSelectK(const std::vector<T>& v, const int k) {
    if (k >= v.size()) {
      return v;
    }

    std::vector<T> result;
    result.reserve(k);
    // 随机生成下标
    auto random_indices = RandomIntVecNoDuplicate(k, 0, v.size());
    // 将随机生成的下标集合对应的元素添加到result中
    for (const auto& index : random_indices) {
      result.emplace_back(v[index]);
    }

    return result;
  }
  template <template <typename U> typename Container, typename T>
  inline std::vector<T> RandomSelectK(const Container<T>& container, const int k) {
    std::vector<T> vec(container.begin(), container.end());
    return this->RandomSelectK<T>(vec, k);
  }

  /**
   * @brief	随机打乱vec
   * @tparam	vec std::vector<T>&
   * @return
   */
  template <class T>
  inline void RandomShuffle(std::vector<T>& vec) {
    std::shuffle(vec.begin(), vec.end(), _generator);
  }

 private:
  long long _seed;
  std::default_random_engine _generator;
};

inline RandomGenerator* GlobalRandom() {
  static RandomGenerator random;
  return &random;
}

/**
 * @brief	用于多线程的随机数类

 */
class ConcurrentRandomWrapper {
 public:
  ConcurrentRandomWrapper() : _max_thread_num(std::thread::hardware_concurrency()) {
    for (int i = 0; i < _max_thread_num; ++i) {
      _threadId_to_random[i] = RandomGenerator();
    }
  }

  ConcurrentRandomWrapper(int num_thread) : _max_thread_num(std::thread::hardware_concurrency()) {
    int num_actual_thread = num_thread;
    if (num_thread <= 0) {
      num_actual_thread = (std::min)(2, _max_thread_num);
    } else if (num_thread > _max_thread_num) {
      num_actual_thread = _max_thread_num;
    }
    for (int i = 0; i < num_actual_thread; ++i) {
      _threadId_to_random[i] = RandomGenerator();
    }
  }

  RandomGenerator& GetRandom(int thread_id) {
    return _threadId_to_random.at(thread_id);
  }

 private:
  const int _max_thread_num;
  std::map<int, RandomGenerator> _threadId_to_random;
};

}  // namespace light_or::util

#endif