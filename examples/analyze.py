import pandas as pd
import os
import matplotlib.pyplot as plt


def get_file_list(file_dir, suffix='.txt'):
    # 获取当前文件目录下的所有文件和文件夹
    files_and_directories = os.listdir(file_dir)
    # 筛选出所有txt文件
    return [f for f in files_and_directories if f.endswith('.txt')]


def get_file_name(file_path, no_suffix=True):
    if no_suffix:
        return os.path.splitext(os.path.basename(file_path))[0]
    else:
        return os.path.basename(file_path)


def read_to_data_frame_list(file_list):
    file_data_frame = {}
    for file in file_list:
        # 读取csv文件
        file_name = get_file_name(file)
        file_data_frame[file_name] = pd.read_csv(file, skiprows=1)

    return file_data_frame


def plot_all(file_data_frame, x, y, pic_type="plot"):
    # Set the figure size
    plt.figure(figsize=(12, 6))
    for file, df in file_data_frame.items():
        if pic_type == "plot":
            plt.plot(df[x], df[y], label=file)
        elif pic_type == "scatter":
            plt.scatter(df[x], df[y], label=file)
    plt.xlabel(x)
    plt.ylabel(y)
    plt.legend()
    plt.show()


class Analyzer:

    def __init__(self, problem_name):
        self.current_directory = os.getcwd()
        self.instance_output_dir = self.current_directory + "/examples/data/" + problem_name + "/output/"
        self.file_name_list = get_file_list(self.instance_output_dir)
        self.file_path_list = [
            self.instance_output_dir + file_name for file_name in self.file_name_list
        ]
        self.first_best_iters = dict()
        self.file_df_list = dict()

    def Load(self):
        self.file_df_list = read_to_data_frame_list(self.file_path_list)

    def PlotResults(self, file_name_list, x="iter", y="value"):
        filtered_results = {k: v for k, v in self.file_df_list.items() if k in file_name_list}
        plot_all(filtered_results, x, y)

    def PlotResults(self, x="iter", y="value"):
        plot_all(self.file_df_list, x, y)
        plot_all(self.first_best_iters, "iter", "value", "scatter")

    def StaticResults(self):
        for file_name, df in self.file_df_list.items():
            min_index = df['value'].idxmin()
            iter = df["iter"][min_index]
            value = df["value"][min_index]
            num_rows = df['value'].shape[0]
            self.first_best_iters[file_name] = pd.DataFrame(
                columns=["iter", "value", "all_iterations"], data=[[iter, value, num_rows]])
            self.first_best_iters[file_name]["best_found_at_percent"] = self.first_best_iters[
                file_name]["iter"] / self.first_best_iters[file_name]["all_iterations"]

    def StaticResultsToStr(self):
        names = []
        iters = []
        values = []
        all_iterations = []

        data_dict = {}
        for file_name, df in self.first_best_iters.items():
            names.append(file_name)
            iters.extend(df["iter"])
            values.extend(df["value"])
            all_iterations.extend(df["all_iterations"])
        data_dict = {
            "name": names,
            "iter": iters,
            "value": values,
            "all_iterations": all_iterations
        }
        res_df = pd.DataFrame(data_dict)
        res_df["best_found_at_percent"] = res_df["iter"] / res_df["all_iterations"]
        # 按列名为 "column" 的列排序
        res_df = res_df.sort_values("value", ascending=True)
        return res_df.to_string()


analyzer = Analyzer("qap")
analyzer.Load()
analyzer.StaticResults()
print(analyzer.StaticResultsToStr())
analyzer.PlotResults()
