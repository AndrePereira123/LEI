import shutil
import sys
import uuid
import logging
import os
import base64
import json


sys.path.append(os.path.abspath(os.path.join(os.path.dirname("Auxiliares.py"), "..")))
from Auxiliares import *

list_of_ids_path = "list_of_ids.json"


################################################# USUARIOS ##############################################

def store_public_key_server(received_msg):
    """Stores a user's public key in the database."""
    user_id = received_msg["user_id"]
    public_key = received_msg["public_key"]
    
    try:
        with open("database.json", "r") as f:
            data = json.load(f)
        

        if "users" not in data:
            data["users"] = []
        

        user_exists = False
        for user in data["users"]:
            if user["user_id"] == user_id:
                user["public_key"] = public_key
                user_exists = True
                break
        

        if not user_exists:
            data["users"].append({
                "user_id": user_id,
                "public_key": public_key
            })
        

        with open("database.json", "w") as f:
            json.dump(data, f, indent=4)
        
        logging.info("Public key stored for user %s", user_id)
        
        response = {
            "message": f"Public key for user {user_id} stored successfully",
        }
        return response
        
    except Exception as e:
        logging.error("Error storing public key for user %s: %s", user_id, str(e))
        return f"Error storing public key: {str(e)}"


def get_public_key_server(received_msg):
    """Retrieves a user's public key from the database."""
    user_id = received_msg["user_id"]
    target_user_id = received_msg.get("target_user_id", user_id)  # If not specified, get own key
    
    try:
        with open("database.json", "r") as f:
            data = json.load(f)
        

        if "users" in data:
            for user in data["users"]:
                if user["user_id"] == target_user_id and "public_key" in user:
                    response = {
                        "user_id": target_user_id,
                        "public_key": user["public_key"]
                    }
                    logging.info("Public key for user %s retrieved by user %s", target_user_id, user_id)
                    return response
        
        logging.error("Public key for user %s not found", target_user_id)
        return f"Public key for user {target_user_id} not found"
        
    except Exception as e:
        logging.error("Error retrieving public key for user %s: %s", target_user_id, str(e))
        return f"Error retrieving public key: {str(e)}"

def add_file_user_server(received_msg):
    file_name = received_msg["file_name"]
    owner_id = received_msg["user_id"]
    file_key = received_msg["file_key"]
    file_content = received_msg["file_content"]


    global list_of_ids_path
    file_id = received_msg["file_name"] + "_" + str(uuid.uuid4())[:8]

    with open(list_of_ids_path, "r") as f:
            lista_ids = json.load(f)
            lista_ids["ids"] = lista_ids.get("ids", [])
            while file_id in lista_ids:
                file_id = received_msg["file_name"] + "_" + str(uuid.uuid4())[:8]
                logging.info("File ID %s already exists, generating a new one", file_id)
            lista_ids["ids"].append(file_id)    

    with open(list_of_ids_path, "w") as f:
        json.dump(lista_ids, f, indent=4)


    user_dir = os.path.join("vaults", owner_id)
    os.makedirs(user_dir, exist_ok=True)
    
    

    file_path = os.path.join(user_dir, file_id)
    with open(file_path, "wb") as f:
        f.write(file_content)
    
 
    encrypted_keys = {owner_id: file_key}
    store_file_metadata(file_id, encrypted_keys, owner_id)

    permissions = [
        {"user_id": owner_id, "permission": "RW"}
    ]
    
    file_entry = {
        "file_id": file_id,
        "file_name": file_name,  # nome original
        "permissions": permissions
    }
    
    update_database(file_entry, owner_id)
    txt = f"File {file_name} added with ID {file_id}"
    logging.info("File added: %s", file_entry)


    response = {
        "message": txt,
    }
    logging.info("File %s added with ID %s", file_name, file_id)

    return response   
    
    

