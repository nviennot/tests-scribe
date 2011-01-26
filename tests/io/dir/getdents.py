#!/usr/bin/python

import os
import sys
import signal
import tempfile

tmp_dir = tempfile.mkdtemp('scribe-test-getdents')

def child():
    for i in range(50):
        paths = []
        for j in range(50):
            (f, p) = tempfile.mkstemp(dir=tmp_dir)
            os.close(f)
            paths.append(p)
        for p in paths:
            os.unlink(p)

def parent():
    def sigchld(signum, stack):
        os.rmdir(tmp_dir)
        sys.exit()
    signal.signal(signal.SIGCHLD, sigchld)

    while True:
        print(os.listdir(tmp_dir))

def main():
    if os.fork():
        parent()
    else:
        child()

if __name__ == '__main__':
    main()
