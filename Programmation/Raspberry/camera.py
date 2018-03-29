#!/usr/bin/python3

import re
import sys
import os
import io
import signal
import socket
import struct
from os import listdir
from os.path import isfile, join
import subprocess
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
    print("[camera.py] Connected to cv program")

    #Capturing img
    print("[camera.py] Capturing image")
    stream = io.BytesIO()
    CAMERA.capture(stream, format='jpeg')
    #img = open("capture.jpg", "rb")
    #stream.write(bytearray(img.read()))
    #img.close()

    #Sending img
    print("[camera.py] Sending image...")
    print(stream.tell())
    connection.send(bytearray(struct.pack('<L', stream.tell())))
    stream.seek(0)
    connection.send(bytearray(stream.read()))

    print("[camera.py] Waiting for OpenCV...")
    r_cv = connection.recv(4).decode("utf-8")
    connection.close()
    cv_socket.close()
    return r_cv

def sigterm_handler(_signo, _stack_frame):
    sys.exit(0)

check_args()
signal.signal(signal.SIGTERM, sigterm_handler)

SOCKET_CMD = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
SOCKET_CMD.connect(sys.argv[2])
SOCKET_CMD.settimeout(0.1)

CAMERA = picamera.PiCamera()
CAMERA.resolution = (1280, 720)
CAMERA.start_recording(get_video_path(str(sys.argv[1])))

try:
    while True:
        try:
            STRING = SOCKET_CMD.recv(1024).decode("utf-8")
            if STRING == str("read\0"):
                print("[camera.py] CV request received")
                RESULT = run_cv()
                SOCKET_CMD.send(RESULT.encode())
                print("[camera.py] Result" + RESULT + " sent")
        except:
            continue
finally:
    CAMERA.stop_recording()
    SOCKET_CMD.close()
    print("[camere.py] Clean stop.")
    