def list_files_user_server(received_msg):
        """Lista os ficheiros disponíveis para acesso. 
            Esses ficheiros podem pertencer ao cofre pessoal, 
            terem sido partilhados por outro utilizador ou partilhados no 
            contexto de um grupo ao qual o utilizador pertence."""
        
        user_id = received_msg["user_id"]
        user_to_list = received_msg["user_to_list"]

        list = []
        with open("database.json", "r") as f:
            data = json.load(f)

        for v in data["vaults"]:
          for f in v["files"]:
            for p in f["permissions"]:
                if p["user_id"] == user_id and "R" in p["permission"]:
                        for p in f["permissions"]:
                            if p["user_id"] == user_to_list:
                                    list.append((f["file_name"],f["file_id"]))
                                    break
        
        ## ficheiros em grupos partilhados

        if len(list) == 0:
            response = {
                "message": f"No files found for user {user_to_list}",
                "files": []
            }
        else:
            response = {
                "message": f"Files for user",
                "files": list
            }
            logging.info("Files for user %s: %s", user_to_list, list)

        return response

def metadata_file_user_server(received_msg):
    file_id = received_msg["file_id"]
    user_id = received_msg["user_id"]
    target_user_id = received_msg["target_user_public_key"]


    
    metadata_file = os.path.join("vaults", "metadata", f"{file_id}.json")
    with open(metadata_file, "r") as f:
        metadata = json.load(f)

    if user_id != metadata["owner_id"] and not received_msg.get("flag", False):
        txt = f"You are no the owner of this file"
        logging.error("User %s does not have permission to share file %s", user_id, file_id)
        return txt
    
    encrypt_file_key = metadata["encrypted_keys"].get(user_id)
    if encrypt_file_key is None:
        with open("database.json", "r") as f:
            data = json.load(f)
            groups = data.get("groups", [])
            for g in groups:
                for f in g["files"]:
                    if f["file_id"] == file_id:
                        for p in g["users"]:
                            if p["user_id"] == user_id and "R" in p["permission"]:
                                group = g
                                break
                    if group: break
                if group: break
            metadata_file = os.path.join("vaults", "metadata", f"{group["group_id"]}.json")
            with open(metadata_file, "r") as f:
                metadata = json.load(f)
                encrypt_file_key = metadata["encrypted_keys"].get(user_id)

        if encrypt_file_key is None:
            txt = f"You do not have access to this file"
            logging.error("User %s does not have access to file %s", user_id, file_id)
            return txt
    
    target_public_key = get_public_key_database(target_user_id)
    response = {
        "encrypted_file_key": encrypt_file_key,
        "target_public_key": target_public_key,
    }
    
    logging.info("Encrypted_file_key for file %s sent to user %s", file_id, user_id)
    return response
    

def share_file_user_server(received_msg):
    file_id = received_msg["file_id"]
    user_id = received_msg["user_id"]
    user_id_to_share = received_msg["user_id_to_share"]
    permission = received_msg["permission"]
    new_file_key = received_msg["file_key"]
    
    metadata_file = os.path.join("vaults", "metadata", f"{file_id}.json")
    with open(metadata_file, "r") as f:
        metadata = json.load(f)
    
    if metadata["owner_id"] != user_id:
        txt = f"Only the owner can share files"
        logging.error("User %s tried to share file %s but is not the owner", user_id, file_id)
        return txt

    with open("database.json", "r") as f:
        data = json.load(f)
        vaults = data.get("vaults", [])
        vault_partilha = None

        for v in vaults:
            if v["owner_id"] == user_id:
                vault_partilha = v
                break

    if vault_partilha is None:
        txt = f"Vault for user {user_id} not found"
        logging.error("Vault for user %s not found", user_id)
        return txt

    file_to_share = None
    for file in vault_partilha["files"]:
        if file["file_id"] == file_id:
            file_to_share = file
            break
    
    if file_to_share is None:
        txt = f"File {file_id} not found in user {user_id}'s vault"
        logging.error("File %s not found in user %s's vault", file_id, user_id)
        return txt
    
    
    
    if user_id_to_share not in metadata["encrypted_keys"]:
        metadata["encrypted_keys"][user_id_to_share] = base64.b64encode(new_file_key).decode('utf-8')
        with open(metadata_file, "w") as f:
            json.dump(metadata, f, indent=4)

    for p in file_to_share["permissions"]:
        if p["user_id"] == user_id_to_share:
            if permission not in p["permission"]:
                p["permission"] += permission
                txt = f"File {file_id} shared with user {user_id_to_share} with extra permission {permission}"
            else: 
                txt = f"File {file_id} was already shared with user {user_id_to_share} with permission {permission}"
            break
    else:            
        file_to_share["permissions"].append({"user_id": user_id_to_share, "permission": permission})
        txt = f"File {file_id} shared with user {user_id_to_share} with permission {permission}"
    
    with open("database.json", "w") as f:
        json.dump(data, f, indent=4)
    
    

    response = {
        "message": txt,
    }

    logging.info("File %s shared with user %s with permission %s", file_id, user_id_to_share, permission)
    return response   
    


