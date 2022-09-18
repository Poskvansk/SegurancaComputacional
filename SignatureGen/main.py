from traceback import print_tb
import cipher
import keygen
import signature
import verify

def get_message(msg):
    pass



if __name__ == "__main__":

    pbk, pvk = keygen.keygen()

    # print("Public key: ", pbk)
    # print("Private key: ", pvk)

    print("Qual mensagme deseja encriptar?")
    # msg = input()
    msg = "This is a message"
    print("Mensagem: ", msg)
    print("---------------------------------\n")

    cipher = cipher.aes_ctr(msg)
    print("Mensagem encriptada: ", cipher)
    
    str = ""
    for i in cipher:    
        aux = ''.join([chr(x) for x in i])
        str += aux
        
    print(str)

    b64 = verify.base64_encode(str)
    print(b64)

    # msg_sha = signature.sha3(msg.encode())
    # print(msg_sha.hexdigest())