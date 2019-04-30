import os
import json
import subprocess
from shutil import copyfile

from source.preprocessor import process_string
from source.config import Config

cwFlags = "-proc gekko -I- -Cpp_exceptions off -enum int -Os -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0"
cwFlags += " -DRVL -DNDEBUG -DRVL_SDK -DTRK_INTEGRATION -DRVL_OS -DEPPC -DGEKKO -DHOLLYWOOD_REV=1 -DBROADWAY_REV=1 -DIOP_REV=1 -DNW4R_RELEASE -DNW4R_CW3 -DNW4R_RVL"

class ProjectManager:
    def __init__(self, path):
        # Set our project path
        self.project_path = path

        # Load our configuration
        with open(os.path.join(path, "project.json"), "r") as file:
            self.project_cfg = json.load(file)

        self.global_config = Config()
        self.global_config.fromFile(os.path.join(self.project_path, "global_config.json"))

        # Process our project includes ahead of time
        self.project_includes = " ".join(("-I%s" % process_string(self.global_config, i)) for i in self.project_cfg["includes"])

        self.project_includes += " -I" + self.project_path

    def build_library(self, release=False):
        """
        Build our library components.

        :param release: Whether or not to build for release.
        :return: List of library objects
        """

        verbose = False

        # Save our working directory
        cwd = os.getcwd()

        # Set our current directory to our project path
        os.chdir(self.project_path)

        result = []

        # If our library bin folder does not exit, create it
        if not os.path.isdir("./lib/bin"):
            os.mkdir("./lib/bin")

        # Arguments for our compiler
        cwArg = cwFlags + " " + self.project_includes + " -I%s\\lib\\" % self.project_path

        # pass down our build mode
        cwArg += " -DRELEASE" if release else " -DDEBUG"

        with open(self.project_path + "\\lib\\build.json", "r") as makefile:
            lib_make = json.load(makefile)
            os.chdir(self.project_path + "\\..\\tool\\")
            for file in lib_make["sources"]:
                # TODO: bin subfolder not auto created!
                compiler_string = "./mwcceppc.exe %s -c -o %s/lib/bin/%s.o %s/lib/%s" % (
                    cwArg, self.project_path, file, self.project_path, file)
                print(compiler_string)
                if subprocess.call(compiler_string):
                    raise RuntimeError("[FATAL] Failed to compile %s!", file)
                else:
                    result.append("%s/lib/bin/%s.o" % (self.project_path, file))

            # Restore our working directory
            os.chdir(cwd)

            self.library_objects = result + lib_make["objects"]

            return self.library_objects

    def compile_module(self, module, release=False):
        result = []
        module_cfg = None

        # Save our working directory
        cwd = os.getcwd()

        # Set our current directory to our project path
        os.chdir(self.project_path)

        # Load our module config
        module_cfg = json.load(open("./modules/%s/module.json" % module, "r"))

        print("Compiling module %s..." % module)

        # Create our module bin folder
        if not os.path.isdir("./modules/%s/bin" % module):
            os.mkdir("./modules/%s/bin" % module)

        # Construct our compiler arguments
        cwArg = cwFlags + " " + self.project_includes + " " + " ".join(
            "-I%s" % (process_string(self.global_config, i)) for i in module_cfg["includes"]) + " -I%s\\modules\\%s\\" % (
                    self.project_path, module)

        # pass down our build mode
        cwArg += " -DRELEASE" if release else " -DDEBUG"

        os.chdir(self.project_path + "\\..\\tool\\")
        for file in module_cfg["sources"]:
            print("...compiling %s" % file)
            compiler_string = "./mwcceppc.exe %s -c -o %s/modules/%s/bin/%s.o %s/modules/%s/%s" % (
                cwArg, self.project_path, module, file, self.project_path, module, file)
            if verbose:
                print(compiler_string)
            if subprocess.call(compiler_string):
                raise RuntimeError("[FATAL] Failed to compile %s!", file)
            else:
                result.append("%s/modules/%s/bin/%s.o" % (self.project_path, module, file))

        # Restore our working directory
        os.chdir(cwd)

        return result

    def build(self, release=False):
        """
        Build the project!

        :param release: Whether or not to build in release mode.
        """

        mode = "release" if release else "debug"
        print("Building configuration: %s" % mode)

        # Compile all modules
        objects = []

        for module in self.project_cfg["modules"]:
            objects += self.compile_module(module, release)

        objects += self.build_library(release)

        os.chdir(self.project_path + "/../tool/")

        kamek_command = "Kamek.exe %s -static=%s -output-gecko=%s -output-code=%s -externals=%s/externals/%s" % (
                        " ".join(objects), self.project_cfg["static"], self.project_path + "\\bin\\gecko.txt",
                        self.project_path + "\\bin\\%s\\CODE.bin" % mode, self.project_path, self.project_cfg["externals"])

        print(kamek_command)

        if subprocess.call(kamek_command):
            raise RuntimeError("Kamek fail")
        else:
            copyfile(self.project_path + "\\bin\\%s\\CODE.bin" % mode,
                     CODE_DEPLOY_BIN)
            with open(self.project_path + "\\bin\\gecko.txt", "r") as gecko:
                producePatch(gecko.readlines(), self.project_path + "\\bin\\PATCH.bin")
            copyfile(self.project_path + "\\bin\\PATCH.bin",
                     PATCH_DEPLOY_PATH)