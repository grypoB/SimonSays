import serial
import datetime
import random

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

    MAX_TIMEOUT = 3000 # time in ms

    rank_combi = 0 # number of button pressed
    rank_stage = 0 # number of stages played

    prev_time = datetime.datetime.now() # time from no  activity since then
    cur_time = datetime.datetime.now() # current time

    state = IDLE # current state

    true_combi = [] # 1 indexing
    #user_combi = []

    def update():
        self.cur_time = datetime.datetime.now()
        if self.state == IDLE:
            #do some idling stuff, none locking
        elif self.state == FLASHING:
            #locking, display the lights
            if self.rank_combi== len(MIN_COMBI_L):
                self.state = WIN
            else:
                self.state = TRUE_COMBI

        elif self.state == TRUE_COMBI:
            # locking
            self.generate_true_combi()
            
            for x in self.true_combi:
                # beam from crystal to x
                # beam form x to crystal
            self.state = USER_COMBI
            self.rank_combi = 0


        elif self.state == USER_COMBI:

            if self.button_press != 0:
                self.prev_time = self.cur_time
                if self.button_press == self.true_combi[self.rank_combi]:
                    self.rank_combi += 1
                    # display some cool light flash
                    if self.rank_combi == len(self.true_combi):
                        self.rank_stage += 1
                        self.state = FLASHING 
                else:
                    self.state = GAME_OVER

            elif (self.cur_time-self.prev_time).total_seconds() > TIMEOUT:
                self.state = GAME_OVER
            # else don't do anything and you might just timeout
        elif self.state == WIN:
            # do some great stuff
        elif self.state == GAME_OVER:
            #do some dank stuff
        else:
            print "fsm in impossible states..."

        self.button_press = 0


    
    def generate_true_combi():
        length = random.randint(MIN_COMBI_L[rank_stage], MAX_COMBI_L[rank_stage])
        true_combi = []
        
        for i in xrange(0,length):
            true_combi.append(random.randint(1,N_BUTTON))
        




uno = serial.Serial('/dev/tty.usbmodem1411',115200)

def send(byte):
    uno.write(byte)

def read_line():
    while uno.in_waiting:
        sys.stdout.write(uno.readline())

def main():
    random.init()
    # init buttons

    # init fsm
    fsm = Fsm()
    
    while True:
        # check buttons
        




if __name__ == "__main__":
    main()
