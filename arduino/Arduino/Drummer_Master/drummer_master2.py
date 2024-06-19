#!/usr/bin/env python

import pygame
import time
import serial
from playsound import playsound

offset_clock = 0.20 #con wait for sleep
''' Duracion Wait for Sleep = 153 segundos
    Dureacion Another Day = 266 segundos
'''
The_Best_of_Times_duration = 790
As_I_Am_duration = 469
The_Glass_Prison_duration = 833
The_Dance_of_Eternity_duration = 376
Take_the_Time_duration = 502

ser = serial.Serial('/dev/ttyACM0', 115200)

time.sleep(5)

ser.write("p0b0s0r0$")
print "p0b0s0r0$" 

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

def ending(song_duration_in_seconds):
    while True:
        start = time.time()
        timer = (start-offset_time)-offset_clock
        if timer > song_duration_in_seconds:
            print "Song Ended"
            break

pygame.mixer.init(44100, -16, 2, 4096)
'''
ser.write("p0b0s0r0a$")
print "p0b0s0r0a$" 
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/The Best of Times.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing  The Best of Times - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,18.47,"p9b0s0r0a$")
    configuration(2,45.27,"p0b0s0r0a$")
    configuration(2,55.16,"p7b0s0r0a$")
    configuration(3,31.01,"p7b0s0r2a$")
    configuration(3,54.33,"p9b0s7r0a$")
    configuration(4,38.19,"p7b0s0r2a$")
    configuration(5,1.45,"p9b0s7r0a$")
    configuration(6,11.91,"p0b0s0r1a$")
    configuration(7,4.87,"p0b0s0r0a$")
    configuration(7,17.76,"p9b0s7r0a$")
    configuration(8,24.24,"p0b0s0r0a$")
    configuration(8,27.33,"p9b0s4r0a$")
    configuration(9,59.15,"p7b0s0r1a$")
    ending(The_Best_of_Times_duration)

time.sleep(3)'''

ser.write("p0b0s0r0a$")
print "p0b0s0r0a$" 
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/As I Am.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing  As I Am - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,16.45,"p9b0s0r0a$")
    configuration(0,36.16,"p2b0s3r0a$")
    configuration(1,12.59,"p0b0s3r2a$")
    configuration(1,14.90,"p2b0s3r2a$")
    configuration(1,44.42,"p3b0s0r0a$")
    configuration(2,13.93,"p9b0s3r0a$")
    configuration(2,28.73,"p9b0s3r2a$")
    configuration(2,13.93,"p9b0s0r0a$")
    configuration(2,44.43,"p2b0s0r0a$")
    configuration(3,12.63,"p0b0s3r2a$")
    configuration(3,27.34,"p2b0s3r1a$")
    configuration(3,56.42,"p9b0s0r0a$")
    configuration(4,11.19,"p9b0s3r2a$")
    configuration(4,55.07,"p0b0s3r2a$")
    configuration(5,24.15,"p2b0s0r0a$")
    configuration(6,8.44,"p0b0s3r2a$")
    configuration(6,21.29,"p3b0s0r1a$")
    configuration(6,49.57,"p0b0s3r2a$")
    configuration(7,19.08,"p9b0s0r0a$")
    configuration(7,47.86,"p0b0s0r0a$")
    ending(As_I_Am_duration)

