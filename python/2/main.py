import stat
import sys
import os

def main():
    path = sys.argv[1]
    try:
        if os.path.isfile(path):
            file = open(path, "r")
            print(file.read())
        elif os.path.isdir(path):
            files = os.listdir(path)
            print(files)
        permissions = os.stat(path)[stat.ST_MODE]
        printPerms(permissions)
    except FileNotFoundError:
        print("file at " + path + " does not exist")

def printPerms(perms):
    print("readable: " + str(bool(perms & stat.S_IRGRP)))
    print("writeable: " + str(bool(perms & stat.S_IWGRP)))

if __name__ == "__main__":
    main()