def delete_file_user_server(received_msg):
    file_id = received_msg["file_id"]
    user_id = received_msg["user_id"]

    with open("database.json", "r") as f:
        data = json.load(f)
        vaults = data.get("vaults", [])
        groups = data.get("groups", [])

    file_to_delete = None
    relevant_vault = None
    relevant_group = None
    for v in vaults:
        for file in v["files"]:
            if file["file_id"] == file_id:
                file_to_delete = file
                relevant_vault = v
                break
    for g in groups:
        for file in g["files"]:
            if file["file_id"] == file_id:
                file_to_delete = file
                relevant_group = g
                break

    if file_to_delete is None:
        txt = f"File {file_id} does not exist"
        logging.error("File %s search has failed, file does not exist", file_id)
        return txt

    if relevant_vault is not None:
        if relevant_vault["owner_id"] == user_id:
            relevant_vault["files"].remove(file_to_delete)
            os.remove(os.path.join("vaults", relevant_vault["owner_id"], file_to_delete["file_id"]))
            txt = f"File {file_id} deleted from your personal vault"
            logging.info("File %s deleted from user %s's vault", file_id, user_id)
        else:
            relevant_vault["permissions"] = [p for p in relevant_vault["permissions"] if p["user_id"] != user_id]
            txt = f"Permission for file {file_id} has been successfully revoked"
            logging.info("Permission for file %s deleted from user %s", file_id, user_id)

    elif relevant_group is not None and relevant_group["owner_id"] == user_id:
        relevant_group["files"].remove(file_to_delete)
        os.remove(os.path.join("vaults", relevant_group["group_id"], file_to_delete["file_id"]))
        txt = f"File {file_id} deleted from group {relevant_group['group_name']}"
        logging.info("File %s deleted from group %s's vault", file_id, relevant_group['group_name'])

    with open("database.json", "w") as f:
        json.dump(data, f, indent=4)

    with open(list_of_ids_path, "r") as f:
        lista_ids = json.load(f)
        lista_ids["ids"] = lista_ids.get("ids", [])
        if file_id in lista_ids["ids"]:
            lista_ids["ids"].remove(file_id)
            logging.info("File ID %s removed from list of IDs", file_id)
        else:
            logging.warning("File ID %s not found in list of IDs", file_id)
    
    os.remove(os.path.join("vaults", "metadata", f"{file_id}.json"))

    with open(list_of_ids_path, "w") as f:
        json.dump(lista_ids, f, indent=4)

    response = {
        "message": txt,
    }
    return response   

