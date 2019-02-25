import matplotlib.pyplot as plt
import numpy as np
import wave


def save_wave_file(filename, data, framerate, channels, sampwidth):
    '''save the date to the wavfile'''
    wf = wave.open(filename, 'wb')
    wf.setnchannels(channels)  # 声道
    wf.setsampwidth(sampwidth)  # 采样字节 1 or 2
    wf.setframerate(framerate)  # 采样频率 8000 or 16000
    wf.writeframes(
        b"".join(data)
    )  # https://stackoverflow.com/questions/32071536/typeerror-sequence-item-0-expected-str-instance-bytes-found
    wf.close()


def main():
    SAMPLING_RATE = 44100
    t = np.arange(0, 2.0, 1.0 / SAMPLING_RATE)
    data = 4000 * np.sin(2 * 10 * 100 * np.pi * t)+2000 * np.sin(2 * 5 * 100 * np.pi * t)
    td = data.astype(np.int16)
    save_wave_file("test2.wav", td, SAMPLING_RATE, 1, 2)
    plt.plot(data)
    plt.show()


main()
