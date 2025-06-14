import json
import os
import base64
from cryptography import x509
import datetime
from cryptography.hazmat.primitives.asymmetric import padding as asym_padding # type: ignore
from cryptography.hazmat.primitives import hashes # type: ignore


from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes # type: ignore
from cryptography.hazmat.primitives import padding # type: ignore
from cryptography.hazmat.primitives.asymmetric import padding as asym_padding # type: ignore
from cryptography.hazmat.primitives import hashes # type: ignore

def cert_load(fname):
    """lê certificado de ficheiro"""
    with open(fname, "rb") as fcert:
        cert = x509.load_pem_x509_certificate(fcert.read())
    return cert


def cert_validtime(cert, now=None):
    """valida que 'now' se encontra no período
    de validade do certificado."""
    if now is None:
        now = datetime.datetime.now(tz=datetime.timezone.utc)
    if now < cert.not_valid_before_utc or now > cert.not_valid_after_utc:
        raise x509.verification.VerificationError(
            "Certificate is not valid at this time"
        )


def cert_validsubject(cert, attrs=[]):
    """verifica atributos do campo 'subject'. 'attrs'
    é uma lista de pares '(attr,value)' que condiciona
    os valores de 'attr' a 'value'."""
    print(cert.subject)
    for attr in attrs:
        if cert.subject.get_attributes_for_oid(attr[0])[0].value != attr[1]:
            raise x509.verification.VerificationError(
                "Certificate subject does not match expected value"
            )


def cert_validexts(cert, policy=[]):
    """valida extensões do certificado.
    'policy' é uma lista de pares '(ext,pred)' onde 'ext' é o OID de uma extensão e 'pred'
    o predicado responsável por verificar o conteúdo dessa extensão."""
    for check in policy:
        ext = cert.extensions.get_extension_for_oid(check[0]).value
        if not check[1](ext):
            raise x509.verification.VerificationError(
                "Certificate extensions does not match expected value"
            )


def valida_certALICE(ca_cert_name,file_name,common_name):
    try:
        print("Verifying certificate...\n\n")
        cert = cert_load(file_name)
        ca_cert = cert_load(ca_cert_name)
        # obs: pressupõe que a cadeia de certifica só contém 2 níveis
        print("Certificado - issuer:", cert.issuer)
        print("CA - subject:", cert.subject)
        cert.verify_directly_issued_by(ca_cert)
        # verificar período de validade...
        cert_validtime(cert)
        # verificar identidade... (e.g.)
        cert_validsubject(cert, [(x509.NameOID.COMMON_NAME, common_name)])
        # verificar aplicabilidade... (e.g.)
        # cert_validexts(
        #     cert,
        #     [
        #         (
        #             x509.ExtensionOID.EXTENDED_KEY_USAGE,
        #             lambda e: x509.oid.ExtendedKeyUsageOID.CLIENT_AUTH in e,
        #         )
        #     ],
        # )
        print("Certificate is valid!")
        return True
    except Exception as e:
        print("Certificate is invalid!")
        print(e)
        return False
    

def mkpair(x, y):
    return x + y


def add_public_key_database(user_id, public_key):
    database_file = "database.json"
    
    if os.path.exists(database_file) and os.stat(database_file).st_size != 0:
        with open(database_file, "r") as f:
            data = json.load(f)
    else:
        data = {}

    if "public_keys" not in data or not isinstance(data["public_keys"], dict):
        data["public_keys"] = {}

    data["public_keys"][user_id] = public_key

    with open(database_file, "w") as f:
        json.dump(data, f, indent=4)

def get_public_key_database(user_id):
    database_file = "database.json"
    
    if os.path.exists(database_file) and os.stat(database_file).st_size != 0:
        with open(database_file, "r") as f:
            data = json.load(f)
    else:
        data = {}

    if "public_keys" not in data or not isinstance(data["public_keys"], dict):
        return None
    return data["public_keys"].get(user_id, None)
        
        

