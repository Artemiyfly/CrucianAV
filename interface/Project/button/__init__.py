from pygame import *

colours = []
f = open('data/colours.txt', 'r')
for i in f.read().split():
    colours.append(map(int, i.split(',')))

class button:
    def __init__(self, pos, func, screen, pic):
        self.pos = pos
        self.f = func
        self.scr = screen
        self.pic = pic
    def in_button(self):
        mouse_pos = mouse.get_pos()
        if (self.pos[0] <= mouse_pos[0] <= self.pos[2] + self.pos[0] and
                        self.pos[1] <= mouse_pos[1] <= self.pos[3] + self.pos[1]):
            return True
        else:
            return False
    def click(self):
        if self.in_button():
            return self.f()
    def draw(self):
        if self.in_button():
            self.scr.blit(self.pic[1], self.pos)
        else:
            self.scr.blit(self.pic[0], self.pos)
    def draw_click(self, ev):
        pass

class radio_button:
    def __init__(self, pos, value, screen, pic):
        self.pos = pos
        self.val = value
        self.scr = screen
        self.ch = False
        self.pic = pic
    def in_button(self):
        mouse_pos = mouse.get_pos()
        if (self.pos[0] <= mouse_pos[0] <= self.pos[2] + self.pos[0] and
                        self.pos[1] <= mouse_pos[1] <= self.pos[3] + self.pos[1]):
            return True
        else:
            return False
    def click(self):
        if self.in_button():
            self.ch = not self.ch
    def draw(self, pos):
        xfont = font.SysFont("Consolas", 20)
        if not self.ch:
            if self.in_button():
                self.scr.blit(self.pic[1], pos)
            else:
                self.scr.blit(self.pic[0], pos)
        else:
            if self.in_button():
                self.scr.blit(self.pic[3], pos)
            else:
                self.scr.blit(self.pic[2], pos)
        self.scr.blit(xfont.render(self.val, True, colours[0]), [self.pos[0] + 10, self.pos[1] + 6])