#include <filesystem>
#include <CLI/CLI.hpp>
#include <light_or/domain_problems/qap/qap.h>
#include "example_template.h"

#define OPEN_BENCHMARK

int main(int argc, char** argv) {
  ExampleHelper example_helper;
  return example_helper.Main<light_or::qap::QAPSolver, light_or::qap::QAPIOParser>(
      argc,
      argv,
      "########## Solving Quadratic Assignment Problem ##########",
      "########## Finished Quadratic Assignment Problem ##########");
}