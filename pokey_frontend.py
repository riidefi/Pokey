import argparse

from source.project_manager import ProjectManager

parser = argparse.ArgumentParser(description='Pokey frontend')

parser.add_argument('project_path', type=str, help='Directory of project to build')
parser.add_argument('build_mode',   type=str, help='Build mode. debug or release')
parser.add_argument('--verbose', action='store_true')

args = parser.parse_args()

ProjectManager(args.project_path, args.verbose).build(
    args.build_mode == "release")
