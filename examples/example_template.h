#include <filesystem>
#include <CLI/CLI.hpp>
#include <light_or/util/io.h>
#include <light_or/util/logging.h>

#define OPEN_BENCHMARK

#ifdef OPEN_BENCHMARK
#define DO_BENCHMARK(instance_file_path, objecive_value, output_dir) \
  BenchmarkRecord(instance_file_path, objecive_value, output_dir);
#else
#define DO_BENCHMARK(instance_file_path, objecive_value, output_dir)
#endif

class ExampleHelper {
 public:
  template <typename DomainSolver, typename DomainIOParser>
  int Main(int argc, char** argv, const std::string& description, const std::string& footer);

  void BenchmarkRecord(const std::string& instance_file_path,
                       double objecive_value,
                       const std::string& output_dir);
  std::string ExportParamsInAlgoParameters(const light_or::AlgorithmParameters& parameters);
  std::string BuildObjectiveStatsFileName(const std::string& instance_file_path,
                                          const light_or::AlgorithmParameters& parameters,
                                          const std::string& objective_stats_filename);

 public:
  std::string objective_found_file                   = "objective_found_list.csv";
  std::string global_best_objective_stats_file_name  = "GlobalBestObjectiveStats";
  std::string current_best_objective_stats_file_name = "CurrentBestObjectiveStats";
};

class CmdArgsParser {
 public:
  CmdArgsParser(const std::string& description, const std::string& footer);
  std::string ArgsStr() const;

  CLI::App& mutable_app() {
    return _app;
  }
  const std::string& algorithm_param_file() const {
    return _algorithm_param_file;
  }
  const std::string& instance_file() const {
    return _instance_file;
  }
  const std::string& output_dir() const {
    return _output_dir;
  }

 private:
  void Init();
  CLI::App _app;
  std::string _description;
  std::string _footer;
  std::string _algorithm_param_file;
  std::string _instance_file;
  std::string _output_dir;
};
CmdArgsParser::CmdArgsParser(const std::string& description, const std::string& footer)
    : _description(description), _footer(footer) {
  Init();
}
void CmdArgsParser::Init() {
  _app.description(_description);
  _app.footer(_footer);
  _app.add_option("-p,--parameters", _algorithm_param_file, "algorithm parameters config file")
      ->required();
  _app.add_option("-i,--instance", _instance_file, "instance file")->required();
  _app.add_option("-o,--output_dir", _output_dir, "output directory")->required(false);
}
std::string CmdArgsParser::ArgsStr() const {
  return spdlog::fmt_lib::format(
      "\t algorithm parameters config file:{} instance_file:{} output_dir:{}",
      _algorithm_param_file,
      _instance_file,
      _output_dir);
}

template <typename DomainSolver, typename DomainIOParser>
int ExampleHelper::Main(int argc,
                        char** argv,
                        const std::string& description,
                        const std::string& footer) {
  CmdArgsParser cmd_parser(description, footer);
  CLI11_PARSE(cmd_parser.mutable_app(), argc, argv);
  SPDLOG_INFO("---------- input args");
  SPDLOG_INFO(cmd_parser.ArgsStr());

  nlohmann::json algo_param_json = nlohmann::json::parse(
      light_or::util::FileIO::ReadFromFile(cmd_parser.algorithm_param_file()));
  light_or::AlgorithmParameters parameters = algo_param_json.get<light_or::AlgorithmParameters>();

  spdlog::set_level(light_or::util::LogLevel(parameters.log_level));
  DomainIOParser io_parser;
  io_parser.Parse(cmd_parser.instance_file());
  DomainSolver solver(parameters.is_use_deterministic_optimization);
  solver.Solve(io_parser.problem(), parameters);

  auto best_soln = solver.BestSolution();
  SPDLOG_CRITICAL(
      "best solution: obj:{} generated at iter:{} search_time:{} generated by op:{} content:{}",
      best_soln->LinearSumObjectiveValue(),
      best_soln->info().iteration,
      best_soln->info().search_time,
      best_soln->info().op_name,
      io_parser.Parse(*best_soln));
  DO_BENCHMARK(
      cmd_parser.instance_file(), best_soln->LinearSumObjectiveValue(), cmd_parser.output_dir());

  std::string global_best_objective_stats_file(cmd_parser.output_dir());
  global_best_objective_stats_file += BuildObjectiveStatsFileName(
      cmd_parser.instance_file(), parameters, global_best_objective_stats_file_name);
  std::string current_best_objective_stats_file(cmd_parser.output_dir());
  current_best_objective_stats_file += BuildObjectiveStatsFileName(
      cmd_parser.instance_file(), parameters, current_best_objective_stats_file_name);
  SPDLOG_INFO(
      "########## Output. global_best_objective_stats_file:{} "
      "current_best_objective_stats_file:{}",
      global_best_objective_stats_file,
      current_best_objective_stats_file);
  solver.solver()->ExportObjectiveStats(light_or::Solver::ObjectiveStatsType::global_best,
                                        global_best_objective_stats_file);
  solver.solver()->ExportObjectiveStats(light_or::Solver::ObjectiveStatsType::current_best,
                                        current_best_objective_stats_file);
  return 0;
}

void ExampleHelper::BenchmarkRecord(const std::string& instance_file_path,
                                    double objecive_value,
                                    const std::string& output_dir) {
  std::filesystem::path instance_path(instance_file_path);
  std::string instance_name = instance_path.stem().string();
  std::stringstream ss;
  ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
  ss << instance_name << "," << objecive_value << "\n";
  light_or::util::FileIO::WriteToFile(output_dir + objective_found_file, ss.str(), true);
}

std::string ExampleHelper::ExportParamsInAlgoParameters(
    const light_or::AlgorithmParameters& parameters) {
  std::stringstream ss;
  ss << "iteration_limit_" << parameters.iterations_limit;
  return ss.str();
}
std::string ExampleHelper::BuildObjectiveStatsFileName(
    const std::string& instance_file_path,
    const light_or::AlgorithmParameters& parameters,
    const std::string& objective_stats_filename) {
  std::filesystem::path instance_path(instance_file_path);
  std::string instance_name = instance_path.stem().string();
  std::stringstream ss;
  ss << objective_stats_filename << "_" << instance_name << "_"
     << ExportParamsInAlgoParameters(parameters) << ".txt";
  return ss.str();
}