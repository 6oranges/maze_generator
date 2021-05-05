import pygame
pygame.init()

def convert(prefix,amount,nprefix):
    for i in range(amount):
        pygame.image.save(pygame.image.load(prefix+str(i+1)+".ppm"),nprefix+str(i+1)+".png")
convert("hard",1,"Mazes/hard")
convert("medium",4,"Mazes/medium")
convert("easy",16,"Mazes/easy")
convert("hardSolved",1,"Mazes/hardSolved")
convert("mediumSolved",4,"Mazes/mediumSolved")
convert("easySolved",16,"Mazes/easySolved")
