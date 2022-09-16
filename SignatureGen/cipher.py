from ast import Mod
from re import A
import numpy as np
import mod

#################################################
# GLOBAL VARIABLES

# key = 0xF0CAF0FAF0CAF0FAF0CAF0FAF0CAF0FA
# iv = 0x000102030405060708090a0b0c0d0e0f

key = [0xF0, 0xCA, 0xF0, 0xFA, 0xF0, 0xCA, 0xF0, 0xFA, 0xF0, 0xCA, 0xF0, 0xFA, 0xF0, 0xCA, 0xF0, 0xFA]
iv = [0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f]

key_bytes = bytearray(key)
key_matrix = np.matrix(key_bytes, dtype=np.ubyte).reshape(4, 4)

#################################################

def subbytes(state):
    pass

def shift_rows(state):

    for i in range(1, 4):
        state[i] = np.roll(state[i], i*-1)

    return state

def gf_reduce(a):

    msb_pos = bin(a).__len__() - 3
    
    while(msb_pos >= 8):
        a = a ^ (0x11B << (msb_pos - 8))
        msb_pos = bin(a).__len__() - 3

    return a

def gf_mult(a, b):

    res = 0

    for i in range(8):
        if(b & (1 << i)):
            res ^= (a << i)

    return res

def gf_mod(a, b):

    res = gf_mult(a, b)
    res = gf_reduce(res)

    return res


def mix_columns(state):

    state = np.matrix([
        [0x87, 0xf2, 0x4d, 0x97],
        [0x6e, 0x4c, 0x90, 0xec],
        [0x46, 0xe7, 0x4a, 0xc3],
        [0xa6, 0x8c, 0xd8, 0x95]
    ])

    mult_matrix = np.matrix([
        [2, 3, 1, 1],
        [1, 2, 3, 1],
        [1, 1, 2, 3],
        [3, 1, 1, 2]
    ], dtype=np.ubyte)



    aux = state[:, 0].T 
    column = []
    temp = []
    
    for i in range(4):
        column.append(aux[0, i])

    aux = mult_matrix[0]
    for i in range(4):
        temp.append(aux[0, i])

    print(column)
    print(temp)

    res = []

    for i in range(4):
        temp = []
        for j in range(4):
            s0 = 0
            for k in range(4):
                # print(hex(state[k, j]), " x ", hex(mult_matrix[j, k]))
                s0 ^= gf_mod(state[k, i], mult_matrix[j,k] )
            temp.append(s0)
            print(temp)
        res.append(temp)
    
    res = np.matrix(res, dtype=np.ubyte).T
    print(res)

    # print("-------------- MIX COLUMNS --------------")
    # print(res)

    # return state

def add_round_key(state):
    pass

def aes(nk = 4):

    if(nk == 4):
        nr = 10
    elif(nk == 6):
        nr = 12
    elif(nk == 8):
        nr = 14
    
    for i in range(nr):
        subbytes()
        shift_rows()
        mix_columns()
        add_round_key()

def main():

    msg = "This is a messag"

    while(len(msg) % 16 != 0):
        msg += "X"

    

    # Convert message to 4x4 matrix
    state = np.array([list(msg[i:i+4]) for i in range(0, len(msg), 4)])
    # print(state)

    # state = shift_rows(state)
    state = mix_columns(state)

main()
