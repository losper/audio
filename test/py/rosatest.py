import librosa
import librosa.display
import matplotlib.pyplot as plt
import wavinfo as wv
import numpy as np

# read wav samples
sec = 2
data, rate = wv.wav_read('../data/1.wav', sec)
data_use = data * 1.0 / (max(abs(data)))
time = np.arange(0, rate * sec) * (1.0 / rate)
# plot a wavform
# print(sr)
# plt.figure()
# librosa.display.waveplot(y, sr)
# plt.title('Beat wavform')
# plt.show()
# plt.plot(signal.hanning(512))
# plt.show()
# data=librosa.feature.mfcc(y,sr)
mel = librosa.feature.mfcc(data_use, rate, n_mfcc=40)
print(mel.shape)
plt.subplot(211)
plt.plot(time, data_use)
plt.subplot(212)
plt.plot(mel)
plt.show()
