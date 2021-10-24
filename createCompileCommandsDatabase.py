import os

os.system("bazel build --config=_gcc generateCompileCommands")
os.system("cp bazel-bin/compile_commands.json ./")
