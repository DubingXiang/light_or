/*
 * @Author: dubing.xiang
 * @Date: 2022-12-10 15:58:02
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-12-10 15:58
 * @Description:
 */

#ifndef LIGHT_OR_INT_VECTOR_H_
#define LIGHT_OR_INT_VECTOR_H_
#include <vector>
#include <light_or/model/solution_def.h>
namespace light_or {

using IntVector = std::vector<int64_t>;

class IntVectorSolution : public SolutionDef {
 public:
  IntVectorSolution(const Model* model);
  virtual ~IntVectorSolution();
  std::unique_ptr<SolutionDef> Clone() const override;
  virtual std::unique_ptr<IntVectorSolution> ChildClone() const = 0;

  const char* class_name() const override {
    return "IntVectorSolution";
  }
  const std::vector<IntVector>& Encodings() const;
  std::vector<IntVector>* MutableEncodings();
  int Shape() const;
  const IntVector* EncodingOrNull(int index) const;
  IntVector* MutableEncodingOrNull(int index);
  int EncodingShape(int index) const;
  std::string FormatStr() const;

 protected:
  std::vector<IntVector> _encodings;
};

}  // namespace light_or

#endif