def replace_file_user_server(received_msg):
    file_id = received_msg["file_id"]
    user_id = received_msg["user_id"]
    file_content = received_msg["file_content"]

    with open("database.json", "r") as f:
        data = json.load(f)
        vaults = data.get("vaults", [])
        groups = data.get("groups", [])

    file_to_replace = None
    relevant_vault = None
    relevant_group = None
    for v in vaults:
        for file in v["files"]:
            if file["file_id"] == file_id:
                file_to_replace = file
                relevant_vault = v
                break
    for g in groups:
        for file in g["files"]:
            if file["file_id"] == file_id:
                file_to_replace = file
                relevant_group = g
                break

    if file_to_replace is None:
        txt = f"File {file_id} does not exist"
        logging.error("File %s search has failed, file does not exist", file_id)
        return txt

    if relevant_vault is not None:
        if relevant_vault["owner_id"] == user_id:
            with open(os.path.join("vaults", relevant_vault["owner_id"], file_to_replace["file_id"]), "wb") as f:
                f.write(file_content)
            txt = f"File {file_id} content replaced in your personal vault"
            logging.info("File %s content replaced in user %s's vault by %s", file_id, relevant_vault['owner_id'],user_id)       
        elif file_to_replace["permissions"]:
            for p in file_to_replace["permissions"]:
                if p["user_id"] == user_id and "W" in p["permission"]:
                    with open(os.path.join("vaults", relevant_vault["owner_id"], file_to_replace["file_id"]), "wb") as f:
                        f.write(file_content)
                    txt = f"File {file_id} content replaced in your shared vault (owner: {relevant_vault['owner_id']})"
                    logging.info("File %s content replaced in user %s's vault by %s", file_id, relevant_vault['owner_id'],user_id)
                    break 
            else:
                txt = f"You do not have permission to replace this file"
                logging.error("User %s did not have permission to replace the file %s", user_id, file_id)
        else:
            txt = f"You do not have permission to replace this file"
            logging.error("User %s did not have permission to replace the file %s", user_id, file_id)

    elif relevant_group["owner_id"] == user_id:
        with open(os.path.join("vaults", relevant_group["group_id"], file_to_replace["file_id"]), "wb") as f:
            f.write(file_content)
        txt = f"File {file_id} replaced in your group {relevant_group['group_name']}"
        logging.info("File %s replaced in group %s's vault by owner(%s)", file_id, relevant_group['group_name'],user_id)
    else: 
        for u in relevant_group["users"]:
            if u["user_id"] == user_id and "W" in u["permission"]:
                with open(os.path.join("vaults", relevant_group["group_id"], file_to_replace["file_id"]), "wb") as f:
                    f.write(file_content)
                txt = f"File {file_id} replaced in shared group {relevant_group['group_name']} owned by {relevant_group['owner_id']}"
                logging.info("File %s replaced in group %s's vault by %s", file_id, relevant_group['group_name'],user_id)
                break
        else: 
            txt = f"You do not have permission to replace this file"
            logging.error("User %s did not have permission to replace the file %s", user_id, file_id)

    with open("database.json", "w") as f:
        json.dump(data, f, indent=4)

    response = {
        "message": txt,
    }
    return response   



def details_file_user_server(received_msg):
    file_id = received_msg["file_id"]
    user_id = received_msg["user_id"]
    

    with open("database.json", "r") as f:
        data = json.load(f)
        vaults = data.get("vaults", [])
        groups = data.get("groups", [])
    
    file_to_send = None
    relevant_vault = None
    relevant_group = None
    
    for v in vaults:
        for file in v["files"]:
            if file["file_id"] == file_id:
                file_to_send = file
                relevant_vault = v
                break
                

    if file_to_send is None:
        for g in groups:
            for file in g["files"]:
                if file["file_id"] == file_id:
                    file_to_send = file
                    relevant_group = g
                    break
    
    if file_to_send is None:
        txt = f"File {file_id} does not exist"
        logging.error("File %s search has failed, file does not exist", file_id)
        return txt
    
    
    if relevant_vault is not None:
        owner_id = relevant_vault["owner_id"]
    else:
        owner_id = relevant_group["owner_id"]
    
    has_permission = False
    
    if relevant_vault is not None:
        for p in file_to_send["permissions"]:
            if p["user_id"] == user_id and "R" in p["permission"]:
                has_permission = True
                break
    else:  
        if relevant_group["owner_id"] == user_id:
            has_permission = True
        else:
            for u in relevant_group["users"]:
                if u["user_id"] == user_id and "R" in u["permission"]:
                    has_permission = True
                    break
    
    if not has_permission:
        txt = f"You do not have permission to view this file's details"
        logging.error("User %s did not have permission to view the details of the file %s", user_id, file_id)
        return txt
    
    try:
        metadata_file = os.path.join("vaults", "metadata", f"{file_id}.json")
        with open(metadata_file, "r") as f:
            metadata = json.load(f)
        
        if user_id not in metadata["encrypted_keys"]:
            txt = f"You do not have access to this file"
            return txt
        
        response = {
            "file_name": file_to_send["file_name"],
            "owner": owner_id,
            "permissions": [p for p in file_to_send["permissions"]]
        }

    except Exception as e:
        txt = f"Error retrieving file: {str(e)}"
        logging.error("Error retrieving file %s for user %s: %s", file_id, user_id, str(e))
    
    
    logging.info("File details sent to %s", user_id)
    return response   


