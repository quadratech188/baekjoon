def correct(c):
    if c == 'e': return 'i'
    if c == 'i': return 'e'
    if c == 'E': return 'I'
    if c == 'I': return 'E'
    return c

import sys

for line in sys.stdin:
    print(*list(map(correct, line)), sep='', end='')
