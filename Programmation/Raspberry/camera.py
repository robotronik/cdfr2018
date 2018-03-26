#!/usr/bin/python3

import re
import sys
import os
import io
import socket
import struct
from os import listdir
from os.path import isfile, join
import time
import select
import subprocess
import numpy as np
import picamera

CV_SOCKET_PATH = './cv_socket'
CV_PROG_PATH = './RobotrOvision'

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

def run_cv():
    #Opening socket
    try:
        os.unlink(CV_SOCKET_PATH)
    except OSError:
        if os.path.exists(CV_SOCKET_PATH):
            raise
    cv_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    cv_socket.bind(CV_SOCKET_PATH)
    cv_socket.listen(1)

    #Running cv program
    subprocess.Popen(CV_PROG_PATH)

    #Waiting for connection
    connection = cv_socket.accept()[0]
    print("Connected.")

    #Sending img
    stream = io.BytesIO()
    #camera.capture(stream, format='jpeg')
    img = open("capture.jpg", "rb")
    stream.write(bytearray(img.read()))
    img.close()
    
    print(stream.tell())
    buff = []
    print("Sending image...")
    connection.send(bytearray(struct.pack('<L', stream.tell())))
    stream.seek(0)
    connection.send(bytearray(stream.read()))

    print("Waiting for OpenCV...")
    result = connection.recv(4).decode("utf-8")
    
    connection.close()
    cv_socket.close()

    return result;

def wait_request(t, socket):
    start = time.time()
    while(time.time() - start < t):
        try:
            string = socket.recv(1024).decode("utf-8")
            if string == str("read\0"):
                print("CV request received")
                result = run_cv()
                #time.sleep(1);
                socket.send(result.encode())
                print("Result sent")
        except:
            continue

check_args()
socket_cmd = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
socket_cmd.connect(sys.argv[2])
socket_cmd.settimeout(0.1)

#camera = picamera.PiCamera()
#start_recording(get_video_path(str(sys.argv[1])))
wait_request(90, socket_cmd)
#camera.stop_recording()
socket_cmd.close()
