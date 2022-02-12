import pygame
from linalg import Vector3D
import colors
import numpy as np
import utilities

class Boid(utilities.DrawSprite):
  MIN_SPEED = 5
  MAX_SPEED = 10

  def __init__(self, bounds):
    # Call the parent class (Sprite) constructor
    pygame.sprite.Sprite.__init__(self)

    # initialize RNG
    rng = np.random.default_rng()

    (self.left, self.right), (self.top, self.bottom), (self.back, self.front) = bounds

    self.areaWidth = self.right - self.left
    self.areaHeight = self.bottom - self.top
    self.areaDepth = self.front - self.back

    x = self.left + self.areaWidth * rng.random()
    y = self.top + self.areaHeight * rng.random()
    z = self.back + 200 * rng.random()
    vx = vy = 5 * (1 + rng.random())
    vz = 10 * rng.random()

    self.position = Vector3D(x, y, z)
    self.velocity = Vector3D(vx, vy, vz)
    self.width = 5
    self.rect = pygame.Rect(self.position.X, self.position.Y, self.width, self.width)
    self.image = pygame.Surface([self.width, self.width], flags=pygame.SRCALPHA)

  @property
  def color(self):
    return *colors.WHITE, self.position.Z

  def update(self):
    self.constrain_speed()
    self.position += self.velocity
    self.rect = pygame.Rect(self.position.X, self.position.Y, self.width, self.width)
    self.image.fill(self.color)

  def constrain_speed(self):
    speed = abs(self.velocity)
    if speed > Boid.MAX_SPEED:
      self.velocity = (self.velocity * Boid.MAX_SPEED) / speed
    if speed < Boid.MIN_SPEED:
      self.velocity = (self.velocity * Boid.MIN_SPEED) / speed