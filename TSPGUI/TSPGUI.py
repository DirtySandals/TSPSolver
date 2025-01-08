from pickle import INST
from tkinter.font import Font
import pygame
import sys
from TSPProcess import TSPProcess
from Button import Button
from TSPPathFinder import InstanceFinder

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

def display_tsp(points):
    while True:
        SCREEN.fill(BACKGROUND)
        
        mouse_pos = pygame.mouse.get_pos()      

        back_button.changeColor(mouse_pos)
        back_button.update(SCREEN)
        
        pygame.draw.rect(SCREEN, graph_color, graph)
        
        for point in points:
            pygame.draw.circle(SCREEN, point_color, point, point_radius)

        start_button = Button(
            image=None,
            pos=(WIDTH // 2, HEIGHT * 6 // 7),
            text_input="Solve Problem",
            font=pygame.font.Font(None, 48),
            base_color=(0, 0, 0),
            hovering_color="White"
        )

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if back_button.checkForInput(mouse_pos):
                    return
        
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
                    display_tsp(points)
        
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
                        display_tsp()
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