#!/usr/bin/env python
import numpy as np
import sys

if (len(sys.argv) < 5):
    print "usage: " + sys.argv[0] + "Nx Ny Lx Ly"
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

print Nx*Ny
for j in xrange(0, Ny):
    for i in xrange(0, Nx):
        nid = i + j*Nx

        n0 = (i+1) + j*Nx
        n1 = i + (j+1)*Nx
        n2 = (i-1) + j*Nx
        n3 = i + (j-1)*Nx
        if (i == Nx-1):
            n0 = -1
        if (j == Ny-1):
            n1 = -1
        if (i == 0):
            n2 = -1
        if (j == 0):
            n3 = -1

        print nid, x[i], y[j], n0, n1, n2, n3

