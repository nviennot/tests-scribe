#!/usr/bin/python

import os
import errno
import sys
import subprocess
import re
import unittest

BUILD_DIR = 'build'
COMPILER_CMD = 'gcc -O2 -x c - -o'

def mkdir_p(path):
    """ Do 'mkdir -p path'
    """
    try:
        os.makedirs(path)
    except OSError as err:
        if err.errno != errno.EEXIST:
            raise

class CompileError(BaseException):
    pass

class ScribeTestCase(unittest.TestCase):
    def __init__(self, headers, source):
        self.description = source
        self.source_chunks = headers + [source]
        self.executable = os.path.join(BUILD_DIR,
                                       re.sub('\.[^.]*$', '', source))
        unittest.TestCase.__init__(self)


    def shortDescription(self):
        return self.description

    def setUp(self):
        # Check if the output is newer than the source
        try:
            src_mtime = max(os.stat(f).st_mtime for f in self.source_chunks)
            out_mtime = os.stat(self.executable).st_mtime
            if out_mtime > src_mtime:
                return
        except OSError:
            pass

        # If not, compiling the sources
        code = b''.join(open(f, 'rb').read() for f in self.source_chunks)
        out_dir = os.path.split(self.executable)[0]
        mkdir_p(out_dir)
        compiler = subprocess.Popen(COMPILER_CMD.split() + [self.executable],
                                    stdin=subprocess.PIPE)
        compiler.communicate(code)
        if compiler.wait():
            raise CompileError()

    def runTest(self):
        pass
