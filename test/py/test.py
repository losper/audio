# -*- coding: utf-8 -*-
# 读Wave文件并且绘制波形
import matplotlib.pyplot as plt
import numpy as np
SAMPLING_RATE = 8000
ga = np.random.gamma(1.0, 1.0, 8000)
t = np.arange(0, 1.0, 1.0 / SAMPLING_RATE)
ffts = np.sin(2 * np.pi * t) + ga
data = np.append(ffts, np.sin(2 * np.pi * 50 * t) + ga)
# sf = np.fft.rfft(ffts, 512)
plt.subplot(211)
plt.plot(data)
plt.subplot(212)
plt.specgram(data, 512, 8000)
plt.show()
