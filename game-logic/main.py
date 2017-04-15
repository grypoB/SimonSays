import serial
import datetime
import random
import time
import sys

# api
# CMD AREA
# Blink 

class Output:
    def idling(self):
        print "idling"

        #turn everything off
        for x in xrange(0,10):
            cmd(0,x)

        # set blinking button
        for x in xrange(1,5):
            cmd(2,x)

    def flash(self, up=10, down=50):
        print "flashing boys!"
        # set flash button 
        for x in xrange(1,5):
            cmd(4,x)
        # beam everyone white
        for x in xrange(5,9):
            cmd(1,x)

        #turn off buttons
        for x in xrange(1,5):
            cmd(0,x)

        cmd(5,0)
        cmd(up,down)
        cmd(5,9)
        cmd(up,down)

    def fill(self, button):
        print "filling button ", button
        # fill button
        cmd(3,button+4)


    def fake_press(self, button):
        print "fake pressing ", button
        # flash button
        cmd(4, button)
        # beam from button
        cmd(1, button+4)

    def win(self):
        print "Champion ..."
        print "Champion ..."
        print "Champion at the Puntnam county spelling beeeeeeeeeeee"

    def game_over(self):
        # turn off strips and crystal
        # beam red from crystal
        # sleep a sec to get timing right
        # turn buttons red for a sec
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
    BREATH_UP   = [0,100,50,20,10]
    BREATH_DOWN = [0,100,50,20,10]
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
                self.state = self.FLASHING
                self.was_idle = False

            #self.ank_combi = 0 # number of button pressed
            self.rank_stage = 0 # number of stages played
        elif self.state == self.FLASHING:
            #locking, display the lights
            self.output.flash(self.BREATH_UP(self.rank_stage),self.BREATH_DOWN(self.rank_stage))
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
            self.prev_time = self.cur_time
            self.rank_combi = 0


        elif self.state == self.USER_COMBI:
            print "button press?"
            if self.button_press != 0:
                self.prev_time = self.cur_time
                if self.button_press == self.true_combi[self.rank_combi]:
                    self.rank_combi += 1
                    # display some cool light flash
                    self.output.fake_press(self.button_press)
                    if self.rank_combi == len(self.true_combi):
                        print "that was the last of them"
                        self.output.fill(self.button_press)
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
        




uno = serial.Serial('/dev/tty.usbmodem1421',9600)

def cmd(byte1, byte2):
    send(byte1)
    send(byte2)
    print "sent ", byte1, " ", byte2
    time.sleep(0.005)

def send(byte):
    uno.write(bytearray([byte]))

def read_line():
    while uno.in_waiting:
        sys.stdout.write(uno.readline())

def main():
    time.sleep(3)
    # init buttons

    # init fsm
    light = Output()
    fsm = Fsm(light)
    
    while True:
        # check buttons
        fsm.button_press = input("?")
        # update
        fsm.update()
        
        time.sleep(0.1)
        read_line()

        




if __name__ == "__main__":
    main()
