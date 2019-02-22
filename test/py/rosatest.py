import librosa
# import librosa.display
import matplotlib.pyplot as plt
import wavinfo as wv
import numpy as np

# read wav samples
sec = 2
data, rate = wv.wav_read('../data/1.wav', 2)
data1, rate1 = wv.wav_read('back10.wav', 3)
data_use = data * 1.0 / (max(abs(data)))
time = np.arange(0, rate * 2) * (1.0 / rate)
data_use1 = data1 * 1.0 / (max(abs(data1)))
time1 = np.arange(0, rate * 3) * (1.0 / rate1)
data_fft = np.fft.rfft(data)
data1_fft = np.fft.rfft(data1, 512)
# plot a wavform
# print(sr)
# plt.figure()
# librosa.display.waveplot(y, sr)
# plt.title('Beat wavform')
# plt.show()
# plt.plot(signal.hanning(512))
# plt.show()
# data=librosa.feature.mfcc(y,sr)
# mel = librosa.feature.mfcc(data_use, rate, n_mfcc=40)
# print(mel.shape)
plt.subplot(211)
plt.plot(data_fft)
plt.subplot(212)
plt.plot(data)
plt.show()
