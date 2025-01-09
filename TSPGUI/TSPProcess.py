from audioop import cross
from pickle import INST
from TSPPathFinder import exeFinder
import select
import subprocess
import threading
import os

def points_to_instance(points):
    instance = ""
    
    for point in points:
        instance += str(point[0]) + "," + str(point[1]) + " "
        
    instance += "EOF"
    
    return instance

class TSPProcess:
    def __init__(self):
        self.process = None
        self.listener_thread = None
        self.best_route = []
        self.exe_path = exeFinder()
        
        self.init_process()
    
    def init_process(self):
        exe_directory = os.path.join(os.getcwd(), "SolverApplication")
        self.process = subprocess.Popen(
            [self.exe_path], 
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            cwd=exe_directory
        )
        
        self.listener_thread = threading.Thread(target=self.listen_to_stdout)
        self.listener_thread.start()
        
    def __del__(self):
        if self.process is not None:
            self.listener_thread.join()
            self.process.terminate()
    
    def parse_route(self, route):
        route = route.strip()
        route = route.split()
        
        if route[0] != "bestroute:":
            return
        
        del route[0]
        
        self.best_route = []
        
        for city in route:
            self.best_route.append(city)
    
    def get_best_route(self):
        return self.best_route
    
    def listen_to_stdout(self):
        while True:
            output = self.process.stdout.readline()

            if output == "" and self.process is not None:
                print("Process Terminated.")
                break
            
            if output:
                print(f"Received: {output.strip()}")
                self.parse_route(output.strip())
                
    def write_to_process(self, message):
        print(message)
        self.process.stdin.write(f"{message}\r\n")
        self.process.stdin.flush()
    
    def stop(self):
        self.write_to_process("stop")
        
    def load_file(self, file):
        message = f"load file {file}"
        self.write_to_process(message)
        
    def load_instance(self, instance):
        formatted_instance = points_to_instance(instance)
        print(formatted_instance)
        message = f"load instance {formatted_instance}"
        self.write_to_process(message)
        
    def start_ga(self, inverover=False, max_generations=None, population_size=None, mutator=None, crossover=None, selector=None):
        message = "start "
        
        if inverover:
            message += "inverover"
            self.write_to_process(message)
            return
        
        if max_generations:
            message += f"maxgenerations {max_generations} "
            
        if population_size:
            message += f"populationsize {population_size} "
            
        if mutator:
            message += f"mutation {mutator} "
            
        if crossover:
            message += f"crossover {crossover} "
            
        if selector:
            message += f"selection {selector} "   

        self.write_to_process(message)
        