def read_file_user_server(received_msg):
    file_id = received_msg["file_id"]
    user_id = received_msg["user_id"]

    with open("database.json", "r") as f:
        data = json.load(f)
        vaults = data.get("vaults", [])
        groups = data.get("groups", [])

    file_to_send = None
    context = None
    is_vault = False

    for v in vaults:
        for file in v.get("files", []):
            if file["file_id"] == file_id:
                file_to_send = file
                context = v
                is_vault = True
                break
        if file_to_send:
            break

    if not file_to_send:
        for g in groups:
            for file in g.get("files", []):
                if file["file_id"] == file_id:
                    file_to_send = file
                    context = g
                    break
            if file_to_send:
                break

    if not file_to_send:
        logging.error("File %s search has failed, file does not exist", file_id)
        return f"File {file_id} does not exist"

    has_permission = False

    if is_vault:
        for p in file_to_send.get("permissions", []):
            if p["user_id"] == user_id and "R" in p.get("permission", ""):
                has_permission = True
                break
    else:
        if context["owner_id"] == user_id:
            has_permission = True
        else:
            for u in context.get("users", []):
                if u["user_id"] == user_id and "R" in u.get("permission", ""):
                    has_permission = True
                    break

    if not has_permission:
        logging.error("User %s did not have permission to view the details of the file %s", user_id, file_id)
        return "You do not have permission to view this file's details"

    try:
        if (is_vault):
            metadata_file = os.path.join("vaults", "metadata", f"{file_id}.json")
        else:
            metadata_file = os.path.join("vaults", "metadata", f"{context['group_id']}.json")
        with open(metadata_file, "r") as f:
            metadata = json.load(f)

        if user_id not in metadata.get("encrypted_keys", {}):
            return "You do not have access to this file"

        encrypted_key = metadata["encrypted_keys"][user_id]

        if is_vault:
            file_path = os.path.join("vaults", context["owner_id"], file_id)
        else:
            file_path = os.path.join("vaults", context["group_id"], file_id)

        with open(file_path, "rb") as f:
            encrypted_content = f.read()

        response = {
            "file_name": file_to_send["file_name"],
            "encrypted_content": base64.b64encode(encrypted_content).decode("utf-8"),
            "encrypted_key": encrypted_key
        }

        logging.info("Encrypted file %s sent to %s for client-side decryption", file_id, user_id)
        return response

    except Exception as e:
        logging.error("Error retrieving file %s for user %s: %s", file_id, user_id, str(e))
        return f"Error retrieving file: {str(e)}"
    

        

def revoke_file_user_server(received_msg):
    file_id = received_msg["file_id"]
    user_id = received_msg["user_id"]
    user_id_to_revoke = received_msg["user_id_to_revoke"]
    
    metadata_file = os.path.join("vaults", "metadata", f"{file_id}.json")
    with open(metadata_file, "r") as f:
        metadata = json.load(f)
    
    if metadata["owner_id"] != user_id:
        txt = f"Only the owner can revoke access"
        logging.error("User %s tried to revoke access for file %s but is not the owner", user_id, file_id)
        return txt
    
    if user_id_to_revoke in metadata["encrypted_keys"]:
        del metadata["encrypted_keys"][user_id_to_revoke]
        
        with open(metadata_file, "w") as f:
            json.dump(metadata, f, indent=4)
    
    with open("database.json", "r") as f:
        data = json.load(f)
        vaults = data.get("vaults", [])
        
    for v in vaults:
        if v["owner_id"] == user_id:
            for file in v["files"]:
                if file["file_id"] == file_id:
                    file["permissions"] = [p for p in file["permissions"] if p["user_id"] != user_id_to_revoke]
                    break
    
    with open("database.json", "w") as f:
        json.dump(data, f, indent=4)
    
    txt = f"User {user_id_to_revoke}'s access to file {file_id} revoked"
    logging.info("User %s's access to file %s revoked", user_id_to_revoke, file_id)
    
    response = {
        "message": txt,
    }
    return response   



############################################### GRUPOS ##########################################

