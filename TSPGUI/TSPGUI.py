import pygame
import sys

from GUI import GUI
import GUI

if __name__ == "__main__":
    gui = GUI.GUI()
    pygame.init()

    screen = pygame.display.set_mode((GUI.WIDTH, GUI.HEIGHT))
    pygame.display.set_caption("TSPGUI")
    
    # Colors (RGB format)
    WHITE = (255, 255, 255)
    RED = (255, 0, 0)
        # Clock to control frame rate
    clock = pygame.time.Clock()

    # Rectangle properties
    rect_x, rect_y = 100, 100
    rect_width, rect_height = 50, 50
    rect_speed = 5

    # Game loop
    while True:
        # Handle events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        # Update game state
        keys = pygame.key.get_pressed()  # Detect key presses

        gui.render(pygame, screen, keys)

        # Update display
        pygame.display.flip()

        # Cap frame rate
        clock.tick(60)