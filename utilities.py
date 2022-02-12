import pygame

# Math function to normalize (set between 0 and 1) a value given its range
def normalize(value, value_range):
  low, high = value_range
  return (value - low)/(high - low)

# Math function to constrain value between limits
def constrain(value, low, high):
  return min(max(value, low), high)

# Sprite class with a draw function
class DrawSprite(pygame.sprite.Sprite):
  def draw(self, screen):
    screen.blit(self.image, self.rect)

# Group class that relies on the DrawSprite draw function
class DrawGroup(pygame.sprite.Group):
  def draw(self, screen):
    for sprite in self.sprites():
      sprite.draw(screen)