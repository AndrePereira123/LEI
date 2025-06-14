import base64
import sys
from Auxiliares import cert_load
import os 
import json
import bson

from cryptography.hazmat.primitives.serialization import pkcs12 
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes 
from cryptography.hazmat.primitives import padding 
from cryptography.hazmat.primitives import serialization
sys.path.append(os.path.abspath(os.path.join(os.path.dirname("Auxiliares.py"), "..")))
from Auxiliares import *
from authenticate import decode_msg, encode_msg

DELIMITER = b'\x00\x00<END>\x00\x00'
my_private_key = None
my_public_key = None

def client_add(args,user_id): 
    if len(args) != 1: 
        print("Invalid number of arguments. Expected 1 argument: file_id.")
        return "Command not recognized"
    
    file_path = args[0]
    file_name = os.path.basename(file_path)

    with open(file_path, "rb") as f:
        file_content = f.read()

    file_key = generate_file_key()

    encrypted_file_key = encrypt_file_key(file_key, my_public_key)
    encrypted_content = encrypt_file_content(file_content, file_key)


    file_data = {
        "command": "ADD-FILE",
        "file_name": file_name,
        "user_id": user_id,
        "file_content": encrypted_content,
        "file_key": encrypted_file_key,  
    }        

    

    msg = bson.BSON.encode(file_data)
    return msg

def client_list(args,user_id):
    if len(args) != 2: 
        print("Invalid number of arguments. Expected 2 arguments: -u/-g , user_id/group_id.")
        return "Command not recognized"
    
    if args[0] == "-u":
        user_id = args[1]
        file_data = {
            "command": "LIST-FILES",
            "user_to_list": user_id,
            "user_id": user_id
        }  
        msg = bson.BSON.encode(file_data)
        return msg      
    elif args[0] == "-g":
        group_id = args[1]
        file_data = {
            "command": "LIST-FILES-GROUP",
            "group_id": group_id,
            "user_id": user_id
        }  
        msg = bson.BSON.encode(file_data)
        return msg

async def client_share(args, user_id, writer, reader, password):
    if len(args) != 3: 
        print("Invalid number of arguments. Expected 3 arguments: file_id, user_id_to_share, permission.")
        return "Command not recognized"
    
    file_id = args[0]
    user_id_to_share = args[1]
    permission = args[2]
    
    if permission not in ["R", "W", "RW"]:
        print("Invalid permission. Use 'R', 'W', or 'RW'.")
        return "Command not recognized"

    try:
        print(f"Requesting metadata for file {file_id}...")
        file_data = {
            "command": "FILE-METADATA",
            "user_id": user_id,
            "file_id": file_id,
            "target_user_public_key": user_id_to_share,
        }  
        msg_to_send = bson.BSON.encode(file_data)
        msg_to_send_bytes = encode_msg(msg_to_send, password)

        writer.write(str(len(msg_to_send_bytes)).encode('utf-8').ljust(16,b' ') + msg_to_send_bytes)
        await writer.drain()
        
        
        msg = await reader.readuntil(DELIMITER)
        if msg[0] == 10: 
            msg = msg[1:]  
        if msg.endswith(DELIMITER): 
            msg = msg.replace(DELIMITER, b"") 
       
        
        received_msg = decode_msg(msg, password)
        dicionario_recebido = bson.BSON.decode(received_msg)
        
        
        my_encrypted_file_key = base64.b64decode(dicionario_recebido["encrypted_file_key"])
        file_key = decrypt_file_key(my_encrypted_file_key, my_private_key)

        target_public_key = dicionario_recebido["target_public_key"]
        target_user_public_key = serialization.load_pem_public_key(target_public_key.encode())

        target_encrypted_file_key = encrypt_file_key(file_key, target_user_public_key)
        
        
        
        file_data = {
            "command": "SHARE-FILE",
            "user_id": user_id,
            "file_id": file_id,
            "file_key": target_encrypted_file_key,  
            "user_id_to_share": user_id_to_share,
            "permission": permission
        }
        msg = bson.BSON.encode(file_data)
        print(f"Share request prepared for user {user_id_to_share}")
        return msg

    except FileNotFoundError as e:
        print(f"Certificate file for user {user_id_to_share} not found: {e}")
        return "Command not recognized"
    except ValueError as e:
        print(f"Value error: {e}")
        return "Command not recognized"
    except TypeError as e:
        print(f"Type error (possibly with public key format): {e}")
        return "Command not recognized"
    except Exception as e:
        print(f"Share operation failed: {e}")
        return "Command not recognized"

