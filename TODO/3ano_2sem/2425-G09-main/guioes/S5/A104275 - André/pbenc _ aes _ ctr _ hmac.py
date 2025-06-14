
import struct, os, sys
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives import hashes, hmac


def h(argv):

    if(argv[1] == "enc"):
        salt = os.urandom(16)  
        file_name = argv[2]

        pass_phrase = argv[3].encode("utf-8")
        kdf = PBKDF2HMAC(
            algorithm=hashes.SHA256(),
            length=64,
            salt=salt,
            iterations=480000,
        )
        keys = kdf.derive(pass_phrase)
        key = keys[:32]
        key_2 = keys[32:]

        nonce = os.urandom(8)
        counter = 0
        full_nonce = struct.pack("<Q", counter) + nonce
        algorithm = algorithms.AES(key)
        cipher = Cipher(algorithm, mode=modes.CTR(full_nonce))

        file = open(f"{file_name}","rb")
        file_data = file.read()

        encryptor = cipher.encryptor()
        dados_encrypt = encryptor.update(file_data)

        h = hmac.HMAC(key_2, hashes.SHA256())
        h.update(dados_encrypt)
        signature = h.finalize()

        file_encript = open(f"{file_name}.enc","wb")
        file_encript.write(salt + full_nonce + signature + dados_encrypt)
    
    elif(argv[1] == "dec"):
        file_name = argv[2]

        file = open(f"{file_name}","rb")
        file_data = file.read()
        salt = file_data[:16]
        full_nonce = file_data[16:32]
        signature = file_data[32:64]

        pass_phrase = argv[3].encode("utf-8")
        kdf = PBKDF2HMAC(
            algorithm=hashes.SHA256(),
            length=64,
            salt=salt,
            iterations=480000,
        )
        keys = kdf.derive(pass_phrase)
        key = keys[:32]
        key_2 = keys[32:]

        algorithm = algorithms.AES(key)
        cipher = Cipher(algorithm, mode=modes.CTR(full_nonce))

        decryptor = cipher.decryptor()

        h = hmac.HMAC(key_2, hashes.SHA256())
        h.update(file_data[64:])
        h.verify(signature)

        dados_decrypt = decryptor.update(file_data[64:])

        file_decrypt = open(f"{file_name[:-4]}.dec","wb")
        file_decrypt.write(dados_decrypt)
    
    
def main():
    h(sys.argv)

if __name__ == "__main__":
    main()