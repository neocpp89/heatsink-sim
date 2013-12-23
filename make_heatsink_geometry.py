#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.mlab import griddata
import sys

if (len(sys.argv) < 5):
    print "usage: " + sys.argv[0] + " Nx Ny Lx Ly"
    print "Nx - Number of grid points in x direction."
    print "Nx - Number of grid points in y direction."
    print "Lx - Length in x direction."
    print "Ly - Length in y direction."
    exit(0)

Nx = int(sys.argv[1])
Ny = int(sys.argv[2])
Lx = int(sys.argv[3])
Ly = int(sys.argv[4])

x = np.linspace(0, Lx, Nx)
y = np.linspace(0, Ly, Ny)
k = np.ones((Nx, Ny))
k = 0.0271 * k;

for i in xrange(0, Nx):
    for j in xrange(0, Ny):
        if (x[i] <= 0.8 and x[i] >= 0.2 and y[j] <= 0.4 and y[j] >= 0.0):
#            k[i,j] = 205*1e-3
            k[i,j] = 0.1

print Nx*Ny
for j in xrange(0, Ny):
    for i in xrange(0, Nx):
        nid = i + j*Nx
        print nid, k[i,j]
