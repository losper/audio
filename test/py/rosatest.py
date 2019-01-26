import librosa
import librosa.display
import matplotlib.pyplot as plt
import scipy.signal as signal
import numpy as np

# Load a wav file
y, sr = librosa.load('../data/1.wav', sr=None)
# plot a wavform
# print(sr)
# plt.figure()
# librosa.display.waveplot(y, sr)
# plt.title('Beat wavform')
# plt.show()
# plt.plot(signal.hanning(512))
# plt.show()
# data=librosa.feature.mfcc(y,sr)
plt.plot(librosa.feature.mfcc(y,sr))
plt.show()
