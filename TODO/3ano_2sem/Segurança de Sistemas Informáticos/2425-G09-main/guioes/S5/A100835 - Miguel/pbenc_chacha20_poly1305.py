import os, sys
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.ciphers.aead import ChaCha20Poly1305
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC


def derive_key(password: str, salt: bytes) -> bytes:
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(),
        length=32,
        salt=salt,
        iterations=480000,
    )
    return kdf.derive(password.encode())


def encode(fich):
    password = input("Introduza a password:")
    salt = os.urandom(16)
    enc_key = derive_key(password, salt)

    nonce = os.urandom(12)

    with open(fich, "rb") as f:
        plaintext = f.read()

    cipher = ChaCha20Poly1305(enc_key)
    ct = cipher.encrypt(nonce, plaintext, associated_data=None)

    with open(fich + ".enc", "wb") as f:
        f.write(salt)
        f.write(nonce)
        f.write(ct)


def decode(fich):
    password = input("Introduza a password:")
    
    with open(fich, 'rb') as f:
        data = f.read()
    salt, nonce, ct = data[:16], data[16:28], data[28:]
    enc_key = derive_key(password, salt)

    cipher = ChaCha20Poly1305(enc_key)
    plaintext = cipher.decrypt(nonce, ct, associated_data=None)

    with open(fich + ".dec", "wb") as f:
        f.write(plaintext)



if __name__ == "__main__":
    operation = sys.argv[1]
    match operation:
        case "enc":
            encode(sys.argv[2])
        case "dec":
            decode(sys.argv[2])
        case _:
            print("Unknown Operation.")
            exit(1)
