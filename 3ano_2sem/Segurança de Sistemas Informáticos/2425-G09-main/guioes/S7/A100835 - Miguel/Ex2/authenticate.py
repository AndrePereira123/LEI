import struct, os, sys
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.ciphers.aead import AESGCM
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes


def derive_key(password: str, salt: bytes) -> bytes:
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(),
        length=32,
        salt=salt,
        iterations=480000,
    )
    return kdf.derive(password.encode())


def encode_msg(plaintext,password):
    if plaintext == b"":
        return b""
    
    salt = os.urandom(16)
    enc_key = derive_key(password, salt)
    nonce = os.urandom(12)


    cipher = AESGCM(enc_key)
    ct = cipher.encrypt(nonce, plaintext, associated_data=None)

    res = salt + nonce + ct
    
    return res

def decode_msg(data,password):
    if data == b"" or len(data) < 28:
        return b""
    
    salt, nonce, ct = data[:16], data[16:28], data[28:]
    enc_key = derive_key(password, salt)

    cipher = AESGCM(enc_key)
    plaintext = cipher.decrypt(nonce, ct, associated_data=None)

    return plaintext
    
    