# -*- coding: utf-8 -*-
# 读Wave文件并且绘制波形
import matplotlib.pyplot as plt
import numpy as np
import struct

# 打开WAV音频
with open('E:/git/passoa/audio/test/py/test1.pcm','rb') as f:#valid0_0二进制文件名
    f.seek(0,2)
    n=f.tell()
    f.seek(0,0)
    buf=f.read(n)
    m=int(n/2)#4是根据你数据的类型和你之前定义的buf长度确定
    print(m,n)

wave_data = np.fromstring(buf, dtype=np.short)
print(wave_data)
# 赋值的归一化
wave_data = wave_data*1.0/(max(abs(wave_data)))

# 最后通过采样点数和取样频率计算出每个取样的时间
time=np.arange(0, m) * (1.0 / m)

out_data=np.fft.fft(wave_data)

plt.figure()
# 左声道波形
plt.subplot(2,1,1)
plt.plot(time, wave_data)
plt.xlabel("time (seconds)")
plt.ylabel("Amplitude")
plt.title("Left channel")
plt.grid()  # 标尺

plt.subplot(2,1,2)
plt.plot(time, out_data)
plt.xlabel("time (seconds)")
plt.ylabel("Amplitude")
plt.title("Left channel")
plt.grid()  # 标尺


plt.show()
