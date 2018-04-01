from button import *

colours = []
f = open('data/colours.txt', 'r')
for i in f.read().split():
    colours.append(map(int, i.split(',')))

class cluster:
    def open(self):
        self.opened *= -1
    def __init__(self, buttons, ibs, pos, screen, pic):
        self.buttons = buttons
        self.opened = -1
        self.open_button = button(pos, self.open, screen, [pic[0], pic[1]])
        self.scr = screen
        self.ibs = ibs
        self.pic = pic
    def draw(self):
        if self.opened == -1:
            self.open_button.pic = [self.pic[0], self.pic[1]]
        else:
            self.open_button.pic = [self.pic[2], self.pic[2]]
            for cur in self.buttons:
                cur.draw()
            for cur in self.ibs:
                cur.draw()
        self.open_button.draw()
    def click(self):
        if self.opened == -1:
            pass
        else:
            for cur in self.buttons:
                cur.click()
            for cur in self.ibs:
                cur.click()
        self.open_button.click()
    def input(self, ev):
        for cur in self.ibs:
            cur.input(ev)

class cluster_panel:
    def __init__(self, clusters):
        self.cl = clusters
        self.opened_bar = len(clusters) - 1
    def draw(self):
        for j in self.cl:
            j.draw()
    def click(self):
        for j in xrange(len(self.cl)):
            self.cl[j].click()
            if self.cl[j].opened == 1 and j != self.opened_bar:
                self.cl[self.opened_bar].opened = -1
                self.opened_bar = j
    def input(self, ev):
        for j in self.cl:
            j.input(ev)

class radio_panel:
    def __init__(self, buttons, pos, screen, pic):
        self.rb = buttons
        self.active = len(buttons) - 1
        self.opened = False
        self.scr = screen
        self.pos = pos
        self.pic = pic
        self.rb[self.active].ch = True
        for j in pic:
            j.set_colorkey([255, 255, 255])
    def in_panel(self):
        mouse_pos = mouse.get_pos()
        if (self.pos[0] <= mouse_pos[0] <= self.pos[2] + self.pos[0] and
                        self.pos[1] <= mouse_pos[1] <= self.pos[3] + self.pos[1]):
            return True
        else:
            return False
    def draw(self):
        xfont = font.SysFont("Consolas", 20)
        if self.opened:
            for j in self.rb:
                j.draw(j.pos)
            self.scr.blit(self.pic[2], self.pos)
        else:
            if self.in_panel():
                self.scr.blit(self.pic[1], self.pos)
            else:
                self.scr.blit(self.pic[0], self.pos)
            self.scr.blit(xfont.render(self.rb[self.active].val, True, colours[0]), [self.pos[0] + 10, self.pos[1] + 6])
    def click(self):
        if self.opened:
            for j in xrange(len(self.rb)):
                self.rb[j].click()
                if self.rb[j].ch and j != self.active:
                    self.rb[self.active].ch = False
                    self.active = j
                self.opened = False
        else:
            if self.in_panel():
                self.opened = True
