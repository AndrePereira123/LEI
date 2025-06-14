import struct, os
import sys
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding


def Setup(name):
    key = os.urandom(32)
    f = open(name,"wb")
    f.write(key)



def Encrypt(file_to_encypt,file_with_key):
    

    f_key = open(file_with_key,"rb")
    key = f_key.read()

    f_cipher = open(file_to_encypt,"rb")
    plaintext = f_cipher.read()


    iv = os.urandom(16)
    padder = padding.PKCS7(128).padder()
    padded_data = padder.update(plaintext) + padder.finalize()

    algorithm = algorithms.AES(key)
    cipher = Cipher(algorithm, modes.CBC(iv))
    encryptor = cipher.encryptor()
    ct = encryptor.update(padded_data) + encryptor.finalize()


    str = file_to_encypt + ".txt.enc"
    f = open(str,"wb")
    f.write(iv)
    f.write(ct)


def Decrypt(file_to_decrypt,file_with_key):
    f_key = open(file_with_key,"rb")
    data =  f_key.read()
    key = data[:32]

    f_cipher = open(file_to_decrypt,"rb") 
    data = f_cipher.read() 
    iv,ct = data[:16],data[16:]
    
    algorithm = algorithms.AES(key)
    cipher = Cipher(algorithm, modes.CBC(iv))
    decryptor = cipher.decryptor()
    decrypted = decryptor.update(ct) + decryptor.finalize()

    unpadder = padding.PKCS7(128).unpadder()
    plaintext = unpadder.update(decrypted) + unpadder.finalize()
    str = file_to_decrypt + ".txt.dec"
    f_decrypted = open(str,"wb")
    f_decrypted.write(plaintext)
   


    
if __name__== "__main__":
    operation = sys.argv[1]
    match operation:
        case "setup":
            Setup(sys.argv[2])
        case "enc":
            Encrypt(sys.argv[2],sys.argv[3])
        case "dec":
            Decrypt(sys.argv[2],sys.argv[3])

