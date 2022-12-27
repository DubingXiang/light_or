/*
 * @Author: dubing.xiang
 * @Date: 2022-11-28 14:16:09
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-11-28 14:16
 * @Description:
 */

#ifndef LIGHT_OR_IO_H_
#define LIGHT_OR_IO_H_
#include <string>

namespace light_or::util {

class FileIO {
 public:
  FileIO()  = default;
  ~FileIO() = default;

  static std::string ReadFromFile(const std::string& file_name);
  static void WriteToFile(const std::string& file_name, const std::string& contents, bool is_app = false);
};

}  // namespace light_or::util

#endif