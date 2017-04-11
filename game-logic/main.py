import serial
import datetime
import random

class Output:
    def idling(self):
        print "idling"

    def flash(self):
        print "flashing boys!"

    def beam(self, start, end):
        if start==0:
            start = "crystal"
        print "beaming ", start, " -> ", end

    def win(self):
        print "Champion ..."
        print "Champion ..."
        print "Champion at the Puntnam county spelling beeeeeeeeeeee"

    def game_over(self):
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

    MIN_COMBI_L = [3,4,5] # for each stage
    MAX_COMBI_L = [3,5,7]
    N_BUTTON = 4

    MAX_TIMEOUT = 5 # time in s

    rank_combi = 0 # number of button pressed
    rank_stage = 0 # number of stages played

    prev_time = datetime.datetime.now() # time from no  activity since then
    cur_time = datetime.datetime.now() # current time

    state = IDLE # current state

    true_combi = [] # 1 indexing
    #user_combi = []

    def __init__(self,output):
        self.output = output

    def update(self):
        self.cur_time = datetime.datetime.now()
        if self.state == self.IDLE:
            #do some idling stuff, none locking
            self.output.idling()
            if self.button_press:
                self.state = self.FLASHING

            #self.ank_combi = 0 # number of button pressed
            self.rank_stage = 0 # number of stages played
        elif self.state == self.FLASHING:
            #locking, display the lights
            self.output.flash()
            if self.rank_stage == len(self.MIN_COMBI_L):
                self.state = self.WIN
            else:
                self.state = self.TRUE_COMBI

        elif self.state == self.TRUE_COMBI:
            # locking
            self.generate_true_combi()
            print "new combi boys (stage ", self.rank_stage, ")"
            for x in self.true_combi:
                # beam from crystal to x
                self.output.beam(0,x)
                # beam form x to crystal
                self.output.beam(x,0)
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
                    self.output.beam(self.button_press,0)
                    if self.rank_combi == len(self.true_combi):
                        print "that was the last of them"
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
        print "and the combi is ", self.true_combi
        




#uno = serial.Serial('/dev/tty.usbmodem1411',115200)

def send(byte):
    uno.write(byte)

def read_line():
    while uno.in_waiting:
        sys.stdout.write(uno.readline())

def main():
    # init buttons

    # init fsm
    light = Output()
    fsm = Fsm(light)
    
    while True:
        # check buttons
        fsm.button_press = input("?")
        # update
        fsm.update()

        




if __name__ == "__main__":
    main()
