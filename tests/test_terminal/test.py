from contextlib import contextmanager
import os
import subprocess as sp
import unittest


PROMPT = "#"
TERM_CMD = "\\quit"
TEST_DB_FILE = "test.db"


def run_program(program):
    return sp.Popen(program + f" {TEST_DB_FILE}", shell=True, stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.PIPE)

def readall(proc):
    out = ""
    while (rout := proc.stdout.readline().decode('utf-8')):
        out += rout
    return out

def readline(proc):
    return proc.stdout.readline().decode('utf-8').strip().replace("# ", "")

def writeline(proc, cmd):
    proc.stdin.write(f'{cmd.strip()}\n'.encode('utf-8'))
    proc.stdin.flush()

def terminate(proc):
    proc.stdin.close()
    proc.stderr.close()
    proc.stdout.close()
    proc.terminate()
    proc.wait(timeout=1)


class MultiSessionTerminalTestCase(unittest.TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        os.remove("test.db")

    @contextmanager
    def db_connection_open(self, *args, **kwargs):
        self.proc = sp.Popen(f"./main {TEST_DB_FILE}", shell=True, stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.PIPE)
        try:
            yield self.proc
        finally:
            self.proc.stdin.close()
            self.proc.stderr.close()
            self.proc.stdout.close()
            self.proc.terminate()
            self.proc.wait(timeout=1)

    def send_cmd(self, cmd):
        writeline(self.proc, cmd)

    def send_cmds(self, cmds):
        for cmd in cmds:
            self.send_cmd(cmd)

    def assert_outputs(self, outs):
        for out in outs:
            self.assertEqual(out, readline(self.proc))


class SingleSessionTerminalTestCase(unittest.TestCase):

    def setUp(self):
        self.proc = run_program("./main")

    def tearDown(self):
        os.remove("test.db")
        terminate(self.proc)

    def send_cmd(self, cmd):
        writeline(self.proc, cmd)

    def send_cmds(self, cmds):
        for cmd in cmds:
            self.send_cmd(cmd)

    def assert_outputs(self, outs):
        for out in outs:
            self.assertEqual(out, readline(self.proc))


class SingleSessionBehavioralTestCase(SingleSessionTerminalTestCase):

    def test_unrecognized_output(self):
        cmds = ["gibberish", TERM_CMD]
        self.send_cmds(cmds)

        expected = ["Unrecognized keyword: gibberish"]
        self.assert_outputs(expected)

    def test_insert_select(self):
        cmds = ["insert 0 abc", "insert 1 xyz", "insert 2 pqd", "select", TERM_CMD]
        self.send_cmds(cmds)

        expected = ["0, abc", "1, xyz", "2, pqd"]
        self.assert_outputs(expected)


class MultiSessionBehavioralTestCase(MultiSessionTerminalTestCase):

    def test_insert_persistence(self):
        cmds_0 = ["insert 123 abc", "insert 987 qwe", TERM_CMD]
        cmds_1 = ["select", "insert 567 ijk", TERM_CMD]
        cmds_2 = ["select", TERM_CMD]
        expected_0 = ["#"]
        expected_1 = ["123, abc", "987, qwe"]
        expected_2 = ["123, abc", "987, qwe", "567, ijk"]

        with self.db_connection_open():
            self.send_cmds(cmds_0)
            self.assert_outputs(expected_0)

        with self.db_connection_open():
            self.send_cmds(cmds_1)
            self.assert_outputs(expected_1)

        with self.db_connection_open():
            self.send_cmds(cmds_2)
            self.assert_outputs(expected_2)