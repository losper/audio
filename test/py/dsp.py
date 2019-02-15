'''
# -*- coding: utf-8 -*-
'''
import math
def average(samples):
    '''
    get average for samples
    '''
    val = 0
    for itor in samples:
        val += itor
    return val/len(samples)
def variance(samples):
    '''
    get variance for samples
    '''
    val = 0
    ave = average(samples)
    for itor in samples:
        val += pow(itor-ave, 2)
    return val/len(samples)
def standard_deviation(samples):
    '''
    get standard deviation for samples
    '''
    return math.sqrt(variance(samples))