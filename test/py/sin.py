'''
# -*- coding: utf-8 -*-
'''
import matplotlib.pyplot as plt
import numpy as np
import scipy.signal as signal

SAMPLING_RATE = 8000
FFT_SIZE = 512
HANN = signal.windows.hann(512, sym=0)

t = np.arange(0, 1.0, 1.0 / SAMPLING_RATE)
x = np.sin(2 * np.pi * 200.25 * t) + 2 * np.sin(2 * np.pi * 300.375 * t)
xs = x[:FFT_SIZE]
xf = np.fft.rfft(xs) / FFT_SIZE
freqs = np.linspace(0, SAMPLING_RATE / 2, FFT_SIZE / 2 + 1)
xfp = 20 * np.log10(np.clip(np.abs(xf), 1e-20, 1e100))
xf1 = np.fft.rfft(xs * HANN * 2) / FFT_SIZE
xfp1 = 20 * np.log10(np.clip(np.abs(xf1), 1e-20, 1e100))

plt.subplot(2, 1, 1)
plt.plot(freqs, xfp1)
plt.plot(freqs, xfp)
plt.show()