def list_files_group_server(received_msg):
    group_id = received_msg["group_id"]
    user_id = received_msg["user_id"]
    
    # Find the group
    with open("database.json", "r") as f:
        data = json.load(f)
        groups = data.get("groups", [])
        group = None
        for g in groups: 
            if g["group_id"] == group_id:
                group = g
                break
        
    if group is None:
        txt = f"Group {group_id} not found"
        logging.error("Group %s not found", group_id)
        return {"message": txt, "files": []}
    
    # Check user permissions
    user_procurado = None
    permissoes = None
    for user in group["users"]:
        if user["user_id"] == user_id:
            user_procurado = user
            permissoes = user["permission"]
            break
    
    if user_procurado is None:
        txt = f"User {user_id} not found in group {group_id}"
        logging.error("User %s not found in group %s", user_id, group_id)
        return {"message": txt, "files": []}
    
    if "R" not in permissoes:
        txt = f"User {user_id} does not have permission to list files from group {group_id}"
        logging.error("User %s does not have permission to list files from group %s", user_id, group_id)
        return {"message": txt, "files": []}

    # Get the files from the group
    files = group.get("files", [])
    
    # Format similar to list_files_user_server
    if len(files) == 0:
        response = {
            "message": f"No files found for group {group['group_id']}",
            "files": []
        }
    else:
        file_list = [(file["file_name"], file["file_id"]) for file in files]
        response = {
            "message": f"Files for group {group['group_id']}",
            "files": file_list
        }
        logging.info("Files for group %s: %s", group["group_id"], file_list)

    return response  

def add_file_group_server(received_msg):    
    user_id = received_msg["user_id"]
    group_id = received_msg["group_id"]
    group = None
    
    with open("database.json", "r") as f:
        data = json.load(f)
        groups = data.get("groups", [])
        for g in groups: 
            if g["group_id"] == group_id:
                group = g
                break
            
    if group is None:
        txt = f"Group {group_id} not found"
        logging.error("Group %s not found", group_id)
        return {"message": txt}
    
    
    user_procurado = None
    permissoes = None
    for user in group["users"]:
        if user["user_id"] == user_id:
            user_procurado = user
            permissoes = user["permission"]
            break
    
    if user_procurado is None:
        txt = f"User {user_id} not found in group {group_id}"
        logging.error("User %s not found in group %s", user_id, group_id)
        return {"message": txt}
    
    if "W" not in permissoes:
        txt = f"User {user_id} does not have permission to add files to group {group_id}"
        logging.error("User %s does not have permission to add files to group %s", user_id, group_id)
        return {"message": txt}

    
    file_name = received_msg["file_name"]
    file_content = received_msg["file_content"]
    file_key = received_msg.get("file_key")  
    
    global list_of_ids_path
    file_id = file_name + "_" + str(uuid.uuid4())[:8]

    
    with open(list_of_ids_path, "r") as f:
        lista_ids = json.load(f)
        lista_ids["ids"] = lista_ids.get("ids", [])
        while file_id in lista_ids["ids"]:
            file_id = file_name + "_" + str(uuid.uuid4())[:8]
            logging.info("File ID %s already exists, generating a new one", file_id)
        lista_ids["ids"].append(file_id)

    with open(list_of_ids_path, "w") as f:
        json.dump(lista_ids, f, indent=4)
    
    
    user_dir = os.path.join("vaults", group_id)
    os.makedirs(user_dir, exist_ok=True)

    
    file_path = os.path.join(user_dir, file_id)
    with open(file_path, "wb") as f:
        f.write(file_content)

    logging.info("File %s saved to %s", file_name, file_path)

    
    encrypted_keys = {group["owner_id"]: file_key}
    store_file_metadata(file_id, encrypted_keys, group["owner_id"])
    
    
    permissions = []
    for user in group["users"]:
        permissions.append({"user_id": user["user_id"], "permission": user["permission"]})
    
    
    file_entry = {
        "file_id": file_id,
        "file_name": file_name,
        "permissions": permissions
    }

    
    add_group_file_database(file_entry, group_id)
    txt = f"File {file_name} added with ID {file_id} to group {group['group_name']}"
    logging.info("File added: %s", file_entry)
    
    return {"message": txt}


