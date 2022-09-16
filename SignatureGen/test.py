from email.mime import base


def base64(data):

    data = data ^ 0x00

    return data

# print(base64("Olá, mundo!"))


