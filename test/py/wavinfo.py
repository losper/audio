import wave as wv
import numpy as np


def wav_read(path, sec):
    wavfile = wv.open(path, 'rb')
    params = wavfile.getparams()
    print(params)
    framesra = params[2]
    # print(params)
    datawav = wavfile.readframes(framesra * sec)
    wavfile.close()
    datause = np.fromstring(datawav, dtype=np.short)
    # datause = datause * 1.0 / (max(abs(datause)))

    # print(len(datause))
    # time = np.arange(0, framesra * sec) * (1.0 / framesra)
    return datause, framesra
