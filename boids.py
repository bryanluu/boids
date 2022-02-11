import pygame
from linalg import Vector3D
import colors
import numpy as np
import utilities

class Boid(utilities.DrawSprite):
  def __init__(self, pos):
    # Call the parent class (Sprite) constructor
    pygame.sprite.Sprite.__init__(self)

    self.pos = pos
    self.width = 5
    self.rect = pygame.Rect(self.pos.X, self.pos.Y, self.width, self.width)
    self.image = pygame.Surface([self.width, self.width], flags=pygame.SRCALPHA)

  def update(self):
    self.image.fill(self.color)

  @property
  def color(self):
    return *colors.WHITE, self.pos.Z

