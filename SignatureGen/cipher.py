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

    aux = np.matrix([[2, 3, 1, 1],
                     [1, 2, 3, 1],
                     [1, 1, 2, 3],
                     [3, 1, 1, 2]], dtype=np.ubyte)

    column = state[:, [0]]
    column = [ord(x[0]) for x in column]

    aux = gf_mod(0x03, 0x6E)
    print(hex(aux))

    # column = np.matrix(column, dtype=np.ubyte).T

    # column = [0x87, 0x6e, 0x46, 0xA6]

    # test = (0x02 * column[0]) ^( 0x03 * column[1]) ^ (0x01 * column[2]) ^ (0x01 * column[3])
    # print("test = ", hex(test))
    # for i in range(4):
    #     print(hex(i))

    # for i in range(4):
        # s = [0, 0, 0, 0]
        # for j in range(4):
        #     s[j] = aux[i, j] * column[j]

        # test = s[0]
        # for k in range(1, 4):
        #     test ^= s[k]
        # print(test)

    # print(aux)
    # print(column)

    # res = np.dot(aux, column)
   
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
