#!/usr/bin/python


# http://raspberrypi.stackexchange.com/a/4754
import os
import select
from random import randint
from time import sleep

EXPORT_PATH = '/sys/class/gpio/export'
PIN_BASE_PATH = "/sys/class/gpio/gpio"

KEYS = [10, 22, 27]
LEDS = [23, 18, 17]

fds = {}

red_led = open(os.path.join('%s%d' % (PIN_BASE_PATH, 24), 'value'), 'w')

def initialize(pin_number, direction):
    try:
        write_once(EXPORT_PATH, pin_number)
    except:
        pass
    pin_path = '%s%d' % (PIN_BASE_PATH, pin_number)
    print pin_path
    write_once(os.path.join(pin_path, 'direction'), direction)
    write_once(os.path.join(pin_path, 'edge'), 'rising')
    file_io = open(os.path.join(pin_path, 'value'), 'w' if direction == 'out' else 'r')
    return file_io


def write_once(path, value):
    with open(path, 'w') as f:
        f.write("%s" % value)


def clean_leds():
    red_led.seek(0)
    red_led.write('0')
    for _, f in leds.iteritems():
        f.seek(0)
        f.write('0')

keys = {}
for pin_number in KEYS:
    file_io = initialize(pin_number, 'in')
    keys[pin_number] = file_io
    fds[file_io.fileno()] = pin_number

leds = {}
for pin_number in LEDS:
    leds[pin_number] = initialize(pin_number, 'out')

po = select.poll()
for pin_number, fd in keys.iteritems():
    po.register(fd, select.POLLPRI)


sequence = [randint(0, 2) for i in range(5)]
for i in sequence:
    clean_leds()
    sleep(1)
    leds[LEDS[i]].seek(0)
    leds[LEDS[i]].write('1')
    sleep(1)

guessed = 0
while 1:
    events = po.poll(30000)
    clean_leds()
    if not events:
        red_led.write('1')
        print ('TIMEOUT!')
    for fd, flag in events:
        os.lseek(fd, 0, 0)
        os.read(fd, 2)
        print 'poll() GPIO %d interrupt occurred' % fds[fd]
        leds[LEDS[KEYS.index(fds[fd])]].seek(0)
        leds[LEDS[KEYS.index(fds[fd])]].write('1')
        guessed += 1
        sleep(1/100)
        break
