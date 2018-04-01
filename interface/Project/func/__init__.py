# -*- coding: utf-8 -*-
import subprocess
import os
import Tkinter
import tkFileDialog

def test(s_way):
    way = '"' + os.path.abspath('core 0.6.exe') + '"'
    p = subprocess.Popen(way + s_way, shell = True, stdout = subprocess.PIPE)
    while True:
        if p.poll() is not None:
            print p.stdout.read()
            break

def ps():
    pass

def file_dialog():
    global string
    root = Tkinter.Tk()
    root.withdraw()
    return unicode(tkFileDialog.askdirectory())