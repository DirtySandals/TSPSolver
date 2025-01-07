from enum import Enum

class AppState(Enum):
    TITLE_SCREEN = 1
    MAIN = 2
    
state = AppState.TITLE_SCREEN
WIDTH = 800
HEIGHT = 600

class Button:
    def __init__(self, center, text, text_color, text_size, transition_state, background_color):
        self.center = center
        self.text = text
        self.text_color = text_color
        self.text_size = text_size
        self.transition_state = transition_state
        self.background_color = background_color
        
    def render(self, pygame, screen, event):
        font = pygame.font.Font(None, self.text_size)
        
        button_text = font.render(self.text, True, self.text_color, None)
        button_container = button_text.get_rect(center=self.center)
        
        screen.blit(button_text, button_container)
        
        if event == None:
            return
        elif event.type == pygame.MOUSEBUTTONDOWN:
            state = self.transition_state

class TitleScreen:
    def __init__(self):
        self.background_color = (255, 232, 215)
        self.title = ["Travelling Salesman Problem", "A Genetic Algorithm Demonstration"]
 
        self.buttons = []
        
        self.initButtons()
    
    def initButtons(self):
        choices_text = ["Use Classic Instances", "Use Custom Instance"]
        choices_center = (WIDTH // 2, HEIGHT * 0.7)
        choices_text_color = (0, 0, 0)
        choices_background_color = (84, 0, 115)
        choices_text_size = 30
        
        for text in choices_text:
            button = Button(
                center=choices_center, 
                text=text,
                text_color=choices_text_color,
                text_size=choices_text_size,
                transition_state=AppState.MAIN,
                background_color=choices_background_color
            )
            
            self.buttons.append(button)

    def render_title(self, pygame, screen, keys):
        font = pygame.font.Font(None,52)
        
        title_center = (WIDTH // 2, HEIGHT * 0.3)
        start_y = title_center[1] - (len(self.title) * font.get_height())
        
        for i, line in enumerate(self.title):
            title_text = font.render(line, True, (0, 0, 0), None)
            title_container = title_text.get_rect(center=(title_center[0], start_y + i * font.get_height()))
        
            screen.blit(title_text, title_container)
    
    def render_choices(self, pygame, screen, keys):
        buttonEvent = None
        for event in pygame.event.get():
            if event.type == pygame.MOUSEBUTTONDOWN:
                buttonEvent = event
                
        for button in self.buttons:
            button.render(pygame, screen, buttonEvent)
            

    def render(self, pygame, screen, keys):
        screen.fill(self.background_color)
        
        self.render_title(pygame, screen, keys)
        self.render_choices(pygame, screen, keys)
                   
class GUI:
    def __init__(self):
        self.title_screen = TitleScreen()

    def render(self, pygame, screen, keys):        
        if state == AppState.TITLE_SCREEN:
            self.title_screen.render(pygame, screen, keys)

