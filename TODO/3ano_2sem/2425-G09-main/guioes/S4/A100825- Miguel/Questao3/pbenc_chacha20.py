
import sys
import os
## o tipo de operação a realizar: setup, enc ou dec
## setup <fkey> cria ficheiro contendo uma chave apropriada para a cifra Chacha20 (com nome <fkey>)
## enc <fich> <fkey> cifra ficheiro passado como argumento <fich>, usando a chave lida do ficheiro <fkey>. O criptograma resultante deverá ser gravado <fich>.enc (i.e. adiciona a extensão .enc ao nome do ficheiro de texto-limpo).
## dec <fich> <fkey> decifra criptograma contido em <fich>, usando a chave lida do ficheiro <fkey>. Armazena o texto-limpo recuperado num ficheiro com nome <fich>.dec.

## https://github.com/uminho-lei-ssi/2425-SSI/blob/main/guioes/S4.md

## ficheiro_base para testar o processo
## chave criada no setup chave
## enc é o ficheiro incriptado
## dec é o fichiero desincriptado deve ser igual ao ficheiro_base inicial

import struct, os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC

def chacha20(argv):

      
    if(argv[1] == "enc"):
        salt = os.urandom(16)  
        file_name = argv[2]

        pass_phrase = argv[3].encode("utf-8")
        kdf = PBKDF2HMAC(
            algorithm=hashes.SHA256(),
            length=32,
            salt=salt,
            iterations=480000,
        )
        key = kdf.derive(pass_phrase)

        nonce = os.urandom(8)
        counter = 0
        full_nonce = struct.pack("<Q", counter) + nonce
        algorithm = algorithms.ChaCha20(key, full_nonce)
        cipher = Cipher(algorithm, mode=None)

        file = open(f"{file_name}","rb")
        file_data = file.read()

        encryptor = cipher.encryptor()
        dados_encrypt = encryptor.update(file_data)

        file_encript = open(f"{file_name}.enc","wb")
        file_encript.write(salt + full_nonce + dados_encrypt)
    
    elif(argv[1] == "dec"):
        file_name = argv[2]

        file = open(f"{file_name}","rb")
        file_data = file.read()
        salt = file_data[:16]
        full_nonce = file_data[16:32]

        pass_phrase = argv[3].encode("utf-8")
        kdf = PBKDF2HMAC(
            algorithm=hashes.SHA256(),
            length=32,
            salt=salt,
            iterations=480000,
        )
        key = kdf.derive(pass_phrase)

        algorithm = algorithms.ChaCha20(key, full_nonce)
        cipher = Cipher(algorithm, mode=None)

        decryptor = cipher.decryptor()
        dados_decrypt = decryptor.update(file_data[32:])

        file_decrypt = open(f"{file_name[:-4]}.dec","wb")
        file_decrypt.write(dados_decrypt)
    
    
def main():
    chacha20(sys.argv)

if __name__ == "__main__":
    main()