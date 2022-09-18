from hashlib import pbkdf2_hmac
import random
import math
import mod

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

def get_encryption_key(p, q):
    n = p * q
    phi = (p - 1) * (q - 1)

    e = 2
    while math.gcd(e, phi) != 1 and e < phi:
        e += 1

    return mod.Mod(e, n)
    

def get_decryption_key(e, p, q):

    n = p * q
    phi = (p - 1) * (q - 1)
    d = mod.Mod(e, phi).inverse

    return mod.Mod(d, n)

def keygen(n = 1024):

    p = 0
    q = 0
    # while not miller_rabin(p):
    #     p = random.randrange(2**( n-1) +1, 2**n)

    # while not miller_rabin(q):
    #     q = random.randrange(2**(n-1) +1, 2**n)

    #test values    
    p = 0xef806b6f570d78f334e1e76063e8edb8190f165c6ded0c896d9fc7b27771440591899afb66c5f4c795ac2490b5d5a7af6d288b1835769f71859b2acdd34753a5e1bbb2abd6022d3ed653fc3d410a7aa6e1247f211f719a35dcd32f0190ac1f3f255060ef3c0ca5cb9954aca6955b1a22b017be2c07b4ac7e3e0c826dee0e65dd
    q = 0xd714ecd8b4c973f324cc3cc13d2931aefba4d1b0f53e89390804ed4d2e0a675ad86b5c3f2af4b265423215d7d7e04a01795008671008b742ee9a6e9bee3e4653f79509c2c46d1cab5ca75bfc2fb2d023dfdc54eb0f23e41c953926d3a2d7a164a5b9db33bea1f4574049944c0445d22671c11a9aed3ffba6e8fb1541e2334b99

    public_key = get_encryption_key(p, q)
    private_key = get_decryption_key(public_key.__int__(), p, q)

    return public_key, private_key


if __name__ == "__main__":

    keygen()