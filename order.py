# order.py

import os
from pathlib import Path
import re

def is_sorted_with_prefix(names):
    """
    检查目录名称是否已经按序号前缀排序。
    
    :param names: 目录名称列表
    :return: 如果已排序返回 True，否则返回 False
    """
    # 提取序号前缀和实际名称
    def extract_prefix(name):
        match = re.match(r"^(\d{2})_(.*)", name)
        return int(match.group(1)) if match else None

    prefixes = [extract_prefix(name) for name in names]
    # 如果所有目录都有有效序号前缀，并且序号连续递增，则认为已排序
    return all(prefix is not None for prefix in prefixes) and prefixes == sorted(prefixes)

def add_prefix_to_directories(directory_path, unsorted_dirs):
    """
    为未排序的目录名称添加序号前缀。
    
    :param directory_path: 需要处理的目录路径
    :param unsorted_dirs: 未排序的目录列表 [(name, mtime), ...]
    """
    # 按时间排序
    sorted_dirs = sorted(unsorted_dirs, key=lambda x: x[1])

    # 从已有最大序号开始编号
    existing_names = os.listdir(directory_path)
    max_prefix = max(
        (int(re.match(r"^(\d{2})_", name).group(1)) for name in existing_names if re.match(r"^\d{2}_", name)),
        default=0
    )
    start_index = max_prefix + 1

    for index, (name, _) in enumerate(sorted_dirs, start=start_index):
        # 格式化序号为两位数（例如 01, 02, ...）
        prefix = f"{index:02d}_"
        
        # 构造新目录名称
        new_name = f"{prefix}{name}"
        old_path = os.path.join(directory_path, name)
        new_path = os.path.join(directory_path, new_name)
        
        # 重命名目录
        os.rename(old_path, new_path)
        print(f"重命名: {name} -> {new_name}")

def sort_directories_by_time(directory_path):
    """
    按照修改时间对目录进行排序，并忽略隐藏目录。
    
    :param directory_path: 需要处理的目录路径
    """
    # 获取所有子目录和文件
    entries = os.listdir(directory_path)
    directories = []

    for entry in entries:
        full_path = os.path.join(directory_path, entry)
        # 忽略隐藏目录和非目录项
        if not entry.startswith('.') and os.path.isdir(full_path):
            directories.append((entry, os.path.getmtime(full_path)))

    # 按名称检查是否已经排序
    dir_names = [name for name, _ in directories]
    if is_sorted_with_prefix(dir_names):
        print("目录已经按序号前缀排序，无需重新排序。")
        print("跳过的目录：", ", ".join(dir_names))
        return

    # 分离已排序和未排序的目录
    def has_prefix(name):
        return re.match(r"^\d{2}_", name) is not None

    sorted_dirs = [name for name, _ in directories if has_prefix(name)]
    unsorted_dirs = [(name, mtime) for name, mtime in directories if not has_prefix(name)]

    if sorted_dirs:
        print("已有序号前缀的目录：", ", ".join(sorted_dirs))

    if unsorted_dirs:
        print("未有序号前缀的目录：", ", ".join(name for name, _ in unsorted_dirs))
        add_prefix_to_directories(directory_path, unsorted_dirs)

if __name__ == "__main__":
    # 当前工作目录
    current_directory = Path.cwd()

    print(f"正在处理目录: {current_directory}")
    sort_directories_by_time(current_directory)
