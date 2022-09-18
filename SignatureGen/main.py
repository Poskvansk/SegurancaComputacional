from traceback import print_tb
import cipher
import keygen
import signature

def get_message(msg):
    pass



if __name__ == "__main__":

    p, q, key = keygen.keygen()
    # print(hex(key))

    print("-------------- RSA --------------")
    print("Qual mensagme deseja encriptar?")
    msg = input()
    print("---------------------------------\n")

    msg_sha = signature.sha3(msg.encode())



    # encrypt hash with private key
    # send message and encrypted hash
    # get message and encrypted hash
    # decrypt hash
    # hash message
    # compare hashes

    # encrypt with aes
    # decrypt with aes

    pass