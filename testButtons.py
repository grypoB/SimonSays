mport RPi.GPIO as GPIO
import time

pin = [17,18,27,22]
old_state = [0, 0, 0, 0]
cur_state = [0, 0, 0, 0]

GPIO.setmode(GPIO.BCM)

for i in xrange(0,4):
    GPIO.setup(pin[i], GPIO.IN, pull_up_down=GPIO.PUD_UP)

while True:
    for i in xrange(0,4):
        cur_state[i] = GPIO.input(pin[i])
        if cur_state[i]==False and old_state[i]==True:
            print i, " on rising edge"