def update_database(file_entry, owner_id):
    database_file = "database.json"
    
    if os.path.exists(database_file):
        with open(database_file, "r") as f:
            data = json.load(f)
    else:
        data = {"vaults": []}

    if "vaults" not in data:
        data["vaults"] = []

    
    vault_user = None

    for vault in data["vaults"]:
    
        for vault in data["vaults"]:
            if not vault or "owner_id" not in vault:
                continue
            if vault["owner_id"] == owner_id:
                vault_user = vault
                break

    
    if not vault_user:
        vault_user = {
            "owner_id": owner_id,
            "files": []
        }
        data["vaults"].append(vault_user)
    elif "files" not in vault_user:
        vault_user["files"] = []
    
    vault_user["files"].append(file_entry)

    with open(database_file, "w") as f:
        json.dump(data, f, indent=4)



def add_group_database(group_entry):
    database_file = "database.json"
    if os.path.exists(database_file):
        with open(database_file, "r") as f:
            data = json.load(f)
    else:
        data = {"groups": []}

    if "groups" not in data:
        data["groups"] = []
    

    data["groups"].append(group_entry)

    with open(database_file, "w") as f:
        json.dump(data, f, indent=4)




def add_group_file_database(file_entry,group_id):
    database_file = "database.json"
    if os.path.exists(database_file):
        with open(database_file, "r") as f:
            data = json.load(f)
    else:
        data = {"groups": []}

    if "groups" not in data:
        data["groups"] = []

    found_group = False


    for group in data["groups"]:
        if not group or "group_id" not in group:
            continue

        if group["group_id"] == group_id:
            group["files"].append(file_entry)
            found_group = True
        
        break

    if not found_group:
        print(f"Warning: No group with ID {group_id} found in database")

    with open(database_file, "w") as f:
        json.dump(data, f, indent=4)


def decrypt_file_key(encrypted_key, user_private_key):
    """Decrypt the file key with user's private key"""
    file_key = user_private_key.decrypt(
        encrypted_key,
        asym_padding.OAEP(
            mgf=asym_padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )
    return file_key


def generate_file_key():
    """Generate a random key for file encryption"""
    return os.urandom(32)  # 256-bit key


def encrypt_file_key(file_key, user_public_key):
    """Encrypt the file key with user's public key"""
    encrypted_key = user_public_key.encrypt(
        file_key,
        asym_padding.OAEP(
            mgf=asym_padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )
    return encrypted_key



def encrypt_file_content(content, file_key):
    """Encrypt file content using AES-256-CBC"""
    iv = os.urandom(16)  # Generate initialization vector
    
    # Pad the content
    padder = padding.PKCS7(algorithms.AES.block_size).padder()
    padded_data = padder.update(content) + padder.finalize()
    
    # Encrypt the content
    cipher = Cipher(algorithms.AES(file_key), modes.CBC(iv))
    encryptor = cipher.encryptor()
    encrypted_content = encryptor.update(padded_data) + encryptor.finalize()
    
    # Return IV + encrypted content
    return iv + encrypted_content



def store_file_metadata(file_id, encrypted_keys, owner_id):
    """Store encrypted file keys"""
    metadata_file = os.path.join("vaults", "metadata", f"{file_id}.json")
    
    
    os.makedirs(os.path.join("vaults", "metadata"), exist_ok=True)
    
    
    metadata = {
        "owner_id": owner_id,
        "encrypted_keys": {}
    }
    
    for user_id, encrypted_key in encrypted_keys.items():
        # Base64 encode the binary key for JSON storage
        metadata["encrypted_keys"][user_id] = base64.b64encode(encrypted_key).decode('utf-8')
    
    with open(metadata_file, "w") as f:
        json.dump(metadata, f, indent=4)

def get_file_key(file_id, user_id, user_private_key):
    """Get file key for a specific user"""
    metadata_file = os.path.join("vaults", "metadata", f"{file_id}.json")
    
    with open(metadata_file, "r") as f:
        metadata = json.load(f)
    
    if user_id not in metadata["encrypted_keys"]:
        raise ValueError(f"User {user_id} does not have access to file {file_id}")
    
    encrypted_key = base64.b64decode(metadata["encrypted_keys"][user_id])
    file_key = decrypt_file_key(encrypted_key, user_private_key)
    
    return file_key