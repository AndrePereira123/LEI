
import asyncio
import os
import sys
import logging

import bson
sys.path.append(os.path.abspath(os.path.join(os.path.dirname("Auxiliares.py"), "..")))
from Auxiliares import *
from authenticate import decode_msg, encode_msg
from client_commands import *
from cryptography.hazmat.primitives import hashes, serialization
from cryptography import x509
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives.serialization import load_pem_parameters
from cryptography.hazmat.primitives.serialization import pkcs12
from client_commands import client_execute, get_user_id



BLACK = "\033[30m"
RED = "\033[31m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
BLUE = "\033[34m"
MAGENTA = "\033[35m"
CYAN = "\033[36m"
WHITE = "\033[37m"

RESET = "\033[0m" 

logging.basicConfig(
    filename="client_logs.txt",  
    level=logging.INFO,          
    format="%(asctime)s - %(levelname)s - %(message)s",  
    datefmt="%Y-%m-%d %H:%M:%S"  
)


conn_port = 7777
shared_key_global = None
DELIMITER = b'\x00\x00<END>\x00\x00'
user_id = None

class Client:
    
    def __init__(self, sckt=None):
        global user_id

        if len(sys.argv) != 2:
            print(RED)
            print("Path to certificate is mandatory")
            print(RESET)
            logging.error("User tryed to run the client without a certificate path")
            return
        
        path_to_file = sys.argv[1]

        self.sckt = sckt
        self.msg_cnt = 0
        
        user_id = get_user_id(path_to_file)
        with open(path_to_file, "rb") as p12_file:  
            p12_data = p12_file.read()
        self.private_key, self.certificate, _ = pkcs12.load_key_and_certificates(p12_data, b"")
        self.password = None

    async def process(self, writer, reader , msg=b""):
        global user_id

        self.msg_cnt +=1
        
        if (msg != b""): 
                
            if msg.endswith(DELIMITER): 
                msg = msg.replace(DELIMITER, b"") 
            received_msg = (decode_msg(msg,self.password))
            dicionario_recebido = bson.BSON.decode(received_msg)
            if (dicionario_recebido.get("encrypted_key") != None):
                dicionario_recebido = handle_file_details_response(dicionario_recebido, self.private_key)

            texto_para_logging = ""
            for key, value in dicionario_recebido.items():
                print(GREEN + "\n" + "-" * 40 + YELLOW + f"Section: \"{key}\"" + GREEN +"-" * 40 + "\n" + RESET)
                texto_para_logging += "\n" + "-" * 40 + f"Section: \"{key}\"" + "-" * 40 + "\n"
                if isinstance(value, list):
                    print("\n".join(f"- {item}" for item in value))
                    texto_para_logging += "\n".join(f"- {item}" for item in value)
                elif isinstance(value, dict):
                    for sub_key, sub_value in value.items():
                        print(f"  {sub_key}: {sub_value}")
                        texto_para_logging += f"  {sub_key}: {sub_value}"
                else:
                    print(value)
                    texto_para_logging += value
            print(GREEN + "\n" + "-" * 40 + BLUE + f"End of message" + GREEN + "-" * 40 + "\n" + RESET)
            texto_para_logging += "\n" + "-" * 40 + f"End of message" + "-" * 40 + "\n"

            logging.info("Received message: %s", texto_para_logging)

        parts = ""
        msg_to_send = "Command not recognized"
        while (msg_to_send == "Command not recognized"):
            parts = ""
            while (len(parts) <= 1):
                print(YELLOW + "Your User ID: " + user_id)
                print('Input message to send')
                print(RESET)
                new_msg = input()
                logging.info("User input: %s", new_msg)
                if new_msg == "exit":
                    print(BLUE + "Exiting the program..." + RESET)
                    return None
                
                parts = new_msg.split(" ")
                if len(parts) <= 1 :
                    print(RED + "Incomplete command!" + RESET)
                    logging.error("Incomplete command input by user")

            
            command = parts[0]
            args = parts[1:]
            msg_to_send = await client_execute(command, user_id, args, self.private_key,writer,reader,self.password)
            if msg_to_send == "Command not recognized":
                print(RED + "Command not recognized" + RESET)
                logging.error("Command not recognized: %s", command)


        msg_to_send_bytes = encode_msg(msg_to_send,self.password)
        return msg_to_send_bytes if len(new_msg)>0 else None

        
        

 
