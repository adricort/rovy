#!/usr/bin/env python

import pygame
import time
import serial
from playsound import playsound

offset_clock = 1.8
Illumination_Theory_duration = 1307.121

ser = serial.Serial('/dev/ttyACM0', 115200)

time.sleep(5)

# Instant in seconds, code as string
def configuration(minute, seconds, code):
    while True:
        instant = minute*60+seconds
        start = time.time()
        timer = (start-offset_time)+offset_clock
        if timer > instant:
            ser.write(code)
            print "Moment:",minute,":",seconds,"  Code:",code
            break
    return

def ending(song_duration):
    while True:
        start = time.time()
        timer = (start-offset_time)-offset_clock
        if timer > song_duration:
            print "Song Ended"
            break

pygame.mixer.init(48000, -16, 2, 4096)

pygame.mixer.music.load('/home/sigmadrian/Music/Segundo Tributo Dream Theater/07 Illumination Theory.mp3')
time.sleep(2)
pygame.mixer.music.play()
time.sleep(2)
offset_time = time.time()
print "Reproducing  Illumination Theory - Dream Theater..."
while pygame.mixer.music.get_busy() == True:

    # Illumination Theory - Mini Reflector + Laser + Smoke Machine
    configuration(0,0.000,"p0s7r0k0a$")
    configuration(0,6.437,"p7s0r1k0a$")
    configuration(0,8.607,"p7s0r1k1a$")
    configuration(1,22.259,"p0s7r0k0a$")
    configuration(1,35.795,"p0s3r0k0a$")
    configuration(2,16.361,"p2s0r0k0a$")
    configuration(2,29.866,"p0s2r0k0a$")
    configuration(2,43.396,"p1s0r0k0a$")
    configuration(2,56.920,"p0s1r0k0a$")
    configuration(3,3.688,"p8s0r0k0a$")
    configuration(3,10.444,"p0s7r0k0a$")
    configuration(3,23.447,"p7s0r1k0a$")
    configuration(3,57.434,"p9s3r0k0a$")
    configuration(4,29.442,"p3s0r0k0a$")
    configuration(4,42.444,"p2s0r0k0a$")
    configuration(4,58.427,"p0s1r0k0a$")
    configuration(5,30.440,"p5s0r0k0a$")
    configuration(5,46.453,"p6s0r0k0a$")
    configuration(6,0.443,"p6s0r1k0a$")
    configuration(6,2.000,"p0s3r0k0a$")
    configuration(6,2.454,"p0s7r0k0a$")
    configuration(6,54.256,"p0s0r1k0a$")
    configuration(7,4.000,"p0s0r0k0a$")
    configuration(7,15.388,"p9s0r0k0a$")
    configuration(8,40.185,"p2s0r0k0a$")
    configuration(10,18.801,"p2s0r1k0a$")

    # Quitar Laser
    configuration(11,0.181,"p0s4r0k0a$")
    configuration(12,6.256,"p4s0r0k0a$")
    configuration(12,21.533,"p2s0r0k0a$")
    configuration(12,27.345,"p7s0r1k0a$")
    configuration(12,31.722,"p2s0r0k0a$")
    configuration(12,34.616,"p0s2r0k0a$")
    configuration(12,52.382,"p1s0r0k0a$")
    configuration(12,56.446,"p0s4r0k0a$")
    configuration(13,07.914,"p1s0r1k0a$")
    configuration(13,30.986,"p0s3r0k0a$")
    configuration(13,54.257,"p1s0r0k0a$")
    configuration(13,58.217,"p1s0r1k0a$")
    configuration(14,0.302,"p0s1r0k0a$")
    configuration(14,2.200,"p2s0r0k0a$")
    configuration(14,35.903,"p5s0r0k0a$")
    configuration(15,2.857,"p0s5r0k0a$")
    configuration(15,18.549,"p7s0r1k1a$")
    configuration(15,33.016,"p0s7r0k0a$")
    configuration(15,53.221,"p7s0r0k0a$")
    configuration(16,51.762,"p0s7r0k0a$")
    configuration(16,55.160,"p7s0r1k0a$")
    configuration(17,22.245,"p0s7r0k0a$")
    configuration(17,29.999,"p7s0r1k0a$")
    configuration(18,24.184,"p9s7r0k0a$")
    configuration(18,39.750,"p7s0r1k0a$")
    configuration(18,52.954,"p7s0r1k1a$")
    configuration(19,15.121,"p0s0r0k0a$")
    configuration(19,18.438,"p9s0r0k0a$")
    configuration(21,34.370,"p0s0r0k0a$")

    ending(Illumination_Theory_duration)


print "SEGUNDA PARTE TERMINADA"