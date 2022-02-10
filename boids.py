import pygame
from linalg import Vector3D
import colors
import numpy as np
import utilities

class Boid(utilities.DrawSprite):
  def __init__(self, pos):
    self.pos = pos
    self.width = 5
    self._width = None

  @property
  def color(self):
    return *colors.WHITE, self.pos.Z

  @property
  def rect(self):
    if self._width != self.width:
      self._width = self.width
      self._rect = pygame.Rect(self.pos.X, self.pos.Y, self.width, self.width)
    return self._rect

  def draw(self, screen):
    screen.fill(self.color, self.rect, pygame.BLEND_RGBA_ADD)