async def handshake(writer, reader):
    global shared_key_global
    
    params_data = await reader.readuntil(b"-----END DH PARAMETERS-----\n") 
    


    if b"PARAMS:" in params_data:

        params_str = params_data.decode().replace("PARAMS: ", "")  #
        parameters = load_pem_parameters(params_str.encode())

        peer_private_key = parameters.generate_private_key()

        
        peer_public_bytes = peer_private_key.public_key().public_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PublicFormat.SubjectPublicKeyInfo
        )


        # Enviar a chave pública do cliente para o servidor
        writer.write(peer_public_bytes)
        await writer.drain()

        # Ler a chave pública do servidor
        server_public_key_bytes = await reader.readuntil(b"-----END PUBLIC KEY-----\n")  # Lê a chave pública do servidor
        server_signature = await reader.readuntil(DELIMITER)
        server_signature = server_signature[:-len(DELIMITER)]  
        server_certificate_bytes = await reader.readuntil(b"-----END CERTIFICATE-----")  # Lê o certificado do servidor


        server_public_key = serialization.load_pem_public_key(server_public_key_bytes)

        par_de_chaves_check = mkpair(server_public_key_bytes,peer_public_bytes)


        server_certificate = x509.load_pem_x509_certificate(server_certificate_bytes)

    
        with open("../Certificados/Server.crt", "wb") as ficheiro:
            ficheiro.write(server_certificate.public_bytes(encoding=serialization.Encoding.PEM))


        valida_certALICE("../Certificados/VAULT_CA.crt",file_name = "../Certificados/Server.crt",common_name="SSI Vault Server")  
        
        ##########     3 PASSO       #####################################################################################

        public_key_certificate = server_certificate.public_key()
        

        public_key_certificate.verify(
        server_signature,
        par_de_chaves_check,
        padding.PSS(
            mgf=padding.MGF1(hashes.SHA256()),
            salt_length=padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
        )
                
        with open(sys.argv[1], "rb") as p12_file:  
            p12_data = p12_file.read()

    
        p12_private_key, certificate , extras = pkcs12.load_key_and_certificates(p12_data, b"")

        par_de_chaves = mkpair(peer_public_bytes, server_public_key_bytes)
        signature = p12_private_key.sign(
            par_de_chaves,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )
        
      
        certificate_bytes = certificate.public_bytes(
            encoding=serialization.Encoding.PEM,  # Use PEM encoding
            )

        
        
        writer.write(signature + DELIMITER + certificate_bytes )  # Envia a assinatura e o certificado do cliente

        await writer.drain()

        
        shared_key = peer_private_key.exchange(server_public_key)
        shared_key_global = shared_key

        ###############################################################################################################

async def tcp_echo_client():
    reader, writer = await asyncio.open_connection('127.0.0.1', conn_port)
    addr = writer.get_extra_info('peername')
    client = Client(addr)
    try:
        await handshake(writer, reader)
        
        if shared_key_global is None:
            print(RED + "Error: Failed to establish a shared key with the server" + RESET)
            logging.error("Failed to establish a shared key with the server")
            writer.close()
            await writer.wait_closed()
            return
            
        client.password = base64.b64encode(shared_key_global).decode('utf-8')
        msg = await client.process(writer,reader)
        while msg:
            writer.write(str(len(msg)).encode('utf-8').ljust(16,b' ') + msg)
            await writer.drain()
            print(GREEN + "Message sent to server" + RESET)
            logging.info('Waiting answer!')
            try:
                msg = await reader.readuntil(DELIMITER)
            except asyncio.IncompleteReadError:
                print(RED + "Connection closed by server" + RESET)
                logging.info("Connection closed by server")
                return

            if msg[0] == 10: 
                msg = msg[1:]  
            if msg:
                msg = await client.process(writer,reader,msg)
                
            else:
                break
        writer.write("0".encode('utf-8').ljust(16,b' '))
        print(GREEN + "Socket closed!" + RESET)
        logging.info('Socket closed!')
    except Exception as e:
        print(RED + "An error occurred: " + str(e) + RESET)
        logging.error(f"An error occurred: {e}")
    finally:
        writer.close()
        await writer.wait_closed()

def run_client():
    if len(sys.argv) != 2:
            print(RED)
            print("Path to certificate is mandatory")
            print(RESET)
            logging.error("User tryed to run the client without a certificate path!")
            return
    
    asyncio.run(tcp_echo_client())


run_client()