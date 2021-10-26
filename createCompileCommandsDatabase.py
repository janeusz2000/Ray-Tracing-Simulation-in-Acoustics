import os

os.system("sudo rm -rf ./compile_commands.json")
os.system("bazel build --config=_gcc generateCompileCommands")
os.system("sudo cp bazel-bin/compile_commands.json ./")