time.sleep(3)
'''
ser.write("p0b0s0r0a$")
print "p0b0s0r0a$"
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/The Glass Prison.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing  The Glass Prison - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,31.59,"p9b0s0r2a$")
    configuration(1,25.21,"p2b0s0r0a$")
    configuration(1,44.85,"p0b0s0r0a$")
    configuration(1,49.45,"p0b11s0r0a$")
    configuration(1,53.90,"p0b0s3r2a$")
    configuration(2,32.19,"p0b11s0r0a$")
    configuration(2,51.89,"p9b0s3r2a$")
    configuration(3,4.61,"p2b0s0r2a$")		# Aqui entra la voz
    configuration(3,38.54,"p0b11s0r0a$")
    configuration(3,56.86,"p2b0s0r0a$")
    configuration(4,37.14,"p0b0s3r2a$")
    configuration(5,24.46,"p9b0s0r0a$")
    configuration(5,28.77,"p0b0s3r2a$")
    configuration(5,33.17,"p0b8s0r0a$")
    configuration(5,45.89,"p0b16s0r0a$")
    configuration(5,53.69,"p0b0s0r0a$")
    configuration(6,1.21,"p0b0s3r2a$")
    configuration(6,16.35,"p1b0s0r0a$")
    configuration(6,31.26,"p0b0s1r2a$")
    configuration(6,46.15,"p1b0s0r2a$")
    configuration(7,1.07,"p0b0s0r1a$")
    configuration(7,16.38,"p1b0s0r2a$")
    configuration(7,49.86,"p0b0s1r2a$")
    configuration(8,4.74,"p1b0s0r0a$")
    configuration(8,19.65,"p0b0s1r2a$")
    configuration(8,34.55,"p1b0s0r0a$")
    configuration(8,51.27,"p0b0s1r2a$")
    configuration(9,06.62,"p1b0s0r2a$")
    configuration(9,17.76,"p0b9s0r2a$")
    configuration(9,25.21,"p1b0s0r0a$")
    configuration(9,28.95,"p0b0s1r2a$")
    configuration(9,36.40,"p1b0s0r0a$")
    configuration(9,38.23,"p0b0s1r2a$")
    configuration(9,45.26,"p0b0s1r2a$")
    configuration(9,58.66,"p0b0s4r2a$")
    configuration(10,11.98,"p0b0s2r2a$")
    configuration(10,25.30,"p0b0s6r2a$")
    configuration(10,38.73,"p0b0s5r2a$")
    configuration(10,51.96,"p0b0s7r2a$")
    configuration(11,5.31,"p0b16s0r0a$")
    configuration(11,11.88,"p0b0s1r2a$")
    configuration(11,21.75,"p0b16s0r0a$")
    configuration(11,25.29,"p0b0s7r2a$")
    configuration(12,50.80,"p9b0s4r0a$")
    configuration(13,46.25,"p9b0s4r2a$")
    configuration(13,50.00,"p0b0s0r0a$")
    ending(The_Glass_Prison_duration)

time.sleep(3)

ser.write("p0b0s0r0a$")
print "p0b0s0r0a$"
pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/The Dance of Eternity.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing  The Dance of Eternity - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,1.90,"p9b0s0r0a$")
    configuration(0,4.81,"p9b8s0r0a$")
    configuration(0,14.97,"p9b8s0r2a$")
    configuration(0,30.50,"p0b0s4r2a$")
    configuration(0,33.00,"p0b0s4r1a$")
    configuration(0,46.09,"p9b0s4r0a$")
    configuration(1,18.93,"p0b0s0r1a$")
    configuration(1,21.37,"p0b0s4r2a$")
    configuration(1,31.82,"p2b0s0r0a$")
    configuration(1,38.99,"p2b0s3r2a$")
    configuration(1,46.41,"p0b0s1r2a$")
    configuration(2,7.72,"p1b0s3r1a$")
    configuration(2,18.47,"p0b0s6r2a$")
    configuration(2,34.19,"p3b0s0r0a$")
    configuration(2,41.51,"p0b1s0r0a$")
    configuration(2,48.52,"p0b0s0r0a$")
    configuration(2,50.60,"p0b0s0r2a$")
    configuration(3,08.85,"p0b0s1r2a$")
    configuration(3,37.54,"p0b12s1r2a$")
    configuration(3,52.13,"p0b16s7r2a$")
    configuration(4,23.04,"p0b0s0r1a$")
    configuration(4,24.80,"p9b0s3r2a$")
    configuration(4,44.97,"p2b0s0r0a$")
    configuration(5,3.33,"p0b0s2r2a$")
    configuration(5,15.26,"p0b6s0r0a$")
    configuration(5,45.70,"p0b0s0r2a$")
    configuration(5,57.88,"p0b0s1r1a$")
    configuration(6,12.85,"p0b0s3r2a$")
    ending(The_Dance_of_Eternity_duration)
'''
ser.write("p0b0s0r0a$")
print "p0b0s0r0a$"

time.sleep(3)

pygame.mixer.music.load('/home/sigmadrian/Music/Tributo Dream Theater/Take the Time.mp3')
pygame.mixer.music.play()
offset_time = time.time()
print "Reproducing  Take the Time - Dream Theater..."
while pygame.mixer.music.get_busy() == True:
    configuration(0,10.89,"p0b0s0r2a$")
    configuration(0,33.11,"p6b0s0r2a$")
    configuration(0,45.12,"p1b0s0r2a$")
    configuration(0,57.31,"p0b0s1r2a$")
    configuration(1,8.73,"p6b0s0r0a$")
    configuration(1,31.44,"p0b14s0r2a$")
    configuration(1,39.95,"p0b0s6r2a$")
    configuration(1,42.81,"p9b0s1r2a$")
    configuration(2,5.94,"p0b0s6r2a$")
    configuration(2,9.89,"p7b0s1r2a$")
    configuration(2,34.04,"p0b0s4r2a$")
    configuration(2,46.77,"p3b0s1r2a$")
    configuration(2,59.77,"p0b0s6r1a$")
    configuration(3,12.66,"p1b0s4r2a$")
    configuration(3,25.01,"p9b0s6r0a$")
    configuration(3,47.05,"p0b0s2r2a$")
    configuration(4,3.29,"p7b0s0r1a$")
    configuration(4,18.98,"p7b0s6r2a$")
    configuration(4,34.17,"p0b15s0r2a$")
    configuration(4,49.31,"p9b0s4r2a$")
    configuration(5,4.21,"p0b0s0r0a$")
    configuration(5,4.82,"p9b0s4r2a$")
    configuration(5,9.77,"p0b16s0r0a$")
    configuration(5,10.95,"p9b0s4r2a$")
    configuration(5,11.67,"p0b0s4r1a$")
    configuration(5,19.49,"p4b0s2r1a$")
    configuration(5,35.00,"p2b0s1r1a$")
    configuration(5,42.77,"p5b0s2r1a$")
    configuration(5,50.68,"p9b15s0r2a$")
    configuration(6,3.19,"p4b0s1r2a$")
    configuration(6,21.39,"p0b0s4r2a$")
    configuration(6,27.68,"p7b0s0r1a$")
    configuration(6,36.84,"p9b0s0r2a$")
    configuration(7,13.46,"p2b0s4r2a$")
    configuration(7,13.46,"p7b0s1r2a$")
    ending(Take_the_Time_duration)

print "SEGUNDA PARTE TERMINADA"