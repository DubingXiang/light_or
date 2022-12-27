/*
 * @Author: dubing.xiang
 * @Date: 2022-07-09 09:48:52
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-07-09 09:48
 * @Description:
 */

#ifndef LIGHT_OR_TIMER_H_
#define LIGHT_OR_TIMER_H_

#include <chrono>

namespace light_or::util {

class Timer {
 public:
  Timer() : _elapsed(0) {}
  ~Timer() {}

  void Start() {
    _begin_time = std::chrono::high_resolution_clock::now();
  }
  void Stop() {
    auto duration = std::chrono::high_resolution_clock::now() - _begin_time;
    _elapsed += std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
  }
  void Restart() {
    _elapsed = 0;
    Start();
  }
  double GetUs() const {
    return static_cast<double>(_elapsed);
  }
  double GetMs() const {
    return _elapsed * 0.001;
  }
  double GetSec() const {
    return _elapsed * 0.000001;
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> _begin_time;
  long long _elapsed;
};

struct TimeMeasure {
  template <typename Func, typename... Args>
  double Run(Func&& func, Args&&... args) {
    Timer sw;
    sw.Start();
    func(std::forward<Args>(args)...);
    sw.Stop();
    return sw.GetMs();
  }
};

#define SHOW_TIME_ELAPSED(Func, ...)                                            \
  []() {                                                                        \
    auto time_slapsed = ::light_or::util::TimeMeasure().Run(Func, __VA_ARGS__); \
    LOG(INFO) << "func:" << #Func << ",time elapsed ms:" << time_elapsed;       \
  }();

}  // namespace light_or::util

#endif