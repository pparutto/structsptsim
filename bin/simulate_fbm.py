#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar 21 14:33:57 2025

@author: pierre
"""

import os
from stochastic.processes.noise import FractionalGaussianNoise
import numpy as np

seed = int(np.random.rand() * (2**32 - 1))
rng = np.random.default_rng(seed)

class TrajInitialiser:
    def __init__(self, box_dims):
        self.box_dims = box_dims

    def generate_init_point(self):
        return np.random.rand(2) * self.box_dims

class Traj:
    def __init__(self, H, D, dt, boundbox, t0, lmbd, init_traj):
        #self.max_n = rng.poisson(lmbd)
        #while self.max_n <= 1:
        #    self.max_n = rng.poisson(lmbd)
        self.bb = boundbox
        self.max_n = int(np.round(rng.exponential(lmbd)))
        while self.max_n <= 1:
            self.max_n = int(np.round(rng.exponential(lmbd)))
        fbn = FractionalGaussianNoise(H, 100+self.max_n)
        self.s2Ddt = np.sqrt(2 * D * dt)
        #fbn = FractionalGaussianNoise(H, 1)
        #self.sampls = []
        #for i in range(self.max_n):
        #    self.sampls.append([fbn.sample(1)[0], fbn.sample(1)[0]])
        #self.sampls = np.array(self.sampls)
        self.sampls = np.stack([fbn.sample(100+self.max_n, algorithm='daviesharte')[100:],
                                fbn.sample(100+self.max_n, algorithm='daviesharte')[100:]], axis=1)
        self.t0 = t0
        self._pts = np.zeros((self.max_n, 2))
        self._pts[0,:] = init_traj.generate_init_point()
        self._n = 1
        self._out = False

    def add_point(self):
        self._pts[self._n,:] = self._pts[self._n-1,:] + self.sampls[self._n,:] * s2Ddt
        if (self._pts[self._n,0] < self.bb[0] or self._pts[self._n,0] > self.bb[1] or
            self._pts[self._n,1] < self.bb[2] or self._pts[self._n,1] > self.bb[3]):
            self._out = True
        self._n += 1

    def done(self):
        return self._out or self._n >= self.max_n

    def get_pts(self):
        return self._pts[:self._n, :]


outdir = "/mnt/data2/SPT_method/simu/freespace_fbm2/density"


width = 420
height = 420
pxsize = 0.024195525
bb = [0, width * pxsize, 0, height * pxsize]
tlambda = 13
D = 1

Nrep = 3

Hs = [0.25, 0.5, 0.75]
DTs = [0.0001, 0.0006, 0.0025, 0.0056, 0.01, 0.0156, 0.0625, 0.146, 0.25, 0.3906, 0.5625, 0.7656, 1]
Nspots = [1, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50]
Nframes = [60000, 12000, 6000, 4000, 3000, 2400, 2000, 1715, 1500, 1334, 1200]


box_dims = np.array([width, height]) * pxsize
tr_init = TrajInitialiser(box_dims)

for H in Hs:
    for dt in DTs:
        s2Ddt = np.sqrt(2 * D * dt)
        for rep in range(1, Nrep + 1):
            for u, nspot in enumerate(Nspots):
                print("{}/{}/{}/{}".format(H, dt, rep, nspot))

                curdir = "{}/{}/{}_{}_{}_{}_{}".format(outdir, rep, H, D, nspot, dt, Nframes[u])
                if not os.path.isdir(curdir):
                    os.makedirs(curdir)

                if os.path.isfile("{}/trajs.csv".format(curdir)):
                    print(" Skipped")
                    continue

                all_trajs = []

                alive_trajs = []
                for i in range(len(alive_trajs), nspot):
                    alive_trajs.append(Traj(H, D, dt, bb, 0, tlambda, tr_init))

                new_trajs = []
                for n in range(1, Nframes[u]):
                    for i in range(len(alive_trajs)):
                        alive_trajs[i].add_point()

                    for i in range(len(alive_trajs), nspot):
                        alive_trajs.append(Traj(H, D, dt, bb, n * dt, tlambda, tr_init))

                    assert(len(alive_trajs) == nspot)

                    todel = []
                    for i, tr in enumerate(alive_trajs):
                        if tr.done():
                            todel.append(i)
                    for i in sorted(todel, reverse=True):
                        all_trajs.append(alive_trajs[i])
                        del alive_trajs[i]

                #ADD the trajectories still alive at the end of the last frame
                all_trajs.extend(alive_trajs)

                with open("{}/params.csv".format(curdir), 'w') as f:
                    f.write("seed, {}\n".format(seed))
                    f.write("width (px), {}\n".format(width))
                    f.write("height (px), {}\n".format(height))
                    f.write("length (frame), {}\n".format(Nframes[u]))
                    f.write("pixelSize (µm), {}\n".format(pxsize))
                    f.write("Exponential lambda (frame), {}\n".format(tlambda))
                    f.write("DT (s), {}\n".format(dt))
                    f.write("D (µm2/s), {}\n".format(D))
                    f.write("density (1/mu m2), {}\n".format(nspot / (width * pxsize * height * pxsize)))
                    f.write("Spots per frame, {}\n".format(nspot))
                    f.write("Hurst exponent H, {}\n".format(H))

                with open("{}/trajs.csv".format(curdir), 'w') as f:
                    for i, tr in enumerate(all_trajs):
                        cur_pts = tr.get_pts()
                        for j in range(cur_pts.shape[0]):
                            f.write("{:d},{:.6f},{:.5f},{:.5f}\n".format(i, tr.t0 + j * dt,
                                                            cur_pts[j,0] + 10, cur_pts[j,1] + 10))



print("DONE")