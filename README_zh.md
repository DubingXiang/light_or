# light_or

## 目录
*   [简介](#introduction)
*   [组件](#components)
*   [安装](#installation)
*   [Benchmarks](#benchmarks)
*   [Quick Start](#quick-start)
*   [联系方式](#contacts)
*   [License](#license)
## 简介
light_or is a tool to help develop operational optimization algorithms to solve combinatorial optimization problems.  

## 组件

*   [bin](bin) 存放二进制文件。
*   [examples](examples) 案例集的根目录。
    *   [config](examples/config) 存放算法参数配置文件。
    *   [data](examples/data) 问题测试数据集。
    *   [analyze.py](examples/analyze.py) 算法结果分析。
    *   [example_template.h](examples/example_template.h) 标准化开发一个 example 的流程。
    *   [qap_example.cc](examples/qap_example.cc) 二次分配问题的 example 求解。
    *   [test_instances.py](examples/test_instances.py) 批量运行examples。
    *   [test_qap.py](examples/test_qap.py)  批量运行 QAP examples。
    *   [xmake.lua] xmake 编译脚本。
*   [include/light_or](include/light_or) 存放头文件。
    *   [base](include/light_or/base) 主要和cpp的一些技巧相关的 util。
    *   [domain_problems](include/light_or/domain_problems) 内置的领域问题的求解器。
    *   [model](include/light_or/model) 模型的核心组成。
        *   [solution_encodings](include/light_or/model/solution_encodings) 解的编码。
    *   [solver](include/light_or/solver) 求解器的核心组成。
        *   [operators](include/light_or/solver/operators) 算子集合。
    *   [util](include/light_or/util) utilities，相比于 base ，更加上层。
*   [lib](lib) 存放静态库文件。
*   [src](src) 存放源文件，是一个 submodule。


## 安装
LightOR 基于 xmake 编译。
## Benchmarks
[quadratic assignment problem](https://github.com/DubingXiang/light_or-docs/blob/master/benchmarks/qap/README.md)

## Quick Start
TODO
## 联系方式
- email:   	    
	- karupin22@163.com
- source:
    - [github](https://github.com/DubingXiang/light_or)    
## License
该项目基于 MIT 开源协议. 详见 [LICENSE](https://github.com/DubingXiang/light_or/blob/master/LICENSE)。