#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.mlab import griddata
import sys

if (len(sys.argv) < 2):
    print "usage: " + sys.argv[0] + "framefile"
    print "framefile - file with temperature data"
    exit(0)

f = open(sys.argv[1], "r")

state = 0
nn = 0

for line in f:
    if (state == 0):
        state = 1
        i = 0
        nn = int(line.split(" ")[0])
        X = range(0, nn)
        Y = range(0, nn)
        Z = range(0, nn)
    elif (state == 1):
        tok = line.split(" ")
        # tok[0] # id
        X[i] = float(tok[1]) #x
        Y[i] = float(tok[2]) #y
        Z[i] = float(tok[3]) #T
        i = i + 1
        if (i == nn):
            state = 0

res = 100
xi = np.linspace(min(X), max(X), res)
yi = np.linspace(min(Y), max(Y), res)
zi = griddata(X,Y,Z,xi,yi,interp='nn')

plt.figure(1, figsize=(6,6))
#plt.scatter(X, Y, c=Z, cmap=plt.cm.hot)
plt.contourf(xi, yi, zi, cmap=plt.cm.hot)
plt.colorbar()
plt.savefig(".".join(sys.argv[1].split(".")[:-1]) +'.pdf', bbox_inches=0)
plt.savefig(".".join(sys.argv[1].split(".")[:-1]) +'.png', bbox_inches=0)

