# -*- coding: utf-8 -*-
# 读Wave文件并且绘制波形
import matplotlib.pyplot as plt
import numpy as np

frame = np.arange(0, 10, 1)
for v in frame[1:]:
    if v > v[-1]:
        print(v)