def delete_group_server(received_msg):
    user_id = received_msg["user_id"]
    group_id = received_msg["group_id"]

    with open("database.json", "r") as f:
        data = json.load(f)
        groups = data.get("groups", [])

    relevant_group = None
    for g in groups:
       if g["group_id"] == group_id:
            relevant_group = g
            break

    if relevant_group is None:
        response = {
            "message": f"Group {group_id} does not exist"
        }
        logging.error("Group %s search has failed, group does not exist", group_id)
        return response

    if relevant_group["owner_id"] != user_id:
        response = {
            "message": f"User {user_id} does not have permission to delete group {group_id}"
        }
        logging.error("User %s does not have permission to delete group %s", user_id, group_id)
        return response

    
    groups.remove(relevant_group)

    
    group_dir = os.path.join("vaults", group_id)  
    if os.path.exists(group_dir):
        shutil.rmtree(group_dir)
        logging.info("Directory %s deleted", group_dir)
    else:
        logging.warning("Directory %s does not exist", group_dir)

    
    metadata_dir = os.path.join("vaults", "metadata")
    for file in relevant_group.get("files", []):
        file_id = file["file_id"]
        metadata_file = os.path.join(metadata_dir, f"{file_id}.json")
        if os.path.exists(metadata_file):
            os.remove(metadata_file)
            logging.info("Metadata file for %s removed", file_id)

    os.remove(os.path.join(metadata_dir, f"{group_id}.json"))
    
    with open("database.json", "w") as f:
        json.dump(data, f, indent=4)

    
    with open(list_of_ids_path, "r") as f:
        lista_ids = json.load(f)
        if group_id in lista_ids["ids"]:
            lista_ids["ids"].remove(group_id)
            logging.info("Group ID %s removed from list of IDs", group_id)
        lista_ids["ids"] = lista_ids.get("ids", [])
        for file in relevant_group.get("files", []):
            file_id = file["file_id"]
            if file_id in lista_ids["ids"]:
                lista_ids["ids"].remove(file_id)
                logging.info("File ID %s removed from list of IDs", file_id)
            else:
                logging.warning("File ID %s not found in list of IDs", file_id)



    with open(list_of_ids_path, "w") as f:
        json.dump(lista_ids, f, indent=4)

    response = {
        "message": f"Group {group_id} deleted"
    }
    logging.info("Group %s deleted", group_id)
    return response 




def create_group_server(received_msg):
        
        group_name = received_msg["group_name"]
        owner_id = received_msg["owner_id"]
        group_key = received_msg["group_key"]


        global list_of_ids_path
        group_id = group_name + "_" + str(uuid.uuid4())[:8]

        with open(list_of_ids_path, "r") as f:
                lista_ids = json.load(f)
                lista_ids["ids"] = lista_ids.get("ids", [])
                while group_id in lista_ids:
                    group_id = group_name + "_" + str(uuid.uuid4())[:8]
                    logging.info("Group ID %s already exists, generating a new one", group_id)
                lista_ids["ids"].append(group_id)            
        with open(list_of_ids_path, "w") as f:
            json.dump(lista_ids, f, indent=4)

        
        encrypted_keys = {owner_id: group_key}
        store_file_metadata(group_id, encrypted_keys, owner_id)
            
        user_dir = os.path.join("vaults", group_id)
        os.makedirs(user_dir, exist_ok=True)

        logging.info("Group vault for %s(%s) created", group_id , group_id)


        users = [
                {"user_id": owner_id, "permission": "RW"}
            ]

        group_entry = {
                "group_id": group_id,
                "group_name": group_name,
                "owner_id": owner_id,
                "users": users,
                "files": []
            }

        add_group_database(group_entry)
        txt = f"Group {group_name} added with ID {group_id}"
        logging.info("Group created successfully: %s (%s) by user %s" , group_name, group_id, owner_id)

        response = {
            "message": txt,
        }
        return response   



