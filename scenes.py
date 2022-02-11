import pygame
import colors
import numpy as np
import time
from linalg import Vector3D
from boids import *
import utilities

class SceneBase:
    def __init__(self):
        self.next = self
        self.initialized = False

    # only needs to be called once throughout main loop
    def initGraphics(self, screen):
        self.screen = screen
        self.initialized = True

    def ProcessInput(self, events, pressed_keys):
        print("uh-oh, you didn't override this in the child class")

    def Update(self):
        print("uh-oh, you didn't override this in the child class")

    def Render(self):
        print("uh-oh, you didn't override this in the child class")

    def SwitchToScene(self, next_scene):
        self.next = next_scene

    def Terminate(self):
        self.SwitchToScene(None)


class Start(SceneBase):
    BUTTON_DELAY = 0.15

    def __init__(self):
        SceneBase.__init__(self)

        self.options = ['Boids', 'Quit']
        self.buttons = pygame.sprite.Group()
        self.startTime = time.time()

    def initGraphics(self, screen):
        SceneBase.initGraphics(self, screen)

        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h

        font = pygame.font.Font('freesansbold.ttf', 20)

        for i, option in enumerate(self.options):
            rect = pygame.Rect(int(screenWidth / 2) - 50,
                               int(screenHeight / 2) - 100 + i * 50, 100, 30)
            passive_color = colors.BLACK
            active_color = colors.RED

            if i == 0:
                def action():
                    self.SwitchToScene(BoidsScene())
            elif i == 1:
                def action():
                    self.Terminate()

            button = Button(rect, action, font, active_color, option,
                            colors.WHITE, passive_color, option, colors.WHITE)

            self.buttons.add(button)

    def ProcessInput(self, events, pressed_keys):
        pass

    def Update(self):
        if time.time() - self.startTime > self.BUTTON_DELAY:
            self.buttons.update()

    def Render(self):
        self.screen.fill(colors.WHITE)
        self.buttons.draw(self.screen)
        pygame.display.flip()


class Button(pygame.sprite.Sprite):
    def __init__(self, rect, action, font, active_color, active_text, active_textcolor, passive_color, passive_text, passive_textcolor):
        # Call the parent class (Sprite) constructor
        pygame.sprite.Sprite.__init__(self)

        self.image = pygame.Surface((rect[2], rect[3]))

        self.rect = rect

        self.font = font

        self.action = action

        self.active_color = active_color
        self.active_text = active_text
        self.active_textcolor = active_textcolor
        self.passive_color = passive_color
        self.passive_text = passive_text
        self.passive_textcolor = passive_textcolor

    def update(self):
        mouseX, mouseY = pygame.mouse.get_pos()
        pressed = pygame.mouse.get_pressed()[0]

        if self.rect.x <= mouseX <= self.rect.x + self.rect.w \
                and self.rect.y <= mouseY <= self.rect.y + self.rect.h:
            self.image.fill(self.active_color)
            self.renderButtonText(self.active_text, self.active_textcolor)

            if pressed:
                self.action()
        else:
            self.image.fill(self.passive_color)
            self.renderButtonText(self.passive_text, self.passive_textcolor)

    def renderButtonText(self, text, color):
        textsurf = self.font.render(text, True, color)
        textrect = textsurf.get_rect()
        # Put text in the middle of button
        textrect.left = self.rect.width/2 - textrect.width/2
        textrect.top = self.rect.height/2 - textrect.height/2
        self.image.blit(textsurf, textrect)


