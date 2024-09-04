#!/usr/bin/python3

import sys
from os import listdir
from os.path import abspath, join, isfile
from argparse import ArgumentParser

parser = ArgumentParser(description="extract polygon from svg files")
parser.add_argument("dir", help="input directory")

args = parser.parse_args()

print("arg", args.dir)


def read_transform(s):
    #t = translate, s = scale
    #tx ty sx sy
    res = [0.0, 0.0, 1.0, 1.0]
    ss = s.split('"')
    ss = ss[1].split(" ")
    for s in ss:
        ss2 = s.split("(")
        trans = ss2[0]
        ss3 = ss2[1].split(",")
        vals = [float(ss3[0]), float(ss3[1].split(")")[0])]
        if trans == "translate":
            res[0:2] = vals
        elif trans == "scale":
            res[2:4] = vals
    return res

for fname in [e for e in listdir(args.dir) if e.endswith(".svg")]:
    out_fname = join(args.dir, fname.rstrip(".svg")) + ".poly"

    if isfile(out_fname):
        print("Error out file already exists: {}".format(out_fname))
        sys.exit(1)

    with open(join(args.dir, fname), 'r') as in_f, open(out_fname, 'w') as out_f:
        for line in in_f:
            line = line.lstrip(" ")
            if "transform" in line:
                transf = read_transform(line)
                out_f.write("TRANSFORM {} {} {} {}\n".format(*transf))
            if line.startswith("d="):
                line = line.split('"')[1]
                res = ""
                coma = True
                tmp = ""
                for e in line.split(" "):
                    if e[0].isalpha():
                        res += e[0] + " "
                        e = e[1:]
                    if e[-1].isalpha():
                        tmp = e[-1]
                        e = e[:-1]

                    if coma:
                        res += e + ","
                        coma = False
                    else:
                        res += e + " "
                        coma = True

                    if tmp:
                        res += tmp + " "
                        tmp = ""


                out_f.write(res.rstrip(" ") + "\n")

    print("Created: {}".format(out_fname))
