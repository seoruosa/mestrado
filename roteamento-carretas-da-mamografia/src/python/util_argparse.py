from argparse import ArgumentTypeError
from os.path import isdir

# https://stackoverflow.com/questions/38834378/path-to-a-directory-as-argparse-argument        
def dir_path(path):
    if isdir(path):
        return path
    else:
        raise ArgumentTypeError(f"readable_dir:{path} is not a valid path")