class CheckExit(SceneBase):
    def __init__(self, paused):
        SceneBase.__init__(self)
        self.next = self
        self.paused = paused
        self.options = ["Yes", "No"]
        self.buttons = pygame.sprite.Group()

    # only needs to be called once throughout main loop
    def initGraphics(self, screen):
        SceneBase.initGraphics(self, screen)
        self.screen = self.paused.screen
        self.background = pygame.Surface([self.screen.get_width(),
                                         self.screen.get_height()],
                                         flags=pygame.SRCALPHA)
        self.background.fill([255, 255, 255, 180])  # translucent white bg
        self.screen.blit(self.background, self.background.get_rect())
        self.warningText = pygame.font.SysFont('Arial', 25)
        font = pygame.font.Font('freesansbold.ttf', 20)

        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h

        for i, option in enumerate(self.options):
            rect = pygame.Rect(int(screenWidth / 2) - 50,
                               int(screenHeight / 2 - 75) + i * 50,
                               100, 30)
            passive_color = colors.BLACK
            active_color = colors.RED

            if i == 0:
                def action():
                    self.SwitchToScene(Start())
            else:
                def action():
                    self.SwitchToScene(self.paused)
                    self.paused.next = self.paused

            button = Button(rect, action, font, active_color, option, colors.WHITE, passive_color, option, colors.WHITE)

            self.buttons.add(button)

    def ProcessInput(self, events, pressed_keys):
        pass

    def Update(self):
        self.buttons.update()

    def Render(self):
        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h
        promptSurf = self.warningText.render("Quit without saving?",
                                             True, (0, 0, 0))
        promptRect = promptSurf.get_rect()
        promptRect.center = screenWidth / 2, 50
        self.screen.blit(promptSurf, promptRect)

        self.buttons.draw(self.screen)
        pygame.display.flip()

    def SwitchToScene(self, next_scene):
        super().SwitchToScene(next_scene)


class Pause(SceneBase):
    def __init__(self, paused):
        SceneBase.__init__(self)
        self.next = self
        self.paused = paused
        self.options = ["Resume", "Quit"]
        self.buttons = pygame.sprite.Group()

    # only needs to be called once throughout main loop
    def initGraphics(self, screen):
        SceneBase.initGraphics(self, screen)
        self.screen = self.paused.screen
        self.background = pygame.Surface([self.screen.get_width(),
                                         self.screen.get_height()],
                                         flags=pygame.SRCALPHA)
        self.background.fill([255, 255, 255, 180])  # translucent white bg
        self.screen.blit(self.background, self.background.get_rect())
        self.pauseText = pygame.font.SysFont('Arial', 25)
        font = pygame.font.Font('freesansbold.ttf', 20)

        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h

        for i, option in enumerate(self.options):
            rect = pygame.Rect(int(screenWidth / 2) - 50,
                               int(screenHeight / 2) + i * 50, 100, 30)
            passive_color = colors.BLACK
            active_color = colors.RED

            if i == 0:
                def action():
                    self.SwitchToScene(self.paused)
                    self.paused.next = self.paused
            else:
                def action():
                    self.SwitchToScene(Start())

            button = Button(rect, action, font, active_color, option, colors.WHITE, passive_color, option, colors.WHITE)

            self.buttons.add(button)

    def ProcessInput(self, events, pressed_keys):
        for event in events:
            if event.type == pygame.KEYDOWN:
                alt_pressed = pressed_keys[pygame.K_LALT] or \
                              pressed_keys[pygame.K_RALT]
                if event.key == pygame.K_p:
                    self.SwitchToScene(self.paused)
                    self.paused.next = self.paused


    def Update(self):
        self.buttons.update()

    def Render(self):
        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h
        promptSurf = self.pauseText.render("PAUSED", True, (0, 0, 0))
        promptRect = promptSurf.get_rect()
        promptRect.center = screenWidth/2, 50
        self.screen.blit(promptSurf, promptRect)

        self.buttons.draw(self.screen)
        pygame.display.flip()

    def SwitchToScene(self, next_scene):
        super().SwitchToScene(next_scene)


class BoidsScene(SceneBase):
    N_BOIDS = 10

    def __init__(self):
        SceneBase.__init__(self)
        # initialize RNG
        self.rng = np.random.default_rng()

        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h

        self.boids = pygame.sprite.Group()

        for i in range(BoidsScene.N_BOIDS):
            x = screenWidth * self.rng.random()
            y = screenHeight * self.rng.random()
            z = self.rng.random()
            boid = Boid(Vector3D(x, y, z))
            self.boids.add(boid)

    # only needs to be called once throughout main loop
    def initGraphics(self, screen):
        SceneBase.initGraphics(self, screen)

        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h

    def ProcessInput(self, events, pressed_keys):
        for event in events:
            if event.type == pygame.KEYDOWN:
                alt_pressed = pressed_keys[pygame.K_LALT] or \
                              pressed_keys[pygame.K_RALT]
                if event.key == pygame.K_p:
                    self.SwitchToScene(Pause(self))

    def Update(self):
        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h
        self.boids.update()

    def Render(self):
        info = pygame.display.Info()
        screenWidth, screenHeight = info.current_w, info.current_h

        self.screen.fill(colors.BLACK)
        
        self.boids.draw(self.screen)
        pygame.display.flip()