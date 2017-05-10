import RPi.GPIO as GPIO
import time
#n = 4
#pin = [17,18,23,24]
n = 1
pin = [22]
#old_state = [0, 0, 0, 0]
#cur_state = [0, 0, 0, 0]
cur_state = [0]
old_state = [0]

GPIO.setmode(GPIO.BCM)

for i in xrange(0,n):
    GPIO.setup(pin[i], GPIO.IN, pull_up_down=GPIO.PUD_UP)

while True:
    #print old_state
    print cur_state 
    for i in xrange(0,n):
        cur_state[i] = GPIO.input(pin[i])
        #print i, " ", cur_state[i] 
        if cur_state[i]==0 and old_state[i]==1:
            print i, " on rising edge"
    old_state = cur_state[:]
    #time.sleep(1)