def client_delete(args,user_id):
    if len(args) != 1: 
        print("Invalid number of arguments. Expected 1 argument: file_id.")
        return "Command not recognized"
    
    file_id = args[0]
    file_data = {
        "command": "DELETE-FILE",
        "user_id": user_id,
        "file_id": file_id
    }  
    msg = bson.BSON.encode(file_data)
    return msg

async def client_replace_file(args,user_id, writer, reader, password):
    if len(args) != 2: 
        print("Invalid number of arguments. Expected 2 arguments: file_id, file_path.")
        return "Command not recognized"
    
    file_id = args[0]
    file_path = args[1]

    with open(file_path, "rb") as f:
        file_content = f.read()


    file_data = {
        "command": "FILE-METADATA",
        "user_id": user_id,
        "file_id": file_id,
        "target_user_public_key": user_id,
        "flag": "replace"
    }  
    msg_to_send = bson.BSON.encode(file_data)
    msg_to_send_bytes = encode_msg(msg_to_send,password)

    writer.write(str(len(msg_to_send_bytes)).encode('utf-8').ljust(16,b' ') + msg_to_send_bytes)
    await writer.drain()
    msg = await reader.readuntil(DELIMITER)
    if msg[0] == 10: 
            msg = msg[1:]  
    if msg.endswith(DELIMITER): 
                msg = msg.replace(DELIMITER, b"") 
   
        
    received_msg = (decode_msg(msg,password))
    dicionario_recebido = bson.BSON.decode(received_msg)
    
    if "Error" in dicionario_recebido:
        print(f"\033[31m Error: {dicionario_recebido['Error']} \033[0m")
        return "Command not recognized"
    
    my_encrypted_file_key = base64.b64decode(dicionario_recebido["encrypted_file_key"])
    file_key = decrypt_file_key(my_encrypted_file_key, my_private_key)

    file_content = encrypt_file_content(file_content, file_key)

    file_data = {
        "command": "REPLACE-FILE",
        "user_id": user_id,
        "file_id": file_id,
        "file_content": file_content
    }  
    msg = bson.BSON.encode(file_data)
    return msg

def client_details_file (args,user_id):
    if len(args) != 1: 
        print("Invalid number of arguments. Expected 1 argument: file_id.")
        return "Command not recognized"
    file_id = args[0]
    file_data = {
        "command": "FILE-DETAILS",
        "user_id": user_id,
        "file_id": file_id
    }  
    msg = bson.BSON.encode(file_data)
    return msg


def client_read_file(args, user_id):
    if len(args) != 1: 
        print("Invalid number of arguments. Expected 1 argument: file_id.")
        return "Command not recognized"
    
    file_id = args[0]
    file_data = {
        "command": "FILE-READ",
        "user_id": user_id,
        "file_id": file_id
    }  
    msg = bson.BSON.encode(file_data)
    return msg

def client_revoke(args,user_id):
    if len(args) != 2: 
        print("Invalid number of arguments. Expected 2 arguments: file_id, user_id_to_revoke.")
        return "Command not recognized"
    
    file_id = args[0]
    user_id_to_revoke = args[1]

    if (user_id == user_id_to_revoke):
        raise ValueError("You cannot revoke your own access.")
    
    file_data = {
        "command": "REVOKE-FILE",
        "user_id": user_id,
        "file_id": file_id,
        "user_id_to_revoke": user_id_to_revoke
    }  
    msg = bson.BSON.encode(file_data)
    return msg

