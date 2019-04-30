import argparse

from source.project_manager import ProjectManager

parser = argparse.ArgumentParser(description='Pokey frontend')

parser.add_argument('project_path', type=str, help='Directory of project to build')
parser.add_argument('build_mode',   type=str, help='Build mode. debug or release')

args = parser.parse_args()

ProjectManager(args.project_path).build(
    args.build_mode == "release")
