import asyncio
import sys
import logging
import os
import bson

from cryptography.x509.oid import ObjectIdentifier


sys.path.append(os.path.abspath(os.path.join(os.path.dirname("Auxiliares.py"), "..")))
from Auxiliares import *
from server_commands import *
from authenticate import decode_msg, encode_msg
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import dh, padding
from cryptography.hazmat.primitives.serialization import pkcs12
from cryptography import x509


logging.basicConfig(
    filename="server_logs.txt",  
    level=logging.INFO,          
    format="%(asctime)s - %(levelname)s - %(message)s",  
    datefmt="%Y-%m-%d %H:%M:%S"  
)

shared_key_global = None

conn_cnt = 0
conn_port = 7777
DELIMITER = b'\x00\x00<END>\x00\x00'



class ServerWorker(object):
    """ Classe que implementa a funcionalidade do SERVIDOR. """
    def __init__(self, cnt, addr=None):
        """ Construtor da classe. """
        self.id = cnt
        self.addr = addr
        self.msg_cnt = 0
        self.password = None
        
    def process(self, msg):
        self.msg_cnt += 1


        txt = decode_msg(msg,self.password)
        logging.info("Received message: %s", txt)
        received_msg = bson.BSON.decode(txt)
        
        command = received_msg["command"]
        
        if command == "ADD-FILE":
            txt = add_file_user_server(received_msg)

        elif command == "SHARE-FILE":
            txt = share_file_user_server(received_msg)

        elif command == "LIST-FILES":
            txt = list_files_user_server(received_msg)

        elif command == "DELETE-FILE":
            txt = delete_file_user_server(received_msg)

        elif command == "REPLACE-FILE":
            txt = replace_file_user_server(received_msg)
        
        elif command == "FILE-READ":
            txt = read_file_user_server(received_msg)
        
        elif command == "FILE-DETAILS":
            txt = details_file_user_server(received_msg)
        
        elif command == "FILE-METADATA":
            txt = metadata_file_user_server(received_msg)

        elif command == "REVOKE-FILE":
            txt = revoke_file_user_server(received_msg)

        elif command == "LIST-FILES-GROUP":
            txt = list_files_group_server(received_msg)
        
        elif command == "CREATE-GROUP":
            txt = create_group_server(received_msg)

        elif command == "ADD-FILE-GROUP":
            txt = add_file_group_server(received_msg)

        elif command == "DELETE-GROUP":
            txt = delete_group_server(received_msg)

        elif command == "ADD-USER-GROUP":
            txt = add_user_group_server(received_msg)
        
        elif command == "DELETE-USER-GROUP":
            txt = delete_user_group_server(received_msg)

        elif command == "GROUP-LIST":
            txt = group_list_server(received_msg)

        else:
            txt = "Command not recognized"
            logging.error("Command not recognized: %s", command)

        if not isinstance(txt, dict):
            new_msg = bson.BSON.encode({"Error": txt})
            return encode_msg(new_msg,self.password)

        new_msg = encode_msg(bson.BSON.encode(txt),self.password)
        return new_msg 

    

    
    
    

  
async def handle_echo(reader, writer):
    
    logging.info("Generating Parameters DH...")
    parameters = dh.generate_parameters(generator=2, key_size=512)
    logging.info("Parameters DH gerados.")

    global conn_cnt
    conn_cnt += 1
    addr = writer.get_extra_info('peername')
    srvwrk = ServerWorker(conn_cnt, addr)

    
    

    server_private_key = parameters.generate_private_key()

    server_public_bytes = server_private_key.public_key().public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

    parameters_pem = parameters.parameter_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.ParameterFormat.PKCS3
    )


    writer.write(b"PARAMS: " + parameters_pem)
    await writer.drain()
    logging.info("Parameters DH Sent.")

    await handshake(server_public_bytes, writer, reader, server_private_key)
    logging.info("Handshake Done.")

    srvwrk.password = base64.b64encode(shared_key_global).decode('utf-8')
    
    while True:
        msg_length = await reader.readexactly(16)
        lenght = msg_length.decode('utf-8').strip()
        if lenght == "0":
            break
        while True:
            next_byte = await reader.readexactly(1)
            if next_byte != b' ':  # Filtrar vazios (primeira mensagem tem 1 vazio sempre)
                break
        msg = await reader.readexactly(int(lenght) - 1)
        msg = next_byte + msg


        msg = srvwrk.process(msg)
        writer.write(msg + DELIMITER)
        await writer.drain()
    
    logging.info("Connection closed by client %s", addr)
    writer.close()



async def handshake(server_public_bytes,writer, reader, my_private_key):
    global shared_key_global
    peer_public_bytes = await reader.readuntil(b"-----END PUBLIC KEY-----\n")

    logging.info("Received Client Public Key")

    with open("../Certificados/VAULT_SERVER.p12", "rb") as p12_file:  
        p12_data = p12_file.read()


    p12_private_key, certificate, extras = pkcs12.load_key_and_certificates(p12_data, b"")
    
    par_de_chaves = mkpair(server_public_bytes, peer_public_bytes)
    signature = p12_private_key.sign(
        par_de_chaves,
        padding.PSS(
            mgf=padding.MGF1(hashes.SHA256()),
            salt_length=padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )
    

    certificate_bytes = certificate.public_bytes(
        encoding=serialization.Encoding.PEM,  
        )
    
    certificate_bytes = certificate_bytes
    


    writer.write(server_public_bytes + signature + DELIMITER + certificate_bytes)
    logging.info("Sent Server Public Key and Signature and Certificate")

    await writer.drain()

    peer_public_key = serialization.load_pem_public_key(peer_public_bytes)
    shared_key = my_private_key.exchange(peer_public_key)
    shared_key_global = shared_key
    
    client_signature = await reader.readuntil(DELIMITER)
    client_signature = client_signature[:-len(DELIMITER)]

    par_de_chaves_check = mkpair(peer_public_bytes,server_public_bytes)

    client_certificate_bytes = await reader.readuntil(b"-----END CERTIFICATE-----")  # Lê o certificado do cliente


    client_certificate = x509.load_pem_x509_certificate(client_certificate_bytes)
    

    with open("../Certificados/Client.crt", "wb") as ficheiro:
        ficheiro.write(client_certificate.public_bytes(encoding=serialization.Encoding.PEM))

    valida_certALICE("../Certificados/VAULT_CA.crt",file_name = "../Certificados/Client.crt",common_name="User 1 (SSI Vault Client 1)")
    
    public_key_certificate = client_certificate.public_key()

    
    public_key_certificate.verify(
        client_signature,
        par_de_chaves_check,
        padding.PSS(
            mgf=padding.MGF1(hashes.SHA256()),
            salt_length=padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )

    subject = client_certificate.subject
    user_id = None
    for attribute in subject:
        if attribute.oid == x509.oid.NameOID.PSEUDONYM:
            user_id = attribute.value
            break
    
    if user_id:
        public_key_pem = client_certificate.public_key().public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
        ).decode()

        add_public_key_database(user_id, public_key_pem)
        logging.info(f"Added public key for user: {user_id}")
    else:
        logging.warning("Could not extract user_id from certificate")


    
def run_server():
    async def main():
        server = await asyncio.start_server(handle_echo, '127.0.0.1', conn_port)
        print('Serving on {}'.format(server.sockets[0].getsockname()))
        print('  (type ^C to finish)\n')

        logging.info('Server started and listening on %s', server.sockets[0].getsockname())
            

        try:
            await server.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped by user.")
        finally:
            server.close()
            await server.wait_closed()
            print('\nFINISHED!')
    asyncio.run(main())




run_server()