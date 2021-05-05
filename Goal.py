# Timothy Stander
# This code creates a random maze, and draws it.
import pygame
import random
import sys
import time

COLUMNS = 1500
ROWS = 1200
CELL_SIZE = 2
MARGIN = 10
screen_x = COLUMNS*CELL_SIZE + 2*MARGIN
screen_y = ROWS*CELL_SIZE + 2*MARGIN
class Node:
    def __init__(self,item):
        self.item=item
        self.next=None
class Stack:
    def __init__(self):
        self.top=None
        self.append=self.push
    def push(self,item):
        n=Node(item)
        n.next=self.top
        self.top=n
    def isempty(self):
        return self.top is None
    def pop(self):
        if self.top is not None:
            item=self.top.item
            self.top=self.top.next
            return item
        else:
            return None
class Cell:
    def __init__(self):
        self.l = self.t = self.r = self.b = True
        self.visited = False

    def Draw(self, surface, i,j,sx,sy,cellsize, color=(0,0,0)):
        x1 = sx + i*cellsize
        y1 = sy + j*cellsize
        x2 = x1 + cellsize
        y2 = y1 + cellsize
        if self.l:
            pygame.draw.line(surface,color,(x1,y1),(x1,y2))
        if self.t:
            pygame.draw.line(surface,color,(x1,y1),(x2,y1))
        if self.r:
            pygame.draw.line(surface,color,(x2,y1),(x2,y2))
        if self.b:
            pygame.draw.line(surface,color,(x1,y2),(x2,y2))
    def __repr__(self):
        return " ".join(str(i) for i in [self.visited,self.l,self.r,self.b,self.t,"|"])
class Maze:
    def __init__(self,columns,rows,cellsize,x,y):
        self.columns=columns
        self.rows=rows
        self.cells = []
        self.cellsize=cellsize
        self.x=x
        self.y=y
        for i in range(columns*rows):
            self.cells.append(Cell())
            
    def ClearVisits(self):
        cells=self.cells
        for i in range(self.columns*self.rows):
            cells[i].visited = False

    def Clear(self):
        cells=self.cells
        for i in range(self.columns*self.rows):
            cells[i].l = True
            cells[i].r = True
            cells[i].t = True
            cells[i].b = True
    def Generate(self):
        stack=[]
        stack.append(0)
        cells=self.cells
        rows,cols=self.rows,self.columns
        while len(stack)>0:
            index=stack.pop()
            cells[index].visited = True
            nexti = []
            # determine which cells we could move to next
            if index%cols!=0 and not cells[index-1].visited: # left
                nexti.append(index-1)
            if index%cols!=cols-1 and not cells[index+1].visited: # right
                nexti.append(index+1)
            if int(index/cols)!=0 and not cells[index-cols].visited: # up
                nexti.append(index-cols)
            if int(index/cols)!=rows-1 and not cells[index+cols].visited: # down
                nexti.append(index+cols)

            if len(nexti) == 0:
                continue # nowhere to go from here
            
            # randomly choose 1 direction to go
            ni = nexti[random.randrange(len(nexti))]

            # knock out walls between this cell and the next cell
            if ni == index+1: # right move
                cells[index].r = cells[ni].l = False
            if ni == index-1: # left move
                cells[index].l = cells[ni].r = False
            if ni == index+cols: # down move
                cells[index].b = cells[ni].t = False
            if ni == index-cols: # up move
                cells[index].t = cells[ni].b = False

            # recursively visit the next cell
            stack.append(index)
            stack.append(ni)
    
    def Draw(self, surface, color=(0,0,0)):
        k=0
        for j in range(self.rows):
            for i in range(self.columns):
                self.cells[k].Draw(surface,i,j,self.x,self.y,self.cellsize, color)
                k+=1

    def Solve( self ):
        stack=[]
        moves=[]
        stack.append(0)
        stack.append(1)
        cols,rows=self.columns,self.rows
        todo=1
        cells=self.cells
        finish=cols*rows-1
        while len(stack)>0:
            todo=stack.pop()
            if todo:
                index=stack.pop()
                # Mark this cell as visited.
                cells[index].visited = True        

                # Get index number of this cell
                
                # Record the index in the class variable mMoves.
                moves.append(index)
                # If we are at the end cell, return true.
                if index==finish:
                    self.mMoves=moves
                    return moves
                
                stack.append(0)
                # move up if there is no wall, and it hasn't been visited. Return true if it returns true.
                if not cells[index].t and not cells[index-cols].visited:
                    stack.append(index-cols)
                    stack.append(1)
                # move left if there is no wall, and it hasn't been visited. Return true if it returns true.
                if not cells[index].l and not cells[index-1].visited:
                    stack.append(index-1)
                    stack.append(1)
                # move right if there is no wall, and it hasn't been visited. Return true if it returns true.
                if not cells[index].r and not cells[index+1].visited:
                    stack.append(index+1)
                    stack.append(1)
                # move down if there is no wall, and it hasn't been visited. Return true if it returns true.
                if not cells[index].b and not cells[index+cols].visited:
                    stack.append(index+cols)
                    stack.append(1)
            else:
                # This is a loser cell, so undo the move from self.mMoves, and return false to the previous cell.
                moves.pop()
        return None

    def DrawSolution(self, surface,color=(255,0,0)):
        size,x,y,cols=self.cellsize,self.x,self.y,self.columns
        for i in range(len(self.mMoves)-1):
            index1 = self.mMoves[i]
            index2 = self.mMoves[i+1]
            sx = x + index1 % cols*size + size//2
            sy = y + index1 // cols*size + size//2
            ex = x + index2 % cols*size + size//2
            ey = y + index2 // cols*size + size//2
        # Now draw it graphically!
            pygame.draw.line(surface,color,(sx,sy),(ex,ey))     


def main():
    pygame.init()
    surface = pygame.display.set_mode((screen_x,screen_y))
    surface.fill((255,255,0))
    theMaze = Maze(COLUMNS,ROWS,CELL_SIZE,MARGIN,MARGIN)
    theMaze.Generate()
    theMaze.ClearVisits()
##    t3=10000000
##    while True:
##        theMaze.Clear()
##        t1=time.clock()
##        theMaze.Generate()
##        t2=time.clock()
##        t4=t2-t1
##        if t3>t4:
##            t3=t4
##            print(t3)
##        theMaze.ClearVisits()
        
    theMaze.Draw(surface,(0,0,0))
    theMaze.Solve()
    theMaze.DrawSolution(surface,(255,0,0))
    running=True
    clock=pygame.time.Clock()
    pygame.image.save(surface,"test.png")
    while running:
        clock.tick(20)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running=False
        pygame.display.flip()
    pygame.quit()
main()
##        Stress Test
##times=1
##amount=20
##while True:
##    times*=2
##    t3=100000
##    for i in range(amount):
##        t1=time.clock()
##        for i in range(times):
##            0!=1
##        t2=time.clock()
##        t4=t2-t1
##        if t3>t4:
##            t3=t4
##    print(t3)
##    t3=100000
##    for i in range(amount):
##        t1=time.clock()
##        for i in range(times):
##            0<1
##        t2=time.clock()
##        t4=t2-t1
##        if t3>t4:
##            t3=t4
##    print(t3)
##    print()
