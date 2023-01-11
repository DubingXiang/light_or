import argparse
from test_instances import *


def test_qap_instances():
    current_directory = os.getcwd(
    )  # os.getcwd() will return working directory: the folder in which the Python scipt is operating, so you should run this script in /your_directory/light_or
    BinaryFile = current_directory + "/bin/release/qap_example"
    InstanceDir = current_directory + "/examples/data/qap/input/"
    AlgorithmParametersDir = current_directory + "/examples/config/"
    OutputDir = current_directory + "/examples/data/qap/output/"
    run_instances(BinaryFile, InstanceDir, AlgorithmParametersDir, OutputDir)
    # run_instance_list(BinaryFile, InstanceDir, instance_list, AlgorithmParametersDir, OutputDir)


if __name__ == "__main__":
    test_qap_instances()

# if __name__ == "__main__":
#     parser = argparse.ArgumentParser()
#     parser.add_argument("-b", "--binary_file", help="binary file to solve instance")
#     parser.add_argument("-id", "--instance_dir", help="instance dataset directory")
#     parser.add_argument("-ad", "--algorithm_parameters_dir", help="algorithm parameters dir")
#     parser.add_argument("-od", "--output_dir", help="output directory")
# args = parser.parse_args()
# test_instances(args.binary_file, args.instance_dir, args.algorithm_parameters_dir,
#                args.output_dir)
