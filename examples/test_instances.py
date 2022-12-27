import os


def ConcatArgsCallback(instance_file_path=None, algorithm_parameters_path=None, output_dir=None):
    return " -i {} -p {} -o {}".format(instance_file_path, algorithm_parameters_path, output_dir)


def run_instances(binary_file,
                  instances_dir,
                  algorithm_parameters_dir,
                  output_dir,
                  concat_args_callback=ConcatArgsCallback):
    # 获取文件夹下的文件列表
    instance_file_list = os.listdir(instances_dir)
    param_file_list = os.listdir(algorithm_parameters_dir)
    # 遍历文件列表
    # count = 2
    for instance_file in instance_file_list:
        # if count < 0:
        #     break
        # count -= 1
        # 调用可执行文件
        instance_file_path = os.path.join(instances_dir, instance_file)
        for param_file in param_file_list:
            algorithm_parameters_file_path = os.path.join(algorithm_parameters_dir, param_file)
            print("\t ########## Sovling Instance:{}, algorithm_parameters:{}".format(
                instance_file, param_file))
            command = concat_args_callback(instance_file_path=instance_file_path,
                                           algorithm_parameters_path=algorithm_parameters_file_path,
                                           output_dir=output_dir)
            # print(command)
            os.system(binary_file + command)


def run_instance_list(binary_file,
                      instances_dir,
                      instance_name_list,
                      algorithm_parameters_dir,
                      output_dir,
                      concat_args_callback=ConcatArgsCallback):
    param_file_list = os.listdir(algorithm_parameters_dir)
    for instance_name in instance_name_list:
        instance_file_path = os.path.join(instances_dir, instance_name)

        for param_file in param_file_list:
            algorithm_parameters_file_path = os.path.join(algorithm_parameters_dir, param_file)
            print("\t ########## Sovling Instance:{}, algorithm_parameters:{}".format(
                instance_name, param_file))
            command = concat_args_callback(instance_file_path=instance_file_path,
                                           algorithm_parameters_path=algorithm_parameters_file_path,
                                           output_dir=output_dir)
            # print(command)
            os.system(binary_file + command)
