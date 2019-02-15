'''
# -*- coding: utf-8 -*-
'''
import matplotlib.pyplot as plt
import numpy as np
import scipy.signal as signal

sampling_rate = 8000
fft_size = 512
hann=signal.hann(512,sym=0)

t = np.arange(0, 1.0, 1.0/sampling_rate)
x = np.sin(2*np.pi*200.25*t)  + 2*np.sin(2*np.pi*300.375*t)
xs = x[:fft_size]
xf = np.fft.rfft(xs)/fft_size
freqs = np.linspace(0, sampling_rate/2, fft_size/2+1)
xfp = 20*np.log10(np.clip(np.abs(xf), 1e-20, 1e100))
xf1 = np.fft.rfft(xs*hann*2)/fft_size
xfp1 = 20*np.log10(np.clip(np.abs(xf1), 1e-20, 1e100))

plt.subplot(2,1,1)
plt.plot(freqs, xfp1)
plt.plot(freqs, xfp)
plt.show()