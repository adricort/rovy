#!/usr/bin/env python

import pygame
import time
import serial
from playsound import playsound

offset_clock = 1.8
Mix_First_Part_duration = 1928.242

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
        timer = (start-offset_time)+offset_clock
        if timer > song_duration:
            print "Song Ended"
            break

pygame.mixer.init(48000, -16, 2, 4096)

pygame.mixer.music.load('/home/sigmadrian/Music/Segundo Tributo Dream Theater/Mix First Part.mp3')
time.sleep(2)
pygame.mixer.music.play()
time.sleep(2)
offset_time = time.time()
print "Reproducing  Mix First Part - Dream Theater..."
while pygame.mixer.music.get_busy() == True:

    # False Awakening Suite - Poner Mini Reflector
    configuration(0,8.709,"p0s6r0k0a$")

    # The Looking Glass
    configuration(2,44.689,"p0s4r0k0a$")
    configuration(3,15.975,"p4s0r0k0a$")
    configuration(3,49.488,"p5s0r0k0a$")
    configuration(4,3.405,"p7s0r1k0a$")
    configuration(4,19.050,"p0s4r0k0a$")
    configuration(4,34.708,"p3s0r0k0a$")
    configuration(5,1.900,"p0s5r0k0a$")
    configuration(5,22.096,"p5s0r0k0a$")
    configuration(5,35.979,"p7s0r1k0a$")
    configuration(5,51.682,"p9s6r0k0a$")
    configuration(6,26.446,"p4s0r0k0a$")
    configuration(7,21.842,"p7s0r1k0a0")
    configuration(7,33.000,"p0s0r0k0a$")

    # Paralyzed - Quitar mini reflector
    configuration(7,36.450,"p0s7r0k0a$")
    configuration(8,11.657,"p7s0r1k0a$")
    configuration(8,23.372,"p0s7r0k0a$")
    configuration(8,29.207,"p7s0r1k0a$")
    configuration(8,35.067,"p0s7r0k0a$")
    configuration(8,40.922,"p7s0r0k0a$")
    configuration(8,46.793,"p7s0r1k0a$")
    configuration(9,10.198,"p2s0r0k0a$")
    configuration(9,16.040,"p7s0r1k0a$")
    configuration(9,21.911,"p2s0r0k0a$")
    configuration(9,27.769,"p7s0r0k0a$")
    configuration(9,33.613,"p7s0r1k0a$")
    configuration(9,51.165,"p0s7r0k0a$")
    configuration(10,2.851,"p9s7r0k0a$")
    configuration(10,17.506,"p6s0r0k0a$")
    configuration(10,41.022,"p7s0r0k0a$")
    configuration(11,21.900,"p0s7r0k0a$")

    # Panic Attack
    configuration(11,52.000,"p0s1r0k0a$")
    configuration(12,19.160,"p1s0r1k0a$")
    configuration(12,23.912,"p0s1r0k0a$")
    configuration(12,38.631,"p1s0r0k0a$")
    configuration(13,15.884,"p7s0r1k0a$")
    configuration(13,31.085,"p0s1r0k0a$")
    configuration(13,57.779,"p0s5r0k0a$")
    configuration(15,06.818,"p5s0r0k0a$")
    configuration(15,14.399,"p0s1r0k0a$")
    configuration(16,11.555,"p1s0r0k0a$")
    configuration(16,19.136,"p0s1r0k0a$")
    configuration(17,31.550,"p1s0r0k0a$")
    configuration(17,46.779,"p0s1r0k0a$")
    configuration(18,32.490,"p1s0r0k0a$")
    configuration(18,40.118,"p0s1r0k0a$")

    # Build Me Up, Break Me Down - Poner Laser
    configuration(19,07.709,"p9s0r0k1a$")
    configuration(19,27.427,"p0s6r0k0a$")
    configuration(20,17.922,"p6s0r0k0a$")
    configuration(20,38.367,"p0s6r0k0a$")
    configuration(21,54.342,"p6s0r0k0a$")
    configuration(22,35.169,"p0s6r0k0a$")
    configuration(23,26.852,"p9s6r0k0a$")
    configuration(23,47.296,"p6s0r0k0a$")
    configuration(24,25.577,"p0s6r0k0a$")
    configuration(25,0.092,"p9s0r0k0a$")
    configuration(26,1.398,"p0s0r0k0a$")

    # Enigma Machine - Quitar Laser y Poner Mini Reflector
    configuration(26,06.756,"p9s0r0k0a$")
    configuration(26,28.108,"p0s2r0k0a$")
    configuration(26,47.948,"p0s0r1k0a$")
    configuration(26,49.947,"p0s2r0k0a$")
    configuration(26,52.637,"p2s0r0k0a$")
    configuration(27,17.132,"p0s2r0k0a$")
    configuration(27,26.455,"p2s0r1k0a$")
    configuration(27,45.937,"p0s2r0k0a$")
    configuration(28,07.957,"p2s0r0k0a$")
    configuration(28,11.942,"p0s0r1k0a$")
    configuration(28,15.620,"p2s0r0k0a$")
    configuration(28,19.614,"p0s0r1k0a$")
    configuration(28,23.949,"p2s0r0k0a$")
    configuration(28,27.949,"p0s0r1k0a$")
    configuration(28,31.621,"p2s0r0k0a$")
    configuration(28,35.624,"p0s2r0k0a$")
    configuration(29,43.935,"p9s2r0k0a$")
    configuration(30,29.298,"p4s0r0k0a$")
    configuration(30,33.949,"p2s0r0k0a$")
    configuration(30,50.624,"p5s0r0k0a$")
    configuration(30,58.464,"p0s2r0k0a$")
    configuration(31,11.781,"p2s0r1k0a$")
    configuration(31,32.616,"p0s2r0k0a$")
    configuration(31,51.952,"p2s0r1k0a$")
    configuration(31,53.080,"p9s0r0k0a$")
    configuration(32,04.291,"p8s0r0k0a$")

    ending(Mix_First_Part_duration)


print "PRIMERA PARTE TERMINADA"