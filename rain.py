import pygame
import random
import math
pygame.init()
s2=pygame.display.set_mode((800,600),pygame.RESIZABLE)
s=pygame.Surface((800,600),pygame.SRCALPHA)
clock=pygame.time.Clock()
class n:
    def __init__(self):
        self.x=random.random()*800
        self.y=random.random()*600
        self.d=random.random()*100+500
        self.dx=0
        self.dy=random.random()*5+5
particles=[n() for i in range(10000)]
paused=False
m=1 # more
k=-1 # way
g=True # gravity/friction
f=False # fake
c=(25,50,200)
z=0 # big stroke
i=False # interactive
u=True # update mouse
t=True # transparent
mx, my = pygame.mouse.get_pos()
while True:
    #clock.tick(60)
    events = pygame.event.get()
    if u:
        mx, my = pygame.mouse.get_pos()
    for event in events:
        if event.type == pygame.QUIT:
            pygame.quit()
            break
        if event.type == pygame.KEYDOWN:
            if event.key==pygame.K_p:
                paused=not paused
            if event.key==pygame.K_m:
                m*=8/7
            if event.key==pygame.K_l:
                m*=7/8
            if event.key==pygame.K_k:
                k*=-1
            if event.key==pygame.K_g:
                g=not g
            if event.key==pygame.K_f:
                f=not f
            if event.key==pygame.K_z:
                z+=1
                z%=3
            if event.key==pygame.K_u:
                u=not u
            if event.key==pygame.K_i:
                i=not i
            if event.key==pygame.K_t:
                t=not t
            if event.key==pygame.K_b:
                for p in particles:
                    p.x%=100
                    p.y%=100
                    p.dx=0
                    p.dy=0
    else:
        if not paused:
            w,h=s2.get_size()
            if (w,h)!=s.get_size():
                s=pygame.Surface((w,h),pygame.SRCALPHA)
            if t:
                pygame.draw.rect(s,(25,25,25,35),(0,0,w,h))
            else:
                pygame.draw.rect(s,(25,25,25),(0,0,w,h))
            
            if g:
                s.fill((0,75,0),(0,h-100,w,100))
            
            for p in particles:
                p.x+=p.dx
                p.y+=p.dy
                if g:
                    p.dy*=.99
                    p.dx*=.99
                    p.dy+=.05
                    if p.y>p.d:
                        p.d=h-random.random()*100
                        p.x=random.random()*w
                        p.y-=h
                        p.dx=0
                        p.dy=random.random()*5+5
                if i: #or (abs(mx-p.x)<m*50 and abs(my-p.y)<m*50):
                    ad=(abs(mx-p.x)**2+abs(my-p.y)**2+1)
                    if not f:
                        ad=math.sqrt(ad)
                    d=1/ad
                    if ad<d*m:# and d*m>ad:
                        p.x=mx+random.random()-.5
                        p.y=my+random.random()-.5
                        p.dx=0
                        p.dy=0
                    else:
                        p.dx-=d*(mx-p.x)*m*k
                        p.dy-=d*(my-p.y)*m*k
                if z==0:
                    s.fill(c,(p.x,p.y,1,4))
                elif z==1:
                    s.set_at((int(p.x),int(p.y)),c)
                else:
                    s.fill(c,(p.x,p.y-2,1,5))
                    s.fill(c,(p.x-2,p.y,5,1))
            s2.blit(s,(0,0))
        pygame.display.flip()
        continue
    break
