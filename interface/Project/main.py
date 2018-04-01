from cluster import *
from input_box import *
import func

init()

def main():
    screen_size = [1120, 630]
    screen = display.set_mode(screen_size)
    display.set_caption("Antivirus")
    main_scr = image.load('data/main.png').convert()
    main_bar = image.load('data/bar/bar.png').convert()
    main_btn = [image.load('data/buttons/main.png').convert(), image.load('data/buttons/main1.png').convert()]
    share_btn = [image.load('data/buttons/share.png').convert(), image.load('data/buttons/share1.png').convert()]
    bar_btn = [image.load('data/bar/btn.png').convert(), image.load('data/bar/btn1.png').convert(),
               image.load('data/bar/btno.png').convert()]
    bar_btn1 = [image.load('data/bar/btn10.png').convert(), image.load('data/bar/btn11.png').convert(),
               image.load('data/bar/btn1o.png').convert()]
    inp_box = [image.load('data/input_box/input_box.png').convert(),
               image.load('data/input_box/input_box1.png').convert(),
               image.load('data/input_box/input_box_o.png').convert(),
               image.load('data/input_box/browse.png').convert(),
               image.load('data/input_box/browse1.png').convert()]
    sett = [image.load('data/bar/sett.png').convert(), image.load('data/bar/sett1.png').convert(),
               image.load('data/bar/setto.png').convert()]
    rp = [image.load('data/buttons/radio_buttons/panelc.png').convert(),
          image.load('data/buttons/radio_buttons/panelc1.png').convert(),
          image.load('data/buttons/radio_buttons/panelo.png').convert()]
    rb = [image.load('data/buttons/radio_buttons/btn.png').convert(), image.load('data/buttons/radio_buttons/btn2.png').convert(),
          image.load('data/buttons/radio_buttons/btn1.png').convert(), image.load('data/buttons/radio_buttons/btn3.png').convert()]

    x = radio_panel([radio_button([383, 310, 300, 30], "Full Scan", screen, rb), radio_button([383, 340, 300, 30], "Only Executable Scan", screen, rb)],
                   [383, 300, 300, 30], screen, rp)
    c1 = cluster([button([443, 240, 193, 50], func.test, screen, main_btn), x], [input_box([250, 200, 580, 30], screen, inp_box)],
                 [5, 5, 100, 40], screen, bar_btn)
    c2 = cluster([button([443, 240, 193, 50], func.ps, screen, share_btn)], [input_box([250, 200, 580, 30], screen, inp_box)],
                 [110, 5, 100, 40], screen, bar_btn1)
    c3 = cluster([], [], [1075, 5, 40, 40], screen, sett)
    c = cluster_panel([c1, c2, c3])

    screen_opened = True

    fps = time.Clock()
    while screen_opened:

        screen.blit(main_scr,[0, 0])
        screen.blit(main_bar,[0, 0])
        c.draw()
        display.flip()

        for cur in event.get():
            if cur.type == MOUSEBUTTONUP:
                if cur.button == 1:
                    c.click()
            c.input(cur)
            if cur.type == QUIT:
                screen_opened = False
                quit()

        fps.tick(30)
main()