import argparse
from test_instances import *


def test_qap_instances():
    current_directory = os.getcwd(
    )  # os.getcwd() will return working directory: the folder in which the Python scipt is operating, so you should run this script in /your_directory/light_or
    BinaryFile = current_directory + "/bin/release/qap_example"
    InstanceDir = current_directory + "/examples/data/qap/input/"
    AlgorithmParametersDir = current_directory + "/examples/config/"
    OutputDir = current_directory + "/examples/data/qap/output/"
    # run_instances(BinaryFile, InstanceDir, AlgorithmParametersDir, OutputDir)
    instance_list = [
        "bur26d.dat", "bur26e.dat", "bur26f.dat", "bur26g.dat", "chr15b.dat", "chr20a.dat",
        "chr20b.dat", "chr20c.dat", "chr22a.dat", "chr22b.dat", "chr25a.dat", "kra30a.dat",
        "kra30b.dat", "kra32.dat", "lipa30a.dat", "lipa40a.dat", "lipa50a.dat", "lipa60a.dat",
        "lipa70a.dat", "lipa80a.dat", "lipa90a.dat", "nug14.dat", "nug16a.dat", "nug17.dat",
        "nug18.dat", "nug20.dat", "nug21.dat", "nug22.dat", "nug24.dat", "nug25.dat", "nug27.dat",
        "nug28.dat", "nug30.dat", "rou20.dat", "sko100a.dat", "sko100b.dat", "sko100c.dat",
        "sko100d.dat", "sko100e.dat", "sko100f.dat", "sko42.dat", "sko49.dat", "sko56.dat",
        "sko64.dat", "sko72.dat", "sko81.dat", "sko90.dat", "ste36a.dat", "ste36b.dat",
        "ste36c.dat", "tai100a.dat", "tai100b.dat", "tai150b.dat", "tai15a.dat", "tai17a.dat",
        "tai20a.dat", "tai20b.dat", "tai256c.dat", "tai25a.dat", "tai25b.dat", "tai30a.dat",
        "tai30b.dat", "tai35a.dat", "tai35b.dat", "tai40a.dat", "tai40b.dat", "tai50a.dat",
        "tai50b.dat", "tai60a.dat", "tai60b.dat", "tai80a.dat", "tai80b.dat", "tho150.dat",
        "tho30.dat", "tho40.dat", "wil100.dat", "wil50.dat"
    ]
    run_instance_list(BinaryFile, InstanceDir, instance_list, AlgorithmParametersDir, OutputDir)


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
