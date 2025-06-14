import struct, os
import sys
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC 



def Derive_key(password : str,salt : bytes) -> bytes:
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(),
        length=32,
        salt=salt,
        iterations=480000
    )
    return kdf.derive(password.encode())


def Encrypt(file_to_encypt):
    password = input("Password: ")
    salt = os.urandom(16)
    key = Derive_key(password,salt)
    
    nonce = os.urandom(8)
    counter = 0 
    full_nonce = struct.pack("<Q", counter) + nonce
    algorithm = algorithms.ChaCha20(key, full_nonce)
    cipher = Cipher(algorithm, mode=None)
    encryptor = cipher.encryptor()
    
    f_cipher = open(file_to_encypt,"rb")
    plaintext = f_cipher.read()
    ct = encryptor.update(plaintext)

    str = file_to_encypt + ".txt.enc"
    f = open(str,"wb")
    f.write(salt)
    f.write(full_nonce)
    f.write(ct)


def Decrypt(file_to_decrypt):
    password = input("Password: ")

    f_cipher = open(file_to_decrypt,"rb") 
    data = f_cipher.read() 
    salt,nonce,ct = data[:16],data[16:32],data[32:]
    
    key = Derive_key(password,salt)
    algorithm = algorithms.ChaCha20(key,nonce)
    cipher = Cipher(algorithm, mode=None)
    decryptor = cipher.decryptor()
    decrypted = decryptor.update(ct)

    str = file_to_decrypt + ".txt.dec"
    f_decrypted = open(str,"wb")
    f_decrypted.write(decrypted)
   


    
if __name__== "__main__":
    operation = sys.argv[1]
    match operation:
        case "enc":
            Encrypt(sys.argv[2])
        case "dec":
            Decrypt(sys.argv[2])

