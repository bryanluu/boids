import pygame
from linalg import Vector3D
import colors
import numpy as np
import utilities

class Boid(utilities.DrawSprite):
  def __init__(self, pos, vel):
    # Call the parent class (Sprite) constructor
    pygame.sprite.Sprite.__init__(self)

    self.position = pos
    self.velocity = vel
    self.width = 5
    self.rect = pygame.Rect(self.position.X, self.position.Y, self.width, self.width)
    self.image = pygame.Surface([self.width, self.width], flags=pygame.SRCALPHA)

  def update(self):
    self.position += self.velocity
    self.rect = pygame.Rect(self.position.X, self.position.Y, self.width, self.width)
    self.image.fill(self.color)

  @property
  def color(self):
    return *colors.WHITE, self.position.Z

