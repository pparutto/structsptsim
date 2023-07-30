#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 11 10:57:07 2023

@author: pierre
"""

from os import makedirs
from os.path import join, isdir
from argparse import ArgumentParser
import numpy as np


parser = ArgumentParser(description="Merge trajectories from multiple simulations")
parser.add_argument("dirPath", help="input directory")
parser.add_argument("N", type=int, help="number of splits")

args = parser.parse_args()

res = args.dirPath + "_merged"
if not isdir(res):
    makedirs(res)

cur = np.zeros((0, 4))
for i in range(1, args.N+1):
    dats = np.loadtxt(join(args.dirPath + "_" + str(i), "trajs.csv"), delimiter=",")
    if i > 1:
        last_tr = cur[cur[:,0] == np.max(cur[:,0]), 1]
        min_t = last_tr[0] + (cur[1,1] - cur[0,1])
        print(min_t, (cur[1,1] - cur[0,1]))
        dats[:,1] = dats[:,1] + min_t
        dats[:,0] = dats[:,0] + np.max(cur[:,0]) + 1
    cur = np.vstack([cur, dats])

with open(join(res, "trajs.csv"), 'w') as f:
    for i in range(cur.shape[0]):
        f.write("{:n},{},{},{}\n".format(*cur[i,:]))
#np.savetxt(join(res, "trajs.csv"), cur, delimiter=",")