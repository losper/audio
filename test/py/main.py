import wave
import pyaudio
import threading

framerate = 0
num_samples = 0
time = 0


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


def record(sec, framerate, num_samples):
    pa = pyaudio.PyAudio()
    stream = pa.open(
        format=pyaudio.paInt16,
        channels=1,
        rate=framerate,
        input=True,
        frames_per_buffer=num_samples)
    my_buf = []
    count = 0
    while count < sec * framerate:  # 控制录音时间
        string_audio_data = stream.read(num_samples)  # 一次性录音采样字节大小
        my_buf.append(string_audio_data)
        count += num_samples
        print('.')
    save_wave_file('back1.wav', my_buf, framerate, 1, 2)
    stream.close()


def play(chunk=4096):
    wf = wave.open("../data/1.wav", 'rb')
    p = pyaudio.PyAudio()
    stream = p.open(
        format=p.get_format_from_width(wf.getsampwidth()),
        channels=wf.getnchannels(),
        rate=wf.getframerate(),
        output=True)

    while True:
        data = wf.readframes(chunk)
        if len(data) == 0:
            break
        stream.write(data)
    stream.close()
    p.terminate()


if __name__ == "__main__":
    record(3, 22050, 4096)
    # t = threading.Thread(target=play, args=(4096,))
    # t1 = threading.Thread(target=record, args=(3, 22050, 4096))
    # t.start()
    # t1.start()
    # t.join()
    # t1.join()
