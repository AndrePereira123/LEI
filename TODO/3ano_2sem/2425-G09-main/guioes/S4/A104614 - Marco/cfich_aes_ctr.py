import struct, os
import sys
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes


def Setup(name):
    key = os.urandom(32)
    f = open(name,"wb")
    f.write(key)



def Encrypt(file_to_encypt,file_with_key):
    f_key = open(file_with_key,"rb")
    key = f_key.read()
    nonce = os.urandom(8)

    counter = 0
    full_nonce = struct.pack("<Q", counter) + nonce
    algorithm = algorithms.AES(key)
    cipher = Cipher(algorithm, modes.CTR(full_nonce))
    encryptor = cipher.encryptor()
    
    f_cipher = open(file_to_encypt,"rb")
    cipher = f_cipher.read()
    ct = encryptor.update(cipher) + encryptor.finalize()

    str = file_to_encypt + ".txt.enc"
    f = open(str,"wb")
    f.write(full_nonce)
    f.write(ct)


def Decrypt(file_to_decrypt,file_with_key):
    f_key = open(file_with_key,"rb")
    data =  f_key.read()
    key = data[:32]

    f_cipher = open(file_to_decrypt,"rb") 
    data = f_cipher.read() 
    nonce,ct = data[:16],data[16:]
    
    algorithm = algorithms.AES(key)
    cipher = Cipher(algorithm, modes.CTR(nonce))
    decryptor = cipher.decryptor()
    decrypted = decryptor.update(ct) + decryptor.finalize()

    str = file_to_decrypt + ".txt.dec"
    f_decrypted = open(str,"wb")
    f_decrypted.write(decrypted)
   


    
if __name__== "__main__":
    operation = sys.argv[1]
    match operation:
        case "setup":
            Setup(sys.argv[2])
        case "enc":
            Encrypt(sys.argv[2],sys.argv[3])
        case "dec":
            Decrypt(sys.argv[2],sys.argv[3])

