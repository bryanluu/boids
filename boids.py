import pygame
from linalg import Vector3D
import colors
import numpy as np
import utilities

class Boid(utilities.DrawSprite):
  MARGIN = 50 # how close to edges are boids allowed?
  TURN_FACTOR = 1 # how quickly do boids avoid edges?
  MIN_SPEED = 5
  MAX_SPEED = 10
  PROTECTED_RANGE = 10 # the range at which boids get paranoid of collisions with others
  AVOID_FACTOR = 0.5 # how quickly do boids avoid each other in the protected range?
  flock = [] # the existing flock of boids

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
    vx = 5 * (1 + rng.random())
    vy = 5 * (1 + rng.random())
    vz = 10 * rng.random()

    self.position = Vector3D(x, y, z)
    self.velocity = Vector3D(vx, vy, vz)
    self.closeness = Vector3D.zero()
    self.width = 5
    self.rect = pygame.Rect(self.position.X - self.width / 2, 
                            self.position.Y - self.width / 2,
                            self.width, self.width)
    self.image = pygame.Surface([self.width, self.width], flags=pygame.SRCALPHA)

    Boid.flock.append(self)

  @property
  def color(self):
    return *colors.WHITE, self.position.Z

  def constrain_speed(self):
    speed = abs(self.velocity)
    if speed > Boid.MAX_SPEED:
      self.velocity = (self.velocity * Boid.MAX_SPEED) / speed
    if speed < Boid.MIN_SPEED:
      self.velocity = (self.velocity * Boid.MIN_SPEED) / speed

  def avoid_edges(self):
    if self.position.X < self.left + Boid.MARGIN:
      self.velocity.X += Boid.TURN_FACTOR
    if self.position.X > self.right - Boid.MARGIN:
      self.velocity.X -= Boid.TURN_FACTOR
    if self.position.Y < self.top + Boid.MARGIN:
      self.velocity.Y += Boid.TURN_FACTOR
    if self.position.Y > self.bottom - Boid.MARGIN:
      self.velocity.Y -= Boid.TURN_FACTOR
    if self.position.Z < self.back + Boid.MARGIN:
      self.velocity.Z += Boid.TURN_FACTOR
    if self.position.Z > self.front - Boid.MARGIN:
      self.velocity.Z -= Boid.TURN_FACTOR

  def constrain_position(self):
    self.position.X = utilities.constrain(self.position.X, self.left, self.right)
    self.position.Y = utilities.constrain(self.position.Y, self.top, self.bottom)
    self.position.Z = utilities.constrain(self.position.Z, self.back + (self.areaDepth * 0.5), self.front)

  def calculate_closeness_to_flock(self):
    self.closeness = Vector3D.zero()
    for other in Boid.flock:
      if self is not other:
        distance = abs(self.position - other.position)
        if distance < Boid.PROTECTED_RANGE:
          self.closeness += self.position - other.position

  def avoid_other_boids(self):
    self.calculate_closeness_to_flock()
    self.velocity += self.closeness * Boid.AVOID_FACTOR

  def update(self):
    self.avoid_other_boids()
    self.avoid_edges()
    self.constrain_speed()
    self.position += self.velocity
    self.constrain_position()
    self.rect = pygame.Rect(self.position.X - self.width / 2, 
                            self.position.Y - self.width / 2,
                            self.width, self.width)
    self.image.fill(self.color)