import struct, os, sys
from cryptography.hazmat.primitives import hashes, hmac
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes


def derive_key(password: str, salt: bytes) -> bytes:
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(),
        length=64,
        salt=salt,
        iterations=480000,
    )
    key = kdf.derive(password.encode())
    return key[:32], key[32:]


def encode(fich):
    password = input("Introduza a password:")
    salt = os.urandom(16)
    enc_key, mac_key = derive_key(password, salt)

    nonce = os.urandom(8)
    counter = 0
    full_nonce = struct.pack("<Q", counter) + nonce

    with open(fich, "rb") as f:
        plaintext = f.read()


    algorithm = algorithms.AES(enc_key)
    mode = modes.CTR(full_nonce)
    cipher = Cipher(algorithm, mode)
    encryptor = cipher.encryptor()
    ct = encryptor.update(plaintext) + encryptor.finalize()

    h = hmac.HMAC(mac_key, hashes.SHA256())
    h.update(full_nonce + ct)
    mac = h.finalize()

    with open(fich + ".enc", "wb") as f:
        f.write(salt)
        f.write(full_nonce)
        f.write(ct)
        f.write(mac)


def decode(fich):
    password = input("Introduza a password:")
    
    with open(fich, 'rb') as f:
        data = f.read()
    salt, nonce, mac, ct = data[:16], data[16:32], data[-32:], data[32:-32]
    enc_key, mac_key = derive_key(password, salt)

    h = hmac.HMAC(mac_key, hashes.SHA256())
    h.update(nonce + ct)
    try:
        h.verify(mac)
    except Exception:
        print("MAC inválido. O ficheiro foi alterado ou a password está errada.")
        return

    algorithm = algorithms.AES(enc_key)
    mode = modes.CTR(nonce)
    cipher = Cipher(algorithm, mode)
    decryptor = cipher.decryptor()
    plaintext = decryptor.update(ct) + decryptor.finalize()

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