def delete_user_group_server(received_msg):
    user_id = received_msg["user_id"]
    group_id = received_msg["group_id"]
    user_id_to_delete = received_msg["user_id_to_delete"]

    with open("database.json", "r") as f:
        data = json.load(f)
        groups = data.get("groups", [])

    relevant_group = None
    for g in groups:
        if g["group_id"] == group_id:
            relevant_group = g
            break
    
    if relevant_group is None:
        txt = f"Group {group_id} does not exist"
        logging.error("Group %s search has failed, group does not exist", group_id)
        return txt
    
    if relevant_group["owner_id"] != user_id:
        txt = f"User {user_id} does not have permission to delete users from group {group_id}"
        logging.error("User %s does not have permission to delete users from group %s", user_id, group_id)
        return txt
    
    for u in relevant_group["users"]:
        if u["user_id"] == user_id_to_delete:
            relevant_group["users"].remove(u)
            break

    with open("database.json", "w") as f:
        json.dump(data, f, indent=4)


    metadata_file = os.path.join("vaults", "metadata", f"{group_id}.json")
    with open(metadata_file, "r") as f:
        metadata = json.load(f)
        if user_id_to_delete in metadata["encrypted_keys"]:
            del metadata["encrypted_keys"][user_id_to_delete]
            with open("metadata.json", "w") as f:
                json.dump(metadata, f, indent=4)

    txt = f"User {user_id_to_delete} deleted from group {group_id}"
    logging.info("User %s deleted from group %s", user_id_to_delete, group_id)
    
    response = {
        "message": txt,
    }
    return response   

def add_user_group_server(received_msg):
    user_id = received_msg["user_id"]
    group_id = received_msg["group_id"]
    user_id_to_add = received_msg["user_id_to_add"]
    permissions = received_msg["permissions"]
    file_key = received_msg.get("file_key")  # Get file keys if provided

    with open("database.json", "r") as f:
        data = json.load(f)
        groups = data.get("groups", [])

    relevant_group = None
    for g in groups:
        if g["group_id"] == group_id:
            relevant_group = g
            break
    
    if relevant_group is None:
        txt = f"Group {group_id} does not exist"
        logging.error("Group %s search has failed, group does not exist", group_id)
        return {"message": txt}
    
    if relevant_group["owner_id"] != user_id:
        txt = f"User {user_id} does not have permission to add users to group {group_id}"
        logging.error("User %s does not have permission to add users to group %s", user_id, group_id)
        return {"message": txt}

    
    user_exists = False
    for u in relevant_group["users"]:
        if u["user_id"] == user_id_to_add:
            
            old_permissions = u["permission"]
            
            new_permissions = "".join(sorted(set(old_permissions + permissions)))
            u["permission"] = new_permissions
            user_exists = True
            txt = f"User {user_id_to_add}'s permissions in group {group_id} updated from '{old_permissions}' to '{new_permissions}'"
            logging.info("User %s permissions in group %s updated from '%s' to '%s'", 
                         user_id_to_add, group_id, old_permissions, new_permissions)
            break
    
    
    if not user_exists:
        relevant_group["users"].append({"user_id": user_id_to_add, "permission": permissions})
        txt = f"User {user_id_to_add} added to group {group_id} with permission {permissions}"
        logging.info("User %s added to group %s with permission %s", user_id_to_add, group_id, permissions)

    
    metadata_file = os.path.join("vaults", "metadata", f"{group_id}.json")
    with open(metadata_file, "r") as f:
        metadata = json.load(f)

    if metadata["owner_id"] != user_id:
        txt = f"Only the owner can share files"
        logging.error("User %s tried to share file %s but is not the owner", user_id, group_id)
        return {"message": txt}

    if user_id_to_add not in metadata["encrypted_keys"] and file_key:
        metadata["encrypted_keys"][user_id_to_add] = base64.b64encode(file_key).decode('utf-8')
        with open(metadata_file, "w") as f:
            json.dump(metadata, f, indent=4)

    
    with open("database.json", "w") as f:
        json.dump(data, f, indent=4)

    response = {
        "message": txt
    }
    return response


def group_list_server(received_msg):
    user_id = received_msg["user_id"]
    groups_to_list = []
    
    with open("database.json", "r") as f:
        data = json.load(f)
        groups = data.get("groups", [])

    for g in groups:
        for user in g["users"]:
            if user["user_id"] == user_id:
                groups_to_list.append({"group_name": g["group_name"], "group_id": g["group_id"],"owner_id": g["owner_id"]})
                break

    if not groups_to_list: 
        txt = f"No groups found for user {user_id}"
        logging.error("No groups found for user %s", user_id)
    else:
        formatted_groups = [f"\n{group['group_name']} (ID: {group['group_id']} owner: {group['owner_id']})" for group in groups_to_list]
        txt = f"\nGroups for user {user_id}: {', '.join(formatted_groups)}"
        logging.info("Groups for user %s: %s", user_id, formatted_groups)
    
    response = {
        "message": txt,
    }
    return response










