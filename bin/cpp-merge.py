import pathlib
import re

PRAGMA_ONCE_REGEX = re.compile('#pragma once')
INCLUDE_QUOTES_REGEX = re.compile('#include "(?P<path>.+)"')

visited_headers = set()
def parse_file(filepath: pathlib.Path):
    if filepath.resolve() in visited_headers: return
    visited_headers.add(filepath.resolve())

    with open(filepath, mode='r') as f:
        for line in f.readlines():
            if PRAGMA_ONCE_REGEX.match(line): continue

            match = INCLUDE_QUOTES_REGEX.match(line);
            if match is not None:
                parse_file(filepath.parent.joinpath(match.group('path')))
            else:
                print(line, end='')

import sys

parse_file(pathlib.Path(sys.argv[1]))
