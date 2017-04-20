import serial
import datetime
import random
import time
import sys
import RPi.GPIO as GPIO
import pygame

pygame.init()

sound_bip = [ pygame.mixer.Sound("A_sharp.ogg") ]
sound_bip.append(pygame.mixer.Sound("C_sharp.ogg"))
sound_bip.append(pygame.mixer.Sound("D_sharp.ogg"))
sound_bip.append(pygame.mixer.Sound("F_sharp.ogg"))
sound_lost = pygame.mixer.Sound("losegame.wav")
sound_win  = pygame.mixer.Sound("win.wav")
sound_next_level = pygame.mixer.Sound("nextlevel.ogg")
pygame.mixer.music.load("background.wav")
pygame.mixer.music.play(-1)

class Buttons:
    
    n = 4
    pin = [17,18,23,24]
    old_state = [1, 1, 1, 1]
    cur_state = [1, 1, 1, 1]

    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        for i in xrange(0,self.n):
            GPIO.setup(self.pin[i], GPIO.IN, pull_up_down=GPIO.PUD_UP)

    def update(self):
        button_pressed = -1 

        for i in xrange(0,self.n):
            self.cur_state[i] = GPIO.input(self.pin[i])
            if self.cur_state[i]==0 and self.old_state[i]==1:
                button_pressed = i
                print i+1, " on rising edge"
        self.old_state = self.cur_state[:]

        return button_pressed+1

class Output:
    def idling(self):
        print "idling"

        #turn everything off
        for x in xrange(0,10):
            cmd(0,x)

        # set blinking button
        for x in xrange(1,5):
            cmd(2,x)

    def flash(self, stage):
        print "flashing boys!"
        time.sleep(1)
        #pygame.mixer.music.pause()
        #pygame.mixer.Sound.play(sound_next_level)
        sound_next_level.play()
        # set flash button 
        for x in xrange(1,5):
            cmd(4,x)
        # beam everyone white
        for x in xrange(5,9):
            cmd(1,x)

        #turn off buttons
        for x in xrange(1,5):
            cmd(0,x)

        if stage!=0:
            cmd(7+stage,0)
            cmd(7+stage,9)
        else:
            cmd(0,0)
            cmd(9,0)
        time.sleep(2)
        #pygame.mixer.music.unpause()

    def fill(self, button):
        print "filling button ", button
        # fill button
        cmd(3,button+4)


    def fake_press(self, button):
        print "fake pressing ", button
        pygame.mixer.Sound.play(sound_bip[button-1])
        # flash button
        cmd(4, button)
        # beam from button
        cmd(1, button+4)
        time.sleep(0.2)

    def win(self):
        pygame.mixer.music.pause()
        pygame.mixer.Sound.play(sound_win)
        cmd(6,0)
        cmd(6,9)
        print "Champion ..."
        print "Champion ..."
        print "Champion at the Puntnam county spelling beeeeeeeeeeee"
	time.sleep(15)
        pygame.mixer.music.unpause()

    def game_over(self):
        # turn off crystal
        pygame.mixer.Sound.play(sound_lost)
        cmd(0,0)
        cmd(0,9)
        for x in xrange(1,5): #unfill hsv
            cmd(7,x+4)
        time.sleep(4)
        print "Game over..."
        print "yeah, game over"


class Fsm:
    # fsm states
    IDLE = 0
    FLASHING = 1
    TRUE_COMBI = 2
    USER_COMBI = 3
    WIN = 4
    GAME_OVER = 5

    MIN_COMBI_L = [1,2,3,4] # for each stage
    MAX_COMBI_L = [1,2,3,4]
    POST_ADD_COMBI = [1,2,3,4]
    BREATH_UP   = [0,15,10,10,10]
    BREATH_DOWN = [0,70,50,25,25]
    N_BUTTON = 4

    MAX_TIMEOUT = 5 # time in s

    rank_combi = 0 # number of button pressed
    rank_stage = 0 # number of stages played

    prev_time = datetime.datetime.now() # time from no  activity since then
    cur_time = datetime.datetime.now() # current time

    state = IDLE # current state
    was_idle = False

    true_combi = [] # 1 indexing
    #user_combi = []

    def __init__(self,output):
        self.output = output

    def update(self):
        self.cur_time = datetime.datetime.now()

        if self.state == self.IDLE:
            #do some idling stuff, none locking
            if self.was_idle == False:
                self.output.idling()
                self.was_idle = True

            if self.button_press:
                self.output.fake_press(self.button_press)
                self.state = self.FLASHING
                self.was_idle = False

            #self.ank_combi = 0 # number of button pressed
            self.rank_stage = 0 # number of stages played
        elif self.state == self.FLASHING:
            #locking, display the lights
            self.output.flash(self.rank_stage)
            if self.rank_stage == len(self.MIN_COMBI_L):
                self.state = self.WIN
            else:
                self.state = self.TRUE_COMBI

        elif self.state == self.TRUE_COMBI:
            # locking
            self.generate_true_combi()
            print "new combi boys (stage ", self.rank_stage, ")"
            for x in self.true_combi:
                time.sleep(1)
                self.output.fake_press(x)
                # wait a sec

            self.state = self.USER_COMBI
            self.prev_time = datetime.datetime.now() 
            self.rank_combi = 0


        elif self.state == self.USER_COMBI:
            #print "button press?"
            if self.button_press != 0:
                self.prev_time = self.cur_time
                if self.button_press == self.true_combi[self.rank_combi]:
                    self.rank_combi += 1
                    # display some cool light flash
                    self.output.fake_press(self.button_press)
                    if self.rank_combi == len(self.true_combi):
                        print "that was the last of them"
                        self.output.fill(self.button_press)
			#time.sleep(1)
                        self.rank_stage += 1
                        self.state = self.FLASHING 
                else:
                    self.state = self.GAME_OVER

            elif (self.cur_time-self.prev_time).total_seconds() > self.MAX_TIMEOUT:
                print "timeout"
                self.state = self.GAME_OVER
            # else don't do anything and you might just timeout
        elif self.state == self.WIN:
            # do some great stuff
            self.output.win()
            self.state = self.IDLE
        elif self.state == self.GAME_OVER:
            #do some dank stuff
            self.output.game_over()
            self.state = self.IDLE
        else:
            print "fsm in impossible states..."

        self.button_press = 0


    
    def generate_true_combi(self):
        length = random.randint(self.MIN_COMBI_L[self.rank_stage], self.MAX_COMBI_L[self.rank_stage])
        self.true_combi = []
        
        for i in xrange(0,length):
            self.true_combi.append(random.randint(1,self.N_BUTTON))
        self.true_combi.append(self.POST_ADD_COMBI[self.rank_stage])
        print "and the combi is ", self.true_combi
        




#uno = serial.Serial('/dev/tty.usbmodem1421',9600)
uno = serial.Serial('/dev/ttyACM0',9600)

def cmd(byte1, byte2):
    send((byte1<<4) | byte2)
    print "sent ", byte1, " ", byte2

def send(byte):
    uno.write(bytearray([byte]))
    time.sleep(0.005)

def read_line():
    while uno.in_waiting:
        sys.stdout.write(uno.readline())

def main():
    time.sleep(3)
    # init buttons
    butt = Buttons()

    # init fsm
    light = Output()
    fsm = Fsm(light)
    
    while True:
        # check buttons
        fsm.button_press = butt.update()
        #fsm.button_press = input("?")

        # update
        fsm.update()
        
        #time.sleep(0.1)
        #read_line()

        




if __name__ == "__main__":
    main()
