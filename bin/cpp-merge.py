import argparse
import pathlib
import re

parser = argparse.ArgumentParser(
        description="Merge CPP files"
        )

parser.add_argument('filename')
parser.add_argument('-I', '--include')

args = parser.parse_args();

include_paths = list(map(lambda p: pathlib.Path(p), args.include.split(';')))

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
                path = match.group('path')

                candidates = [filepath.parent / path] + list(map(lambda p: p / path, include_paths))

                for candidate in candidates:
                    if candidate.exists():
                        parse_file(candidate)
                        break
                else:
                    raise FileNotFoundError(f'Failed to find included file {path} @ {filepath}.\nCandidates: {candidates}')
            else:
                print(line, end='')

import sys

parse_file(pathlib.Path(args.filename))
