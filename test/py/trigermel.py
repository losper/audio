'''
# -*- coding: utf-8 -*-
# ref:https://blog.csdn.net/qq_39516859/article/details/80815369
# ref:https://haythamfayek.com/2016/04/21/speech-processing-for-machine-learning.html
'''

import numpy as np
import matplotlib.pyplot as plt
NFFT = 512
rate = 8000
low_freq_mel = 0
high_freq_mel = (2595 * np.log10(1 + (rate / 2) / 700))  # Convert Hz to Mel
nfilt = 40
mel_points = np.linspace(low_freq_mel, high_freq_mel,
                         nfilt + 2)  # Equally spaced in Mel scale
hz_points = (700 * (10**(mel_points / 2595) - 1))  # Convert Mel to Hz
bin = np.floor((NFFT + 1) * hz_points / rate)
fbank = np.zeros((nfilt, int(np.floor(NFFT / 2 + 1))))
print(hz_points)
print(bin)
freq = []  # 采样频率值
df = rate / NFFT
for n in range(0, int(NFFT / 2 + 1)):
    freqs = int(n * df)
    freq.append(freqs)
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
print(fbank.shape)
# W2 = int(NFFT / 2 + 1)
# df = rate / NFFT
# freq = []
# for n in range(0, W2):
#     freqs = int(n * df)
#     freq.append(freqs)
# bank = np.zeros((nfilt, W2))
# print(bank.shape)
# for m in range(1, nfilt + 1):
#     f1 = hz_points[m - 1]
#     f2 = hz_points[m + 1]
#     f0 = hz_points[m]
#     n1 = np.floor(f1 / df)
#     n2 = np.floor(f2 / df)
#     n0 = np.floor(f0 / df)
#     for k in range(1, W2):
#         if m >= n1 and m <= n0:
#             bank[m - 1, k] = (k - n1) / (n0 - n1)
#         elif m > n0 and m <= n2:
#             bank[m - 1, k] = (n2 - k) / (n2 - n0)
#     plt.plot(freq, bank[m - 1, :])
#     plt.show()
