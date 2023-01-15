# light_or

## Table of Contents
*   [Introduction](#introduction)
*   [Components](#components)
*   [Installation](#installation)
*   [Benchmarks](#benchmarks)
*   [Quick Start](#quick-start)
*   [Contacts](#contacts)
*   [License](#license)
## Introduction ([中文](/README_zh.md))
light_or is a tool that help you develop Operational Research algorithms to solve combinatorial optimization problems.  

## Components

*   [bin](bin) Store binary files.
*   [examples](examples) Root directory for all examples.
    *   [config](examples/config) Basic Algorithm Parameters File.
    *   [data](examples/data) Domain Problem Test Data.
    *   [analyze.py](examples/analyze.py) Analyze the algorithm results.
    *   [example_template.h](examples/example_template.h) A helper to standardize the development process of an example.
    *   [qap_examples.cc](examples/qap_examples.cc) Quadratic Assignment Problem Example.
    *   [test_instances.py](examples/test_instances.py) Run test instances batchly.
    *   [test_qap.py](examples/test_qap.py) Run test QAP instances batchly.
    *   [xmake.lua] xmake for examples.
*   [include/light_or](include/light_or) Root directory for headers.
    *   [base](include/light_or/base) Basic utilities. Mainly related to cpp tricks.
    *   [domain_problems](include/light_or/domain_problems) Defined internal domain problems solver.
    *   [model](include/light_or/model) Core components of the model.
        *   [solution_encodings](include/light_or/model/solution_encodings) Encodings of solution.
    *   [solver](include/light_or/solver) Core components of the solver.
        *   [operators](include/light_or/solver/operators) Operators.
    *   [util](include/light_or/util) Utilities needed by the light or solver. High level than the base.
*   [lib](lib) Store core lib of light or solver.
*   [src](src) Root directory for source files. It is a submodule.


## Installation
LightOR is currently xmake based build.
## Benchmarks
[quadratic assignment problem](https://github.com/DubingXiang/light_or-docs/blob/master/benchmarks/qap/README.md)

## Quick Start
TODO
## Contacts
- email:   	    
	- karupin22@163.com
- source:
    - [github](https://github.com/DubingXiang/light_or)    
## License
This project is licensed under the terms of the MIT license. See [LICENSE](https://github.com/DubingXiang/light_or/blob/master/LICENSE) for additional details.