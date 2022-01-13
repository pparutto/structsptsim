#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jan 12 19:20:13 2022

@author: pierre
"""

import numpy as np
import bezier
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

pts = np.array([[1, 1, 1], [1, 1.5, 2], [1.4, 2.2, 3.5], [2, 2.5, 3], [2.3, 4, 1]]).T

curve = bezier.Curve(pts, degree=4)
cpts = []
for t in np.arange(0.0, 1.0, 0.1):
    cpts.append(curve.evaluate(t))
cpts = np.array(cpts)
cpts = cpts.reshape((cpts.shape[0], cpts.shape[1]))

fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot(pts[0,:], pts[1,:], pts[2,:])
ax.plot(cpts[:,0], cpts[:,1], cpts[:,2])