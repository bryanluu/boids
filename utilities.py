import pygame

# Sprite class with a draw function
class DrawSprite(pygame.sprite.Sprite):
  def draw(self, screen):
    screen.blit(self.image, self.rect)


# Group class that relies on the DrawSprite draw function
class DrawGroup(pygame.sprite.Group):
  def draw(self, screen):
    for sprite in self.sprites():
      sprite.draw(screen)