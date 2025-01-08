import os
import glob

def InstanceFinder():
    current_directory = os.getcwd()

    folder_name = "SolverApplication"

    if not os.path.isdir(os.path.join(current_directory, folder_name)):
        raise NotADirectoryError("SolverApplication folder not found")
    
    folder_path = os.path.join(current_directory, folder_name)
    instances_path = os.path.join(folder_path, "*.txt")

    instances = []
    for instance in glob.glob(instances_path):
        instances.append(os.path.splitext(os.path.basename(instance))[0])
    
    if len(instances) > 0:
        return instances
    else:
        raise FileNotFoundError("No instance files found is /SolverApplication")
        
def exeFinder():
    current_directory = os.getcwd()

    folder_name = "SolverApplication"

    if not os.path.isdir(os.path.join(current_directory, folder_name)):
        raise NotADirectoryError("SolverApplication folder not found")
    
    folder_path = os.path.join(current_directory, folder_name)
    exe_path = os.path.join(folder_path, "TSPSolver.exe")
    
    print(exe_path)
    
    if os.path.isfile(exe_path):
        return exe_path
    else:
        raise FileNotFoundError("TSPSolver.exe not found")
