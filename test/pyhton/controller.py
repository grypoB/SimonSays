import serial
import sys
import time
import struct


MAX = 255
print "starting"
mega= serial.Serial('/dev/tty.usbmodem1411',9600)
print "mega go"

def send(byte):
    mega.write(byte)


def main():
    time.sleep(5);
    print "running"

    while True:
        time.sleep(1)
        send(bytearray([2, 150]))
        for x in xrange(1,151):
            send(bytearray([x,151-x,0]))

        time.sleep(1)
        send(bytearray([1, 150]))
        for x in xrange(1,151):
            send(bytearray([0,151-x,0]))

        time.sleep(1)
        send(bytearray([2, 150]))
        for x in xrange(1,151):
            send(bytearray([x,x,x]))

        time.sleep(1)
        send(bytearray([1, 150]))
        for x in xrange(1,151):
            send(bytearray([0,151-x,x]))


if __name__ == "__main__":
    main()
