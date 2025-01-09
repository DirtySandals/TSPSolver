from pickle import INST
from tkinter.font import Font
import pygame
import sys
from enum import Enum
import numpy as np
from TSPProcess import TSPProcess
from Button import Button
from TSPPathFinder import InstanceFinder
from InstanceParser import parse_instance

instances = InstanceFinder()

process = TSPProcess()

pygame.init()

WIDTH, HEIGHT = 800, 600

SCREEN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("TSP")

BACKGROUND = (81, 213, 224)

clock = pygame.time.Clock()

back_button = Button(
            image=None,
            pos=(WIDTH // 10, HEIGHT // 10),
            text_input="Back",
            font=pygame.font.Font(None, 32),
            base_color=(0, 0, 0),
            hovering_color="White"
)

point_color = (255, 0, 0)
point_radius = 5

graph_width = WIDTH // 2
graph_height = HEIGHT // 2
graph_center_x = WIDTH // 2
graph_center_y = HEIGHT // 2
graph_color = (255, 255, 255)

graph = pygame.Rect(graph_center_x - graph_width // 2, graph_center_y - graph_height // 2, graph_width, graph_height)

def display_tsp(points, inverover, mutator, crossover, selector, pop_size):
    start_button = Button(
            image=None,
            pos=(WIDTH // 2, HEIGHT * 6 // 7),
            text_input="Start Algorithm",
            font=pygame.font.Font(None, 48),
            base_color=(0, 0, 0),
            hovering_color="White"
    )
    
    while True:
        SCREEN.fill(BACKGROUND)
        
        mouse_pos = pygame.mouse.get_pos()      

        back_button.changeColor(mouse_pos)
        back_button.update(SCREEN)
        
        pygame.draw.rect(SCREEN, graph_color, graph)
        
        for point in points:
            pygame.draw.circle(SCREEN, point_color, point, point_radius)
        
        start_button.changeColor(mouse_pos)
        start_button.update(SCREEN)

            

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if back_button.checkForInput(mouse_pos):
                    return
                if start_button.checkForInput(mouse_pos):
                    process.start_ga(inverover, 20000, pop_size, mutator, crossover, selector)
        
        pygame.display.update()

def configure_algorithm(points):
    def handle_option_click(options, mouse_pos, current_value):
        for option in options:
            if option["button"].checkForInput(mouse_pos):
                return option["value"]
        
        return current_value

    class AlgorithmType(Enum):
        InverOver = 0
        Custom = 1
        
    selected_algorithm = None
    
    # Default algorithm
    selected_mutator = "inversion"
    selected_crossover = "pmx"
    selected_selector = "tournament"
    selected_population_size = "50"
    
    option_rows = [200, 350]
    option_cols = [250, 550]

    option_cell = []
    
    for row in option_rows:
        for col in option_cols:
            option_cell.append((col, row))
    
    subtitle_option_separation = 25
    
    mutators = [
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[0], (0, subtitle_option_separation))),
                        text_input="Inversion",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "inversion"        
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[0], (0, subtitle_option_separation + 24))),
                        text_input="Insert",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "insert"  
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[0], (0, subtitle_option_separation + 24 * 2))),
                        text_input="Swap",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "swap"  
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[0], (0, subtitle_option_separation + 24 * 3))),
                        text_input="Scramble",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "scramble"  
        }
    ]

    crossovers = [
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[1], (0, subtitle_option_separation))),
                        text_input="PMX",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "pmx"        
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[1], (0, subtitle_option_separation + 24))),
                        text_input="Order",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "order"  
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[1], (0, subtitle_option_separation + 24 * 2))),
                        text_input="Cycle",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "cycle"  
        }
    ]
    
    selectors = [
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[2], (0, subtitle_option_separation))),
                        text_input="Tournament",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "tournament"        
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[2], (0, subtitle_option_separation + 24))),
                        text_input="Fitness Proportional",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "fitness"  
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[2], (0, subtitle_option_separation + 24 * 2))),
                        text_input="Elitism",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "elitism"  
        }
    ]

    population_sizes = [
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[3], (0, subtitle_option_separation))),
                        text_input="25",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "25"       
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[3], (0, subtitle_option_separation + 24))),
                        text_input="50",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "50"
        },
        {
            "button": Button(
                        image=None,
                        pos=tuple(np.add(option_cell[3], (0, subtitle_option_separation + 24 * 2))),
                        text_input="100",
                        font=pygame.font.Font(None, 24),
                        base_color=(0, 0, 0),
                        hovering_color="White"
                    ),
            "value": "100"
        }
    ]

    inverover_button = Button(
            image=None,
            pos=(WIDTH // 2, HEIGHT // 2),
            text_input="InverOver",
            font=pygame.font.Font(None, 48),
            base_color=(0, 0, 0),
            hovering_color="White"
    )
    
    custom_button = Button(
            image=None,
            pos=(WIDTH // 2, HEIGHT // 2 + 100),
            text_input="Custom",
            font=pygame.font.Font(None, 48),
            base_color=(0, 0, 0),
            hovering_color="White"
    )

    next_button = Button(
            image=None,
            pos=(WIDTH // 2, HEIGHT * 6 // 7),
            text_input="Next",
            font=pygame.font.Font(None, 48),
            base_color=(0, 0, 0),
            hovering_color="White"
    )
    
    while True:
        SCREEN.fill(BACKGROUND)
        
        mouse_pos = pygame.mouse.get_pos()      

        back_button.changeColor(mouse_pos)
        back_button.update(SCREEN)
        
        if selected_algorithm is AlgorithmType.Custom:
            next_button.changeColor(mouse_pos)
            next_button.update(SCREEN)

        if selected_algorithm is None:
            select_font = pygame.font.Font(None, 32)
            select_text = select_font.render("Select Algorithm", True, (0, 0, 0), None)
            select_rect = select_text.get_rect(center=(WIDTH // 2, WIDTH // 3))
            
            SCREEN.blit(select_text, select_rect)
            
            inverover_button.changeColor(mouse_pos)
            inverover_button.update(SCREEN)

            custom_button.changeColor(mouse_pos)
            custom_button.update(SCREEN)
           
        elif selected_algorithm is AlgorithmType.Custom:
            custom_font = pygame.font.Font(None, 48)
            custom_title = custom_font.render("Custom Algorithm", True, (0, 0, 0), None)
            custom_rect = custom_title.get_rect(center=(WIDTH // 2, HEIGHT // 6))

            SCREEN.blit(custom_title, custom_rect)
            
            subtitle_font = pygame.font.Font(None, 32)
            
            mutator_text = subtitle_font.render("Mutator", True, (0, 0, 0), None)
            mutator_rect = mutator_text.get_rect(center=option_cell[0])
            
            SCREEN.blit(mutator_text, mutator_rect)
            
            for option in mutators:
                if option["value"] == selected_mutator:
                    option["button"].base_color = (255, 0, 0)
                else:
                    option["button"].base_color = (0, 0, 0)
                    
                option["button"].changeColor(mouse_pos)
                option["button"].update(SCREEN)

            crossover_text = subtitle_font.render("Crossover", True, (0, 0, 0), None)
            crossover_rect = crossover_text.get_rect(center=option_cell[1])
            
            SCREEN.blit(crossover_text, crossover_rect)

            for option in crossovers:
                if option["value"] == selected_crossover:
                    option["button"].base_color = (255, 0, 0)
                else:
                    option["button"].base_color = (0, 0, 0)
                option["button"].changeColor(mouse_pos)
                option["button"].update(SCREEN)
                
            selector_text = subtitle_font.render("Selection", True, (0, 0, 0), None)
            selector_rect = selector_text.get_rect(center=option_cell[2])
            
            SCREEN.blit(selector_text, selector_rect)
            
            for option in selectors:
                if option["value"] == selected_selector:
                    option["button"].base_color = (255, 0, 0)
                else:
                    option["button"].base_color = (0, 0, 0)

                option["button"].changeColor(mouse_pos)
                option["button"].update(SCREEN)

            population_text = subtitle_font.render("Population Size", True, (0, 0, 0), None)
            population_rect = population_text.get_rect(center=option_cell[3])
            
            SCREEN.blit(population_text, population_rect)
            
            for option in population_sizes:
                if option["value"] == selected_population_size:
                    option["button"].base_color = (255, 0, 0)
                else:
                    option["button"].base_color = (0, 0, 0)

                option["button"].changeColor(mouse_pos)
                option["button"].update(SCREEN)
            

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if back_button.checkForInput(mouse_pos):
                    if selected_algorithm is not None:
                        selected_algorithm = None
                    else:
                        return
                if selected_algorithm is None:
                    if inverover_button.checkForInput(mouse_pos):
                        selected_algorithm = AlgorithmType.InverOver
                        display_tsp(points, True, None, None, None, None)
                        return
                    elif custom_button.checkForInput(mouse_pos):
                        selected_algorithm = AlgorithmType.Custom
                elif selected_algorithm is AlgorithmType.Custom:
                    if next_button.checkForInput(mouse_pos):
                        display_tsp(points, False, selected_mutator, selected_crossover, selected_selector, selected_population_size)
                        return
                    
                    selected_mutator = handle_option_click(mutators, mouse_pos, selected_mutator)
                    selected_crossover = handle_option_click(crossovers, mouse_pos, selected_crossover)
                    selected_selector = handle_option_click(selectors, mouse_pos, selected_selector)
                    selected_population_size = handle_option_click(population_sizes, mouse_pos, selected_population_size)
        
        pygame.display.update()

def make_instance():
    points = []
    max_points = 50
    
    while True:
        SCREEN.fill(BACKGROUND)
        
        mouse_pos = pygame.mouse.get_pos()

        back_button.changeColor(mouse_pos)
        back_button.update(SCREEN)
        
        pygame.draw.rect(SCREEN, graph_color, graph)
        
        for point in points:
            pygame.draw.circle(SCREEN, point_color, point, point_radius)

        if len(points) >= max_points:
            warning_font = pygame.font.Font(None, 24)
            warning = warning_font.render("Maximum Number Of Points Created", True, point_color, None)
            warning_rect = warning.get_rect(center=(WIDTH // 2, 4 * HEIGHT // 5))
            
            SCREEN.blit(warning, warning_rect)
        
        remove_point = Button(
            image=None,
            pos=(600, 100),
            text_input="Remove Last Entry",
            font=pygame.font.Font(None, 24),
            base_color=(0, 0, 0),
            hovering_color="White"
        )

        remove_point.changeColor(mouse_pos)
        remove_point.update(SCREEN)

        solve_button = Button(
            image=None,
            pos=(WIDTH // 2, HEIGHT * 6 // 7),
            text_input="Solve Problem",
            font=pygame.font.Font(None, 48),
            base_color=(0, 0, 0),
            hovering_color="White"
        )
        
        solve_button.changeColor(mouse_pos)
        solve_button.update(SCREEN)


        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if back_button.checkForInput(mouse_pos):
                    return
                if graph.collidepoint(mouse_pos) and len(points) < max_points:
                    points.append(mouse_pos)
                if remove_point.checkForInput(mouse_pos) and len(points) > 0:
                    points.pop()
                if solve_button.checkForInput(mouse_pos) and len(points) > 0:
                    process.load_instance(points)
                    configure_algorithm(points)
        
        pygame.display.update()

def select_instance():
    while True:
        SCREEN.fill(BACKGROUND)
        
        mouse_pos = pygame.mouse.get_pos()
        
        back_button.changeColor(mouse_pos)
        back_button.update(SCREEN)

        title_font = pygame.font.Font(None, 50)
        menu_title = title_font.render("Select An Instance", True, (0, 0, 0), None)
        menu_title_rect = menu_title.get_rect(center=(WIDTH // 2, HEIGHT // 4))

        SCREEN.blit(menu_title, menu_title_rect)
        
        button_font = pygame.font.Font(None, 32)
        buttons = []
        button_start_height = HEIGHT // 3
        button_gap = button_font.get_height() * 1.5
        for i, instance in enumerate(instances):
            buttons.append(Button(
                image=None,
                pos=(WIDTH // 2, button_start_height + button_gap * i),
                text_input=instance,
                font=button_font,
                base_color=(0, 0, 0),
                hovering_color="White"
            ))

        for button in buttons:
            button.changeColor(mouse_pos)
            button.update(SCREEN)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if back_button.checkForInput(mouse_pos):
                    return
                
                for button in buttons:
                    if button.checkForInput(mouse_pos):
                        process.load_file(button.text_input)
                        path = f"./SolverApplication/{button.text_input}.txt"
                        instance_coords = parse_instance(path)
                        configure_algorithm(instance_coords)
                        break
        
        pygame.display.update()

def main_menu():
    while True:
        SCREEN.fill(BACKGROUND)
        
        mouse_pos = pygame.mouse.get_pos()
        
        title_font = pygame.font.Font(None, 50)
        menu_title_1 = title_font.render("Genetic Algorithm", True, (0, 0, 0), None)
        menu_title_rect_1 = menu_title_1.get_rect(center=(WIDTH // 2, HEIGHT // 4))
        
        SCREEN.blit(menu_title_1, menu_title_rect_1)

        menu_title_2 = title_font.render("TSP Solver", True, (0, 0, 0), None)
        menu_title_rect_2 = menu_title_2.get_rect(center=(WIDTH // 2, (HEIGHT // 4) + menu_title_1.get_height()))
        
        SCREEN.blit(menu_title_2, menu_title_rect_2)
        
        button_font = pygame.font.Font(None, 32)
        
        file_instance_button = Button(
            image=None,
            pos=(WIDTH // 2, HEIGHT // 2),
            text_input="Solve Preloaded Instance",
            font=button_font,
            base_color=(0, 0, 0),
            hovering_color="White"
        )

        custom_instance_button = Button(
            image=None,
            pos=(WIDTH // 2, HEIGHT // 2 + 150),
            text_input="Solve Custom Instance",
            font=button_font,
            base_color=(0, 0, 0),
            hovering_color="White"
        )

        for button in [file_instance_button, custom_instance_button]:
            button.changeColor(mouse_pos)
            button.update(SCREEN)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if file_instance_button.checkForInput(mouse_pos):
                    select_instance()
                if custom_instance_button.checkForInput(mouse_pos):
                    make_instance()
        
        pygame.display.update()
        

if __name__ == "__main__":
    main_menu()