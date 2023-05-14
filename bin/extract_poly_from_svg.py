#!/usr/bin/python3

import sys
from os import listdir
from os.path import abspath, join, isfile
from argparse import ArgumentParser

parser = ArgumentParser(description="extract polygon from svg files")
parser.add_argument("dir", help="input directory")

args = parser.parse_args()

print("arg", args.dir)


for fname in [e for e in listdir(args.dir) if e.endswith(".svg")]:
    out_fname = join(args.dir, fname.rstrip(".svg")) + ".poly"

    if isfile(out_fname):
        print("Error out file already exists: {}".format(out_fname))
        sys.exit(1)

    with open(join(args.dir, fname), 'r') as in_f, open(out_fname, 'w') as out_f:
        for line in in_f:
            line = line.lstrip(" ")
            if line.startswith("d="):
                out_f.write(line.lstrip("d=\"").rstrip("\"\n") + "\n")

    print("Created: {}".format(out_fname))
