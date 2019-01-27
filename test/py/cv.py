'''
# -*- coding: utf-8 -*-
'''

import torch
import numpy as np
 
z=np.fft.fft([3,5,8])
print(z)
print(np.fft.ifft(z))
# x=torch.tensor([5.0,3.0,4.0],requires_grad=True)
# y=torch.tensor([3.0,1.0,1.0],requires_grad=True)

# z=torch.log(y)
# z1=x+y

# # print(y)
# v = torch.tensor([1, 1, 1], dtype=torch.float)
# # z.backward(v)
# # z.backward(v,retain_graph=True)
# print(x.grad)
# print(y.grad)
# z1.backward(v)
# print(x.grad)
# print(y.grad)
# # print(x1.grad)