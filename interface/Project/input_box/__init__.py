# -*- coding: utf-8 -*-
from pygame import *
import func

colours = []
f = open('data/colours.txt', 'r')
for i in f.read().split():
    colours.append(map(int, i.split(',')))
f.close()
simbols = {}
f = open('data/simbols.txt', 'r')
for i in f.read().split():
    j = map(int, i.split(':'))
    simbols[chr(j[0])] = chr(j[1])
f.close()

class input_button:
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
            return unicode(self.f())
    def draw(self):
        if self.in_button():
            self.scr.blit(self.pic[1], self.pos)
        else:
            self.scr.blit(self.pic[0], self.pos)

class input_box:
    def __init__(self, pos, screen, pic):
        self.pos = pos
        self.scr = screen
        self.str = unicode("")
        self.opened = False
        self.curs = len(self.str)
        self.open_button = input_button([self.pos[0] + self.pos[2] + 2, self.pos[1], 30, self.pos[3]],
                                        func.file_dialog, screen, [pic[3], pic[4]])
        self.pic = pic
    def draw(self):
        box_font = font.SysFont("Consolas", 20)
        self.open_button.draw()
        if self.opened:
            self.scr.blit(self.pic[2], self.pos)
            self.scr.blit(box_font.render("|", True, colours[0]),
                          [self.pos[0] + 11 * min(self.curs, (self.pos[2] - 10) / 11), self.pos[1] + 6])
        else:
            if self.in_box():
                self.scr.blit(self.pic[1], self.pos)
            else:
                self.scr.blit(self.pic[0], self.pos)
            if len(self.str) == 0:
                self.scr.blit(box_font.render("Enter a path", True, colours[5]),[self.pos[0] + 5, self.pos[1] + 6])
        self.scr.blit(box_font.render(self.str[max(0, self.curs - (self.pos[2] - 10) / 11) : max(self.curs, (self.pos[2] - 10) / 11)],
                                      True, colours[0]),[self.pos[0] + 5, self.pos[1] + 6])
    def in_box(self):
        mouse_pos = mouse.get_pos()
        if (self.pos[0] <= mouse_pos[0] <= self.pos[2] + self.pos[0] and
                        self.pos[1] <= mouse_pos[1] <= self.pos[3] + self.pos[1]):
            return True
        else:
            return False
    def click(self):
        if self.in_box():
            self.opened = not self.opened
        sss = unicode(self.open_button.click())
        if sss != "None":
            self.opened = False
            self.str = sss
            self.curs = len(self.str)
    def input(self, ev):
        if self.opened:
            if ev.type == KEYDOWN:
                if 31 < ev.key < 127:
                    if ev.mod & (KMOD_CAPS | KMOD_SHIFT):
                        if chr(ev.key) in simbols:
                            self.str = self.str[0 : self.curs] + simbols[chr(ev.key)] + self.str[self.curs : len(self.str)]
                        else:
                            self.str = self.str[0 : self.curs] + chr(ev.key).upper() + self.str[self.curs : len(self.str)]
                    else:
                        self.str = self.str[0 : self.curs] + chr(ev.key) + self.str[self.curs : len(self.str)]
                    self.curs += 1
                if ev.key == 8 and self.curs > 0:
                    self.str = self.str[0 : self.curs - 1] + self.str[self.curs : len(self.str)]
                    self.curs -= 1
                if ev.key == K_LEFT:
                    self.curs = max(0, self.curs - 1)
                if ev.key == K_RIGHT:
                    self.curs = min(len(self.str), self.curs + 1)
                if ev.key == K_RETURN:
                    self.opened = not self.opened