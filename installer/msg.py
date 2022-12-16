from colorama import init, deinit, Fore, Back, Style
import sys

class message_controller:
    _totalstep = 0
    _step = 0
    silent = False
    _last_message = ''
    _indent = 0
    _at_bol = True

    def indent(self):
        self._indent += 1

        if not self._at_bol:
            print()
            self._at_bol = True

    def unindent(self):
        self._indent -= 1
        if (self._indent < 0):
            self._indent = 0
        
        if not self._at_bol:
            print()
            self._at_bol = True

    def print(self, m):
        if not self._at_bol:
            print()
        self._last_message = ("   " * self._indent) + m
        print(self._last_message, end=" ", flush=True)
        self._at_bol = False

    def begin(self, m):
        self.print(m)
        print()

    def result(self, r, m = '', quit = True):
        if (self.silent):
            if not r:
                sys.exit(self._step)
            return

        if m == '':
            if r:
                m = "OK"
            else:
                m = "Error"
        
        if r:
            self._print_result(m, Fore.GREEN)

        else:
            self._print_result(m, Fore.RED)
            if quit:
                sys.exit(self._step)

    def skip(self, m = 'Skipped'):
        if (self.silent):
            return
        # print(Fore.YELLOW + "[" + m + "]" + Style.RESET_ALL)
        self._print_result(m, Fore.YELLOW)

    def _print_result(self, m, color):
        spaces = (70 - len(self._last_message)) * '.'
        print(Fore.LIGHTBLACK_EX + spaces + color + "[" + m + "]" + Style.RESET_ALL)
        self._at_bol = True

    def print_with_result(self, m, r):
        self.print(m)
        self.result(r)