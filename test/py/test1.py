'''
# -*- coding: utf-8 -*-
# ref:https://haythamfayek.com/2016/04/21/speech-processing-for-machine-learning.html
# ref:https://blog.csdn.net/xl928471061/article/details/72896293
'''
# import sys
# import wave

import matplotlib.pyplot as plt
from scipy.fftpack import dct
# import np as np

import wavinfo as wv
import numpy as np
# read wav samples
sec = 3
data, rate = wv.wav_read('../data/OSR_us_000_0010_8k.wav', sec)
time = np.arange(0, rate * sec) * (1.0 / rate)
# pre_emphasis
pre_emphasis = 0.97
emphasized_signal = np.append(data[0], data[1:] - pre_emphasis * data[:-1])
plt.subplot(2, 1, 1)
plt.plot(time, data)
plt.subplot(2, 1, 2)
plt.plot(time, emphasized_signal)
plt.show()

# framing
frame_size = 0.025
frame_stride = 0.01
frame_length, frame_step = frame_size * rate, frame_stride * rate
signal_length = len(emphasized_signal)
frame_length = int(round(frame_length))
frame_step = int(round(frame_step))

num_frames = int(
    np.ceil(float(np.abs(signal_length - frame_length)) /
            frame_step))  # Make sure that we have at least 1 frame
pad_signal_length = num_frames * frame_step + frame_length
z = np.zeros((pad_signal_length - signal_length))
pad_signal = np.append(emphasized_signal, z)

indices = np.tile(np.arange(0, frame_length), (num_frames, 1)) + np.tile(
    np.arange(0, num_frames * frame_step, frame_step), (frame_length, 1)).T
frames = pad_signal[indices.astype(np.int32, copy=False)]

# window hamming
plt.plot(np.arange(0, frame_length), np.hamming(frame_length))
plt.show()
frames *= np.hamming(frame_length)

# Fourier-Transform and Power Spectrum
NFFT = 512
mag_frames = np.absolute(np.fft.rfft(frames, NFFT))  # Magnitude of the FFT
pow_frames = ((1.0 / NFFT) * ((mag_frames)**2))  # Power Spectrum

plt.plot(pow_frames[0])
plt.show()

# Filter Banks
nfilt = 40
low_freq_mel = 0
high_freq_mel = (2595 * np.log10(1 + (rate / 2) / 700))  # Convert Hz to Mel
mel_points = np.linspace(low_freq_mel, high_freq_mel,
                         nfilt + 2)  # Equally spaced in Mel scale
hz_points = (700 * (10**(mel_points / 2595) - 1))  # Convert Mel to Hz
bin = np.floor((NFFT + 1) * hz_points / rate)

freq = []  # 采样频率值
df = rate / NFFT
for n in range(0, int(NFFT / 2 + 1)):
    freqs = int(n * df)
    freq.append(freqs)

fbank = np.zeros((nfilt, int(np.floor(NFFT / 2 + 1))))
for m in range(1, nfilt + 1):
    f_m_minus = int(bin[m - 1])  # left
    f_m = int(bin[m])  # center
    f_m_plus = int(bin[m + 1])  # right
    for k in range(f_m_minus, f_m):
        fbank[m - 1, k] = (k - bin[m - 1]) / (bin[m] - bin[m - 1])
    for k in range(f_m, f_m_plus):
        fbank[m - 1, k] = (bin[m + 1] - k) / (bin[m + 1] - bin[m])
    plt.plot(freq, fbank[m - 1])
plt.show()

print("pow_frames:", pow_frames.shape, "fbank:", fbank.shape)
filter_banks = np.dot(pow_frames, fbank.T)
# filter_banks 's data can not be 0
filter_banks = np.where(filter_banks == 0,
                        np.finfo(float).eps,
                        filter_banks)  # Numerical Stability
filter_banks = 20 * np.log10(filter_banks)  # dB
print("filter_banks:", filter_banks.shape)
num_ceps = 12
mfcc = dct(
    filter_banks, type=2, axis=1,
    norm='ortho')[:, 1:(num_ceps + 1)]  # Keep 2-13
print("mfcc:", mfcc.shape)
