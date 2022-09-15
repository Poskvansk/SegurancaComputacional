import random

def miller_rabin(n, k=10):
    if n == 2 or n == 3:
        return True
    if n <= 1 or n % 2 == 0:
        return False
    d = n - 1
    r = 0
    while d % 2 == 0:
        d //= 2
        r += 1
    for i in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for i in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def keygen(n = 1024):

    n = 128
    p = 0; q = 0

    while not miller_rabin(p):
        p = random.randrange(2**(n-1) +1, 2**n)

    while not miller_rabin(q):
        q = random.randrange(2**(n-1) +1, 2**n)

    key = p * q

    # print(hex(p))
    # print("----------------------------------\n")
    # print(hex(q))

    return p, q, key

keygen()