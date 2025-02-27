#!/usr/local/bin/python3
import os
import subprocess

# 获取当前目录
current_dir = os.getcwd()

# 遍历当前目录下的所有子目录（不递归）
for item in os.listdir(current_dir):
    # 检查是否是目录
    item_path = os.path.join(current_dir, item)
    if os.path.isdir(item_path):
        # 检查目录中是否存在 Makefile
        makefile_path = os.path.join(item_path, "Makefile")
        if os.path.isfile(makefile_path):
            # 进入该目录
            os.chdir(item_path)
            # 运行 make clean 命令
            try:
                subprocess.run(["make", "clean"], check=True)
            except subprocess.CalledProcessError as e:
                print(f"Failed to run 'make clean' in: {item_path}. Error: {e}")
            # 切换回当前目录
            os.chdir(current_dir)
