#include <filesystem>
#include <CLI/CLI.hpp>
#include <light_or/domain_problems/bin_packing/bin_packing.h>
#include "example_template.h"

#define OPEN_BENCHMARK

int main(int argc, char** argv) {
  ExampleHelper example_helper;
  return example_helper
      .Main<light_or::bin_packing::BinPackingSolver, light_or::bin_packing::BinPackingIOParser>(
          argc,
          argv,
          "########## Solving Bin Packing Problem ##########",
          "########## Finished Solving Bin Packing Problem ##########");
}