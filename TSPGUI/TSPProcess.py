from TSPPathFinder import exeFinder
from subprocess import Popen, PIPE
from threading import Thread
from os import getcwd, set_blocking
from time import sleep
from typing import List, Tuple

# Converts array of tuples to eligible terminal command of entries
def points_to_instance(points: List[Tuple[int, int]]) -> str:
    instance = ""
    
    for point in points:
        instance += str(point[0]) + "," + str(point[1]) + " "
        
    instance += "EOF"
    
    return instance

"""
    TSPProcess abstracts the TSPSolver as it runs the executable as a subprocess.
    It writes to the process and listens to any errors and outputs from the application
"""
class TSPProcess:
    # Init variables
    def __init__(self):
        self.process = None
        self.listener_thread = None
        self.best_route = []
        self.exe_path = exeFinder()
        self.listening = False
        
        self.init_process()
        
    # Open the subprocess
    def init_process(self) -> None:
        # Provide directory it will be working in
        exe_directory = getcwd()
        # Open pipe to subprocess
        self.process = Popen(
            [self.exe_path], 
            stdin=PIPE,
            stdout=PIPE,
            stderr=PIPE,
            text=True,
            cwd=exe_directory
        )
        # Set no blocking to eventually stop listening to process
        set_blocking(self.process.stdout.fileno(), False)
        # Set up thread to continually listen to process
        self.listener_thread = Thread(target=self.listen_to_stdout)
        self.listener_thread.start()
        
    # Cleanup listening thread and terminate process    
    def cleanup(self) -> None:
        if self.process is not None:
            self.stop()
            self.listening = False
            self.listener_thread.join()
            self.process.terminate()
            self.process = None
            
    # Destructor cleans up            
    def __del__(self):
        self.cleanup()
        
    # Parses best route given by process
    def parse_route(self, route: str) -> None:
        route = route.strip()
        route = route.split()
        
        if route[0] != "bestroute:":
            return
        
        del route[0]
        
        new_best_route = []
        
        for city in route:
            new_best_route.append(int(city))
        
        self.best_route = new_best_route
        
    # Getter for best route
    def get_best_route(self) -> List[int]:
        return self.best_route
    
    # Listens to stdout continuallu
    def listen_to_stdout(self) -> None:
        self.listening = True
        # Listen until stoplistening
        while self.listening:
            try:
                # Read incoming line
                output = self.process.stdout.readline()
                # If proces teminated unexpectedly, stop listening
                if self.process.poll() is not None:
                    print("Process Terminated.")
                    break
                # Check if output is best route
                if output:
                    print(f"Received: {output.strip()}")
                    self.parse_route(output.strip())
                # If no response, sleep to avoid constant checking
                else:
                    sleep(0.01)      
            # Handle BlockingIOError associated with non blocking readline                    
            except BlockingIOError:
                print('err')
    
    # Writes message to process
    def write_to_process(self, message: str) -> None:
        print(f"Sending: {message}")
        self.process.stdin.write(f"{message}\r\n")
        self.process.stdin.flush()
        
    # Stops process' algorithm
    def stop(self) -> None:
        self.write_to_process("stop")
        
    # Asks process to load file instance    
    def load_file(self, file: str) -> None:
        message = f"load file {file}"
        self.write_to_process(message)
        self.best_route = []
        
    # Asks process to load custom instance    
    def load_instance(self, instance: List[Tuple[int, int]]) -> None:
        formatted_instance = points_to_instance(instance)
        message = f"load instance {formatted_instance}"
        
        self.write_to_process(message)
        self.best_route = []
        
    # Asks process to start a genetic algorithm    
    def start_ga(self, inverover: bool, max_generations: int, population_size: str, mutator: str, crossover: str, selector: str) -> None:
        message = "start "
        # Pass on arguments
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

        self.best_route = []

        self.write_to_process(message)
        