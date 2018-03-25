#!/usr/bin/python3

import re
import sys
import os
import socket
from os import listdir
from os.path import isfile, join
import picamera

def check_args():
    if len(sys.argv) < 3:
        print("Usage : python3 camera.py <video-path> <socket-cmd-path>")
        sys.exit(0)

def get_video_path(directory):
    if not os.path.isdir(directory):
        os.makedirs(directory)

    videos = [entry for entry in listdir(directory) if isfile(join(directory, entry))]
    numbers = [int(re.findall(r"\d\d\d", video)[0])
               for video in videos
               if re.match(r"record\d\d\d.h264", video) != None]

    number = 0
    if numbers:
        number = max(numbers) + 1
    
    return join(directory, "record"+str(number).zfill(3)+".h264")

check_args()
VIDEO_DIR = str(sys.argv[1])
VIDEO_PATH = get_video_path(VIDEO_DIR)

print("Recording to "+VIDEO_PATH+"\n")

socket_cmd = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM);
print(sys.argv[2])
socket_cmd.connect(sys.argv[2])

#camera = picamera.PiCamera();
#camera.resolution = (1280, 720);

