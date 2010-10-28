#!/usr/bin/python

import os
import errno
import sys
import subprocess
import re
import unittest
import scribe

COMPILER_CMD = 'gcc -O2 -x c - -o'
DIST_DIR = sys.path[0]
BUILD_DIR = os.path.join(DIST_DIR, 'build')
TESTS_DIR = os.path.join(DIST_DIR, 'tests')

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
    def __init__(self, source, headers, show_dmesg=False):
        self.description = source
        self.files = headers + [source]
        self.source_file = source
        self.headers_file = headers
        self.executable = re.sub('\.[^.]*$', '', source) \
                            .replace(TESTS_DIR, BUILD_DIR)
        self.logfile = self.executable + '.log'
        self.show_dmesg = show_dmesg
        unittest.TestCase.__init__(self)

    def shortDescription(self):
        return self.description

    def setUp(self):
        # Check if the output is newer than the source
        try:
            src_mtime = max(os.stat(f).st_mtime for f in self.files)
            out_mtime = os.stat(self.executable).st_mtime
            if out_mtime > src_mtime:
                return
        except OSError:
            pass

        # If not, compiling the sources
        code = open(self.source_file, 'rb').read()

        code = b''.join(open(f, 'rb').read() for f in self.headers_file) + code

        out_dir = os.path.split(self.executable)[0]
        mkdir_p(out_dir)

        print("Compiling %s" % self.source_file)
        compiler = subprocess.Popen(COMPILER_CMD.split() + [self.executable],
                                    stdin=subprocess.PIPE)
        compiler.communicate(code)
        if compiler.wait():
            raise CompileError()

    def runTest(self):
        log = open(self.logfile, 'w+')

        ps = scribe.Popen(log, self.executable, record = True,
                          show_dmesg = self.show_dmesg,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (rcd_stdout, rcd_stderr) = ps.communicate()
        rcd_err = ps.wait()

        log.seek(0)

        ps = scribe.Popen(log, replay = True, backtrace_len = 100,
                          show_dmesg = self.show_dmesg,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (rpl_stdout, rpl_stderr) = ps.communicate()
        rpl_err = ps.wait()

        self.assertEqual(rcd_stdout, rcd_stdout)
        self.assertEqual(rcd_stderr, rcd_stderr)
        self.assertEqual(rcd_err, rcd_err)



def get_sources(path, headers=[]):
    """ returns a list of tuples (source, [headers])
    """
    files_and_dirs = set(os.path.join(path, f) for f in os.listdir(path))
    dirs = set(d for d in files_and_dirs if os.path.isdir(d))
    files = set(f for f in files_and_dirs - dirs if f.endswith(".c"))

    header_files = set(f for f in files
                       if os.path.split(f)[1].startswith("header"))
    source_files = files - header_files
    headers = set(headers) | header_files

    return [res for d in dirs for res in get_sources(d, headers)] + \
           [(f, list(headers)) for f in source_files]

if __name__ == '__main__':
    from optparse import OptionParser
    usage = 'usage: %prog [options] test_filter'
    parser = OptionParser(usage=usage)
    parser.add_option("-v", "--verbose", dest="verbosity",
                      action="count", default=0,
                      help="display test progress, pass twice for more verbosity")
    parser.add_option("-d", "--dmesg",
                      action="store_true", dest="dmesg", default=False,
                      help="Show a dmesg trace if the replay diverges")
    (options, args) = parser.parse_args()

    path_filter = args or ['']

    test_cases = (ScribeTestCase(source, headers, show_dmesg = options.dmesg)
                  for source, headers in get_sources(TESTS_DIR)
                  if True in (source.find(f) != -1 for f in path_filter))
    test_suite = unittest.TestSuite()
    test_suite.addTests(test_cases)
    test_runner = unittest.TextTestRunner(verbosity=options.verbosity)
    test_runner.run(test_suite)
