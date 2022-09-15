def subbytes(state):
    pass

def shift_rows(state):
    pass

def mix_columns(state):
    pass

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
