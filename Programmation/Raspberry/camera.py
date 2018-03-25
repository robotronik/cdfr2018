#!/usr/bin/python3

import re
import sys
import os
import socket
from os import listdir
from os.path import isfile, join
import time
import select
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

def start_recording(camera, path):
    print("Recording to "+path+"\n")
    #camera.resolution = (1280, 720)
    #camera.start_recording(path)

def connect_socket(path):
    _socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    _socket.connect(path)
    return _socket

def wait_request(t, socket):
    start = time.time()
    while(time.time() - start < t):
        try:
            string = socket.recv(1024).decode("utf-8")
            if string == str("read\0"):
                time.sleep(5);
                socket.send(b"rgb\0")
        except:
            continue

check_args()
socket_cmd = connect_socket(sys.argv[2])
socket_cmd.settimeout(0.1)

#camera = picamera.PiCamera()
#start_recording(get_video_path(str(sys.argv[1])))
wait_request(90, socket_cmd)
#camera.stop_recording()
socket_cmd.close()
