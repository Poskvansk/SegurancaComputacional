import hashlib
import numpy as np

def ctob64(c):
    if (c <= 25): return c + 65
    if (c <= 51): return c + 71
    if (c <= 61): return c - 4
    if (c == 62): return 43
    if (c == 63): return 47
    return 0
    
def b64toc(b):
    if (b >= 65 and b <= 90): return b - 65
    if (b >= 97 and b <= 122): return b - 71
    if (b >= 48 and b <= 57): return b + 4
    if (b == 43): return 62
    if (b == 47): return 63
    return 0


def base64_decode(msg):
    
        decoded = ""
    
        for i in range(0, len(msg), 4):
    
            slice = np.zeros(4, dtype=np.ubyte)
    
            aux = msg[i:i+4]

            for i in range(len(aux)):
                slice[i] = ord(aux[i])
                slice[i] = b64toc(slice[i])
    
            decode = np.zeros(3, dtype=np.ubyte)

            decode[0] = (slice[0] << 2) | (slice[1] >> 4)
            decode[1] = ((slice[1] & 0xf) << 4) | (slice[2] >> 2)
            decode[2] = ((slice[2] & 0x3) << 6) | slice[3]
    
            res = [chr(c) for c in decode]

            res = ''.join(res)
            decoded += res
    
        decoded = decoded.rstrip('\0')
        return decoded

def base64_encode(msg):

    encoded = ""

    for i in range(0, len(msg), 3):       

        slice = np.zeros(3, dtype=np.ubyte)

        aux = msg[i:i+3]

        for i in range(len(aux)):
            slice[i] = ord(aux[i])

        encode = np.zeros(4, dtype=np.ubyte)

        encode[0] = slice[0] >> 2
        encode[1] = ((slice[0] & 0x3) << 4) | (slice[1] >> 4)
        encode[2] = ((slice[1] & 0xf) << 2) | (slice[2] >> 6)
        encode[3] = slice[2] & 0x3f

        res = [chr(ctob64(c)) for c in encode]

        if(len(aux) == 2):
            res[3] = '='
        elif(len(aux) == 1):
            res[2] = '='
            res[3] = '='
        
        res = ''.join(res)
        encoded += res

    print(encoded)
    return encoded


def verify(msg, sig):

    m = hashlib.sha3_256()
    m.update(msg.encode())
    print(m.hexdigest())

    # return m.digest() == sig



if __name__ == '__main__':
    
    # msg = "This is a messag"
    # verify(msg, 0)
    pass
