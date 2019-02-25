import matplotlib.pyplot as plt
import numpy as np
import wavinfo as wv


def framing(signal, rate, frame_size, frame_stride):
    frame_length, frame_step = frame_size * rate, frame_stride * rate
    signal_length = len(signal)
    frame_length = int(round(frame_length))
    frame_step = int(round(frame_step))
    num_frames = int(
        np.ceil(float(np.abs(signal_length - frame_length)) /
                frame_step))  # Make sure that we have at least 1 frame
    pad_signal_length = num_frames * frame_step + frame_length
    z = np.zeros((pad_signal_length - signal_length))
    pad_signal = np.append(signal, z)

    indices = np.tile(np.arange(0, frame_length), (num_frames, 1)) + np.tile(
        np.arange(0, num_frames * frame_step, frame_step), (frame_length, 1)).T
    frames = pad_signal[indices.astype(np.int32, copy=False)]
    return frames


data, time, rate, nframes = wv.read_wav("../data/1.wav")
plt.plot(time, data)
plt.show()
frames = framing(data, rate, 0.025, 0.01)
plt.plot(frames[40])
plt.show()

frames *= np.hamming(len(frames[0]))
mag_frames = np.absolute(np.fft.rfft(frames, 512))
for n in np.arange(5):
    plt.plot(mag_frames[40 + n])
    plt.show()
