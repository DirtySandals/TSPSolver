from os import path, getcwd
from glob import glob
from typing import List

# Search SolverApplication for all .txt files (all instances)
def InstanceFinder() -> List[str]:
    # Join current directory with SolverApplication to ensure it exists
    current_directory = getcwd()

    folder_name = "./../Instances"

    if not path.isdir(path.join(current_directory, folder_name)):
        raise NotADirectoryError("SolverApplication folder not found")
    
    folder_path = path.join(current_directory, folder_name)
    instances_path = path.join(folder_path, "*.txt")
    # Search all .txt instances and return them
    instances = []
    for instance in glob(instances_path):
        instances.append(path.splitext(path.basename(instance))[0])
    
    if len(instances) > 0:
        return instances
    else:
        raise FileNotFoundError("No instance files found is /SolverApplication")

# Ensures TSPSolver.exe exists        
def exeFinder() -> str:
    current_directory = getcwd()

    folder_name = "SolverApplication"

    if not path.isdir(path.join(current_directory, folder_name)):
        raise NotADirectoryError("SolverApplication folder not found")
    
    folder_path = path.join(current_directory, folder_name)
    exe_path = path.join(folder_path, "TSPSolver.exe")
    
    if path.isfile(exe_path):
        return exe_path
    else:
        raise FileNotFoundError("TSPSolver.exe not found")
