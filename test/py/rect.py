# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as pl
import matplotlib
import math
import random


def tcos(N,A,ph,fr):
    return [A*math.cos(2*math.pi*fr*t/N+ph) for t in range(N)]

v=np.linspace(0,1,500)
y=np.zeros(500)
# for i in range(5):
#     y+=np.cos(5*2*np.pi*i*v+np.pi)
y=np.sin(v*4*math.pi)+np.sin(2*v*4*math.pi)
x=np.linspace(-1,1,500)
pl.plot(x,y)
pl.title('5hz sin')
pl.show()
