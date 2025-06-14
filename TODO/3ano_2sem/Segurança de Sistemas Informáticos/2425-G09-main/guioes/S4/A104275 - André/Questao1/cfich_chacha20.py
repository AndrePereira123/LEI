
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

def chacha20(argv):

    if(argv[1] == "setup"):
        key = os.urandom(32)
        fkey = argv[2]
        if os.path.exists(fkey):
            os.remove(fkey)
        file = open(f"{fkey}","wb")
        file.write(key) ## 32 bytes/256-bit key
    
    elif(argv[1] == "enc"):

        file_name = argv[2]
        fkey = open(f"{argv[3]}","rb")
        dados_key = fkey.read()
        key = dados_key[:32]

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
        file_encript.write(full_nonce + dados_encrypt)
    
    elif(argv[1] == "dec"):
        file_name = argv[2]
        fkey = open(f"{argv[3]}","rb")
        dados_key = fkey.read()

        key = dados_key[:32]

        file = open(f"{file_name}","rb")
        file_data = file.read()
        full_nonce = file_data[:16]

        algorithm = algorithms.ChaCha20(key, full_nonce)
        cipher = Cipher(algorithm, mode=None)

        decryptor = cipher.decryptor()
        dados_decrypt = decryptor.update(file_data[16:])

        file_decrypt = open(f"{file_name[:-4]}.dec","wb")
        file_decrypt.write(dados_decrypt)
    
    
def main():
    chacha20(sys.argv)

if __name__ == "__main__":
    main()