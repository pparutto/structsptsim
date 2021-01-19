#!/usr/bin/python3

import sys
from os.path import abspath, join, isfile
from argparse import ArgumentParser

parser = ArgumentParser(description="extract polygon from svg file")
parser.add_argument("fname", help="input svg filename")

args = parser.parse_args()

out_fname = join(abspath(args.fname), args.fname.rstrip(".svg") + ".poly")

if isfile(out_fname):
    print("Error out file already exists: {}".format(out_fname))
    sys.exit(1)

with open(args.fname, 'r') as in_f, open(out_fname, 'w') as out_f:
    for line in in_f:
        line = line.lstrip(" ")
        if line.startswith("d="):
            out_f.write(line.lstrip("d=\"").rstrip("\"\n") + "\n")

print("Created: {}".format(out_fname))
