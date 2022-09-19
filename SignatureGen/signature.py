import hashlib

def sha3(msg):
    m = hashlib.sha3_256()
    m.update(msg)

    return m


def oaep():
    pass


def verify_signature(msg, sig, pbk):

    print("Verificando assinatura...")

    msg_sha = sha3(msg.encode())
    msg_sha = int.from_bytes(msg_sha.digest(), byteorder='big')

    sig = pow(sig, pbk.__int__(), pbk.modulus)
    
    if(msg_sha == sig):
        print("Assinatura válida!")
    else:
        print("Assinatura inválida!")


def sign(msg, pvk):

    print("Assinando mensagem...")

    msg_sha = sha3(msg.encode())

    if(msg_sha.digest_size > pvk.modulus.bit_length()):
        print("Message too long")
        return

    msg_sha = int.from_bytes(msg_sha.digest(), byteorder='big')

    encrypted_hash = pow(msg_sha, pvk.__int__(), pvk.modulus)
    
    return encrypted_hash