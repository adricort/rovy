#!/usr/bin/env python

import pygame
import time
import serial
from playsound import playsound

offset_clock = 0.20
Wait_for_Sleep_duration = 153
Another_Day_duration = 266
Pull_Me_Under_duration = 495
Six_oClock_duration = 333
Caugh_in_a_Web_duration = 331
The_Mirror_Lie_duration = 800

ser = serial.Serial('/dev/ttyACM0', 115200)

time.sleep(5)

# Instant in seconds, code as string
def configuration(minute, seconds, code):
    while True:
        instant = minute*60+seconds
        start = time.time()
        timer = (start-offset_time)-offset_clock
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

pygame.mixer.init(44100, -16, 2, 4096)

#ser.write("p0b0s0r0a$")
#print "p0b0s0r0a$" 
'''
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/Wait for Sleep.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing  Wait for Sleep - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,0.1,"p9b0s0r0a$")
    configuration(2,25.00,"p0b0s0r0a$")
    ending(Wait_for_Sleep_duration)

time.sleep(5)

pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/Another Day.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing  Another Day - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,14.72,"p6b0s0r0a$")
    configuration(0,26.48,"p0b0s0r0a$")
    configuration(0,57.40,"p9b0s6r0a$")
    configuration(1,11.11,"p6b0s0r0a$")
    configuration(1,52.11,"p0b14s0r0a$")
    configuration(2,2.35,"p9b0s6r2a$")
    configuration(3,0.43,"p6b0s0r0a$")
    configuration(3,27.75,"p0b6s0r2a$")
    configuration(3,34.57,"p0b14s0r2a$")
    configuration(3,39.62,"p0b0s0r1a$")
    configuration(3,44.80,"p9b0s0r0a$")
    configuration(4,12.33,"p0b0s1r2a$")
    ending(Another_Day_duration)
time.sleep(4)

ser.write("p0b0s1r2a$")
print "p0b0s1r2a$" 
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/Pull Me Under.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing  Pull Me Under - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,57.68,"p1b0s0r2a$")
    configuration(1,16.40,"p9b0s1r2a$")
    configuration(2,1.10,"p6b0s0r2a$")
    configuration(2,19.60,"p0b1s0r2a$")
    configuration(2,38.58,"p0b0s6r2a$")
    configuration(2,57.37,"p0b0s0r1a$")
    configuration(2,59.71,"p0b0s1r2a$")
    configuration(3,34.79,"p1b0s0r0a$")
    configuration(4,13.41,"p9b0s1r0a$")
    configuration(4,32.19,"p0b1s0r2a$")
    configuration(4,50.80,"p0b9s0r2a$")
    configuration(5,0.23,"p6b0s0r0a$")
    configuration(5,10.63,"p1b0s0r2a$")
    configuration(5,29.46,"p9b0s1r2a$")
    configuration(6,7.10,"p0b0s0r1a$")
    configuration(6,25.94,"p0b0s1r2a$")
    configuration(6,35.28,"p6b0s0r0a$")
    configuration(6,53.95,"p0b1s0r0a$")
    configuration(7,12.65,"p0b0s1r2a$")
    configuration(8,12.36,"p0b0s0r0a$")
    ending(Pull_Me_Under_duration)

time.sleep(4)
'''
ser.write("p0b0s3r0a$")
print "p0b0s3r0a$"
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/Six oClock.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing 6:00 - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,1.82+1.43,"p0b0s0r2a$")
    configuration(0,21.09+1.43,"p9b0s0r0a$")
    configuration(0,30.72+1.43,"p3b0s0r0a$")
    configuration(0,45.16+1.43,"p0b0s3r2a$")
    configuration(0,54.90+1.43,"p3b0s0r0a$")
    configuration(1,33.26+1.43,"p0b0s3r2a$")
    configuration(1,43.16+1.43,"p3b0s0r0a$")
    configuration(2,0.63+1.43,"p0b0s3r0a$")
    configuration(2,2.38+1.43,"p3b0s0r0a$")
    configuration(2,12.03+1.43,"p0b0s0r1a$")
    configuration(2,26.43+1.43,"p9b11s0r2a$")
    configuration(2,36.05+1.43,"p4b0s0r2a$")
    configuration(2,55.26+1.43,"p9b0s3r2a$")
    configuration(3,14.50+1.43,"p9b0s4r2a$")
    configuration(3,33.74+1.43,"p0b3s4r2a$")
    configuration(3,52.92+1.43,"p0b11s0r2a$")
    configuration(4,12.16+1.43,"p3b0s0r1a$")
    configuration(5,9.83+1.43,"p0b0s3r2a$")
    ending(Six_oClock_duration)

time.sleep(4)
'''
ser.write("p0b0s3r2a$")
print "p0b0s3r2a$"
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/Caught in a Web.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing Caught in a Web - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(1,12.82+1.93,"p3b0s0r0a$")
    configuration(1,31.24+1.93,"p0b11s0r0a$")
    configuration(1,40.26+1.93,"p3b0s0r0a$")
    configuration(1,58.11+1.93,"p3b0s0r2a$")
    configuration(2,15.97+1.93,"p0b0s0r1a$")
    configuration(2,34.39+1.93,"p0b0s3r2a$")
    configuration(3,33.06+1.93,"p9b0s3r0a$")
    configuration(4,13.27+1.93,"p0b0s3r2a$")
    configuration(4,38.98+1.93,"p3b0s0r0a$")
    configuration(5,15.30+1.93,"p0b0s3r2a$")
    ending(Caugh_in_a_Web_duration)


time.sleep(4)

ser.write("p0b0s1r2a$")
print "p0b0s1r2a$"
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/The Mirror_Lie.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing The Mirror/Lie - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,5.67,"p2b0s0r0a$")
    configuration(6,45.53,"p0b0s3r2a$")		# Inicio Lie
    configuration(9,22.29,"p2b0s0r2a$")
    configuration(10,7.82,"p0b0s3r2a$")
    configuration(10,27.41,"p0b0s3r2a$")
    configuration(10,47.30,"p2b0s0r2a$")
    configuration(11,7.55,"p0b0s3r2a$")
    configuration(11,59.53,"p9b0s0r0a$")
    configuration(12,21.14,"p0b0s3r2a$")
    ending(The_Mirror_Lie_duration)

print "PRIMERA PARTE TERMINADA"'''