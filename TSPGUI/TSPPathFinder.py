import os
import glob

# Search SolverApplication for all .txt files (all instances)
def InstanceFinder():
    # Join current directory with SolverApplication to ensure it exists
    current_directory = os.getcwd()

    folder_name = "SolverApplication"

    if not os.path.isdir(os.path.join(current_directory, folder_name)):
        raise NotADirectoryError("SolverApplication folder not found")
    
    folder_path = os.path.join(current_directory, folder_name)
    instances_path = os.path.join(folder_path, "*.txt")
    # Search all .txt instances and return them
    instances = []
    for instance in glob.glob(instances_path):
        instances.append(os.path.splitext(os.path.basename(instance))[0])
    
    if len(instances) > 0:
        return instances
    else:
        raise FileNotFoundError("No instance files found is /SolverApplication")

# Ensures TSPSolver.exe exists        
def exeFinder():
    current_directory = os.getcwd()

    folder_name = "SolverApplication"

    if not os.path.isdir(os.path.join(current_directory, folder_name)):
        raise NotADirectoryError("SolverApplication folder not found")
    
    folder_path = os.path.join(current_directory, folder_name)
    exe_path = os.path.join(folder_path, "TSPSolver.exe")
    
    if os.path.isfile(exe_path):
        return exe_path
    else:
        raise FileNotFoundError("TSPSolver.exe not found")