######################################################################################################

def group_create(args,user_id):
    if len(args) != 2: 
        print("Invalid number of arguments. Expected 1 argument: group_name.")
        return "Command not recognized"
    
    file_key = generate_file_key()
    
    encrypted_file_key = encrypt_file_key(file_key, my_public_key)

    group_name = args[1]      
    group_data = {
        "command": "CREATE-GROUP",
        "group_name": group_name,
        "owner_id": user_id,
        "group_key": encrypted_file_key
    }
    msg = bson.BSON.encode(group_data)
    return msg

async def group_add_file(args, user_id, writer, reader, password):
    if len(args) != 3: 
        print("Invalid number of arguments. Expected 2 arguments: group_id, file_path.")
        return "Command not recognized"
    
    group_id = args[1]  
    file_path = args[2]
    file_name = os.path.basename(file_path)

    file_data = {
            "command": "FILE-METADATA",
            "user_id": user_id,
            "file_id": group_id,
            "target_user_public_key": user_id,
            "flag": "add_file_group"
        }  
    msg_to_send = bson.BSON.encode(file_data)
    msg_to_send_bytes = encode_msg(msg_to_send, password)

    writer.write(str(len(msg_to_send_bytes)).encode('utf-8').ljust(16,b' ') + msg_to_send_bytes)
    await writer.drain()
    
    
    msg = await reader.readuntil(DELIMITER)
    if msg[0] == 10: 
        msg = msg[1:]  
    if msg.endswith(DELIMITER): 
        msg = msg.replace(DELIMITER, b"") 
    
    
    received_msg = decode_msg(msg, password)
    dicionario_recebido = bson.BSON.decode(received_msg)
    
    
    my_encrypted_file_key = base64.b64decode(dicionario_recebido["encrypted_file_key"])
    file_key = decrypt_file_key(my_encrypted_file_key, my_private_key)

    
    with open(file_path, "rb") as f:
        file_content = f.read()
    
    encrypted_file_key = encrypt_file_key(file_key, my_public_key)
    encrypted_content = encrypt_file_content(file_content, file_key)

    group_data = {
        "command": "ADD-FILE-GROUP",
        "user_id": user_id,
        "group_id": group_id,
        "file_name": file_name,
        "file_content": encrypted_content,
        "file_key": encrypted_file_key
    }   
    msg = bson.BSON.encode(group_data)
    return msg

def group_delete(args,user_id):
    if len(args) != 2: 
        print("Invalid number of arguments. Expected 1 argument: group_id.")
        return "Command not recognized"
    group_id = args[1]  

    group_data = {
        "command": "DELETE-GROUP",
        "user_id": user_id,
        "group_id": group_id,
    }   
    msg = bson.BSON.encode(group_data)
    return msg

