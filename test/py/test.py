# -*- coding: utf-8 -*-
# 读Wave文件并且绘制波形
import matplotlib.pyplot as plt
import numpy as np

SAMPLING_RATE = 44100
t = np.arange(0, 2.0, 1.0 / SAMPLING_RATE)
data = 1000 * np.sin(2 * 10 * np.pi * t)

X = [[150, 2], [3, 4], [5, 6]]
plt.imshow(X)
plt.show()
