import hashlib

def sha3(msg):
    m = hashlib.sha3_256()
    m.update(msg)
    # print(m.hexdigest())

    return m


def oaep():
    pass

if __name__ == "__main__":
    print(sha3(b"hello world"))