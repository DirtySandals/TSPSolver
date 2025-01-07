import pygame
import sys
from TSPProcess import TSPProcess

from GUI import GUI
import GUI

process = TSPProcess()

pygame.init()

WIDTH, HEIGHT = 800, 600

SCREEN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("TSP")

BACKGROUND = (0, 0, 0)

clock = pygame.time.Clock()

def display_tsp():
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                pass
            
        clock.tick()
        
        pygame.display.update()

def make_instance():
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                pass
            
        clock.tick()
        
        pygame.display.update()

def select_instance():
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                pass
            
        clock.tick()
        
        pygame.display.update()

def main_menu():
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                pass
            
        clock.tick()
        
        pygame.display.update()
        

if __name__ == "__main__":
    main_menu()