async def group_add_user(args,user_id, writer, reader, password):
    if len(args) != 4: 
        print("Invalid number of arguments. Expected 3 arguments: group_id, user_id_to_add, permissions.")
        return "Command not recognized"
    
    group_id = args[1]  
    user_id_to_add = args[2]        
    permissions = args[3]       
    if permissions not in ["R", "W", "RW"]:
        raise ValueError("Invalid permission. Use 'R', 'W', or 'RW'.")
    if (user_id == user_id_to_add):
        raise ValueError("You cannot add yourself to the group.")

    file_data = {
            "command": "FILE-METADATA",
            "user_id": user_id,
            "file_id": group_id,
            "target_user_public_key": user_id_to_add,
        }  
    msg_to_send = bson.BSON.encode(file_data)
    msg_to_send_bytes = encode_msg(msg_to_send, password)

    writer.write(str(len(msg_to_send_bytes)).encode('utf-8').ljust(16,b' ') + msg_to_send_bytes)
    await writer.drain()
    
    
    msg = await reader.readuntil(DELIMITER)
    if msg[0] == 10: 
        msg = msg[1:]  
    if msg.endswith(DELIMITER): 
        msg = msg.replace(DELIMITER, b"") 
    
    
    received_msg = decode_msg(msg, password)
    dicionario_recebido = bson.BSON.decode(received_msg)
    
    
    my_encrypted_file_key = base64.b64decode(dicionario_recebido["encrypted_file_key"])
    file_key = decrypt_file_key(my_encrypted_file_key, my_private_key)

    target_public_key = dicionario_recebido["target_public_key"]
    target_user_public_key = serialization.load_pem_public_key(target_public_key.encode())

    target_encrypted_file_key = encrypt_file_key(file_key, target_user_public_key)

    group_data = {
        "command": "ADD-USER-GROUP",
        "user_id": user_id,
        "group_id": group_id,
        "user_id_to_add": user_id_to_add,
        "permissions": permissions,
        "file_key": target_encrypted_file_key
    }   
    msg = bson.BSON.encode(group_data)
    return msg

def group_delete_user(args,user_id):
    if len(args) != 3: 
        print("Invalid number of arguments. Expected 2 arguments: group_id, user_id_to_delete.")
        return "Command not recognized"
    group_id = args[1]  
    user_id_to_delete = args[2]

    if (user_id == user_id_to_delete):
        raise ValueError("You cannot remove yourself from the group.")

    group_data = {
        "command": "DELETE-USER-GROUP",
        "user_id": user_id,
        "group_id": group_id,
        "user_id_to_delete": user_id_to_delete
    }   
    msg = bson.BSON.encode(group_data)
    return msg

def group_list(args,user_id):
    if len(args) != 1: 
        print("Invalid number of arguments. Expected 0 arguments.")
        return "Command not recognized"
    
    group_data = {
        "command": "GROUP-LIST",
        "user_id": user_id,
    }   
    msg = bson.BSON.encode(group_data)
    return msg

####################################################################################################################################


def get_user_id(p12_path):

    with open(p12_path, "rb") as f:
        p12_data = f.read()

    private_key, cert, additional_certs = pkcs12.load_key_and_certificates(p12_data, None)

    subject = cert.subject
    user_id = None

    for attribute in subject:
        if attribute.oid._name == "pseudonym":
            user_id = attribute.value
            break

    return user_id


async def client_execute(str,user_id,args, private_key,writer,reader,password):  # args é uma lista com todos os argumentos do comando

    global my_private_key, my_public_key
    my_private_key = private_key
    my_public_key = private_key.public_key()

    if (str == "add"):
        msg = client_add(args,user_id)
        return msg
    elif (str == "list"):
        msg = client_list(args,user_id)
        return msg
    elif (str == "share"):
        msg = await client_share(args,user_id, writer, reader, password)
        return msg
    elif (str == "delete"):
        msg = client_delete(args,user_id)
        return msg
    elif (str == "replace"):
        msg = await client_replace_file(args,user_id , writer, reader, password)
        return msg
    elif (str == "read"):
        msg = client_read_file(args,user_id)
        return msg
    elif (str == "details"):
        msg = client_details_file(args,user_id)
        return msg
    elif (str == "revoke"):
        msg = client_revoke(args,user_id)
        return msg
    
    
    elif (str == "group"):
        if args[0] == "create":
            msg = group_create(args,user_id)
            return msg
        
        elif args[0] == "delete":
            msg = group_delete(args,user_id)
            return msg

        elif args[0] == "add":  
            msg = await group_add_file(args,user_id , writer, reader, password)
            return msg

        elif args[0] == "add-user":
            msg = await group_add_user(args,user_id,writer, reader, password)
            return msg
        
        elif args[0] == "delete-user":
            msg = group_delete_user(args,user_id)
            return msg
        
        elif args[0] == "list":
            msg = group_list(args,user_id)
            return msg
        
    return "Command not recognized"

