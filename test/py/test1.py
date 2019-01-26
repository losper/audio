import matplotlib.pyplot as plt
import numpy as np

Fs = 800.0  # sampling rate
Ts = 1.0/Fs # sampling interval
ff = 5   # frequency of the signal

t = np.arange(0,1,Ts) # time vector

#frequency ff=5
ty = np.sin(2*np.pi*ff*t)#+5*np.sin(2*np.pi*2*ff*t)+3*np.cos(2*np.pi*3*ff*t)
tys = ty[:200]*np.hanning(200)
y=np.hstack([tys,tys,tys])
#set frequency
n = len(y) # length of the signal
k = np.arange(n)
frq = k[range(n//2)] # one side frequency range

Y = np.fft.fft(y)/(n/2.0) # fft computing and normalization
# 为什么要做normalization?
Y = Y[range(n//2)]

fig, ax = plt.subplots(2, 1)
ax[0].plot(k,y)
ax[0].set_xlabel('Time')
ax[0].set_ylabel('Amplitude')
ax[1].plot(frq,abs(Y), 'r') # plotting the spectrum
ax[1].set_xlabel('Freq (Hz)')
ax[1].set_ylabel('|Y(freq)|')

plt.show()

# # 50Hz正弦波512点FFT采样
# t = np.arange(0, 1.0, 1.0/8000)
# x = np.sin(2*np.pi*50*t)[:512]*np.hanning(512)
# plt.plot(np.hstack([x,x,x]),linewidth=2)
# plt.xlabel('Sampling point')
# plt.ylabel('Amplitude[a.u.]')
# plt.ylim(-1.5,1.5)
# plt.show()

