'''
# -*- coding: utf-8 -*-
'''
def to_hash(list_v):
    """
    BKDRHash
    """
    seed = 131
    hash_v = 0
    if isinstance('a', str):
        for itor in list_v:
            hash_v = hash_v*seed+ord(itor)    
    else:
        for itor in list_v:
            hash_v = hash_v*seed+itor
    return hash_v & 0x7FFFFFFF

def sim_hash(list_v):
    """
    BKDRHash
    """
    val = [0 for i in range(32)]
    tmp = 0
    for itor in list_v:
        for j in range(32):
            tmp = to_hash(itor[0])
            val[j] += itor[1] if tmp & (1 << j) else -itor[1]
    hash_v = 0
    for i in range(32):
        if val[i] > 0:
            hash_v |= (1 << i)
    return hash_v

def hanming_distance(hash1, hash2):
    """
    hanming_distance
    """
    dist = 0
    val = hash1 ^ hash2
    while val:
        val = val & (val - 1)
        dist = dist + 1
    return dist