def decrypt_file_from_server(response_json, private_key):
    response = json.loads(response_json)
    
    file_name = response["file_name"]
    encrypted_content = base64.b64decode(response["encrypted_content"])
    encrypted_key = base64.b64decode(response["encrypted_key"])
    
    
    file_key = decrypt_file_key(encrypted_key, private_key)
    
    
    file_content = decrypt_file_content(encrypted_content, file_key)
    
    return file_name, file_content


def decrypt_file_content(encrypted_content, file_key):
    """Decrypt file content using AES-256-CBC"""
    iv = encrypted_content[:16]  
    actual_content = encrypted_content[16:]  
    
    
    cipher = Cipher(algorithms.AES(file_key), modes.CBC(iv))
    decryptor = cipher.decryptor()
    padded_content = decryptor.update(actual_content) + decryptor.finalize()
    
    
    unpadder = padding.PKCS7(algorithms.AES.block_size).unpadder()
    content = unpadder.update(padded_content) + unpadder.finalize()
    
    return content

def handle_file_details_response(response_dict, private_key):
    try:
        
        file_name = response_dict["file_name"]
        encrypted_content = base64.b64decode(response_dict["encrypted_content"])
        encrypted_key = base64.b64decode(response_dict["encrypted_key"])
        
        
        file_key = decrypt_file_key(encrypted_key, private_key)
        
        
        file_content = decrypt_file_content(encrypted_content, file_key)
        
        
        return {
            "file_name": file_name,
            "file_content": file_content.decode('utf-8')
        }
    except json.JSONDecodeError:
        return {"error": f"Invalid JSON response from server: {response_dict}"}
    except KeyError as e:
        return {"error": f"Missing key in server response: {str(e)}"}
    except Exception as e:
        return {"error": f"Decryption error: {str(e)}"}



##cert_bytes: b'-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxgjYnr1d5qDDSUcqTow9\nFO4ezyO/0F/opsvm1Ywp6lYhdCXeWOMeq9nWP4ypIOslQdHD67BBssSMwMNnHLHj\n7YzFhB9xdIZHw/o77/XxbnNey2B+AQ/M1VaEZrhsRygxCFXTaUtbMMoZenBoQmYL\nmDor+WyByFwuZihhpw0DlkNsU8VEW3Af4iFeck5PTY0x+YYix/YIOvx/0NI0gmLD\ndMqrlf7fALlPkwcE2N4+lvUiyJNaCm+lozYqXuDOWfCa5FhIpAreuGafTHHX/CcJ\nYSNuNXDKV7Klt0IhGnWmb/Iakn3SloJ8ALb77ybpL45FKJ3q1fr8gZcXR/8dCE23\nrwIDAQAB\n-----END PUBLIC KEY-----\n'
##pem_bytes:  b'-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtImM+XTzNobVHnk2SiCX\nsFcZ/Xih90yRNxNoG0+NEx9ehBjXCp0Ya9H9HLvJc6aruDVvhNZPOytYwvQ1vFAA\nCUfWykA3RdHoix8YGYIYHasYnqXDSfN9oPjVPIufJOXoJtpqz8TTfzjo6kdSkd8V\n1VZAN1zqEFfLONDtQ5NtEMBMrvhNBK8NmekkK+t1txEaJamVnpyi8D4Y96aZvpqT\n5Di0uqeE64cS865Cl7eUF9+crD9CbfOrqb0IrWNjmnwuIkjcrZ0mdr4zvde/6xkk\nL3BgupwvoM3V3fayfqOlY9GOu+t1dga1gnRPvjx1++Y/71JPRFeDjao8FoX1EvEs\nzQIDAQAB\n-----END PUBLIC KEY-----\n'
