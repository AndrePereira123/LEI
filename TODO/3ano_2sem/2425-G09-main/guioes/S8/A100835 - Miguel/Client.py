# Código baseado em https://docs.python.org/3.6/library/asyncio-stream.html#tcp-echo-client-using-streams
import asyncio
import socket
from Auxiliares import *
from authenticate import derive_key, decode_msg, encode_msg
from cryptography.hazmat.primitives import hashes, serialization
from cryptography import x509
from cryptography.hazmat.primitives.asymmetric import dh, padding
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.serialization import load_pem_parameters
from cryptography.hazmat.primitives.serialization import pkcs12




conn_port = 7777
password = "teste"
DELIMITER = b'<END>'

class Client:
    
    def __init__(self, sckt=None):
        
        self.sckt = sckt
        self.msg_cnt = 0
    def process(self, msg=b""):
        
        self.msg_cnt +=1
        
        if (msg != b""):
            print('Received bytes with ' + str(len(msg)) + " length")
         
            if len(msg) < 28:
                print("Received message is too short!")
            else:
                if msg.endswith(DELIMITER): 
                    msg = msg.replace(DELIMITER, b"") 
                received_msg = (decode_msg(msg,password))
                print('Received (%d): %r' % (self.msg_cnt ,received_msg))
            
        print('Input message to send (empty to finish)')
        new_msg = input()
        if new_msg == "":
            return None
        msg_to_send = encode_msg(new_msg.encode(),password)
        return msg_to_send if len(new_msg)>0 else None

        
        

 
async def handshake(writer, reader):
    
    
    print("Handshake iniciado...")
    params_data = await reader.readuntil(b"-----END DH PARAMETERS-----") 
    

    print("RECEBIDO (parâmetros adicionais):\n", params_data.decode())

    if b"PARAMS:" in params_data:

        params_str = params_data.decode().replace("PARAMS: ", "")  #
        parameters = load_pem_parameters(params_str.encode())

        peer_private_key = parameters.generate_private_key()

        
        peer_public_bytes = peer_private_key.public_key().public_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PublicFormat.SubjectPublicKeyInfo
        )

        print("----------------------------------------------------")
        print("Chave pública do cliente:", peer_public_bytes.decode())
        print("----------------------------------------------------")


        # Enviar a chave pública do cliente para o servidor
        writer.write(peer_public_bytes)
        await writer.drain()

        # Ler a chave pública do servidor
        server_public_key_bytes = await reader.readuntil(b"-----END PUBLIC KEY-----")  # Lê a chave pública do servidor
        server_signature = await reader.readuntil(DELIMITER)
        server_certificate_bytes = await reader.readuntil(b"-----END CERTIFICATE-----")  # Lê o certificado do servidor
        extras_with_delimiter = await reader.readuntil(DELIMITER)
        extras = extras_with_delimiter[:-len(DELIMITER)]
        print("\n\n SERVER PUBLIC KEY: \n")
        print(server_public_key_bytes)
        print("\n\n SERVER SIGNATURE: \n")
        print(server_signature)
        print("\n\n SERVER CERTIFICATE: \n")
        print(server_certificate_bytes)
        print("\n\n EXTRAS: \n")   
        print(extras)

        print("RECEBIDO (chave pública do servidor):\n", server_public_key_bytes.decode())

        # Converte a chave pública recebida para o formato correto
        server_public_key = serialization.load_pem_public_key(server_public_key_bytes)

       
        server_certificate = x509.load_pem_x509_certificate(server_certificate_bytes)

        
        with open("Server.crt", "wb") as ficheiro:
            ficheiro.write(extras)
            ficheiro.write(server_certificate.public_bytes(encoding=serialization.Encoding.PEM))



        ## AQUI TEMOS QUE MUDAR O COMMON NAME PORQUE ELE e alterado nao sabemos o porquê dele alterar -------------
        ##valida_certALICE("VAULT_CA.crt",file_name = "Server.crt",common_name="SSI VAULT SERVICE CA")
        valida_certALICE("VAULT_CA.crt",file_name = "Server.crt",common_name="SSI Vault Server")  
        
        ##########     3 PASSO       #####################################################################################
        
        with open("VAULT_CLI1.p12", "rb") as p12_file:  
            p12_data = p12_file.read()

    
        p12_private_key, certificate, extras = pkcs12.load_key_and_certificates(p12_data, b"")

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
        
        extras_info = ""
        for extra_cert in extras:
            subject = extra_cert.subject.rfc4514_string()  
            issuer = extra_cert.issuer.rfc4514_string()   
            extras_info += f"subject: {subject}\nissuer: {issuer}\n"

        
        extras_bytes = extras_info.encode("utf-8")
        
        writer.write(signature + DELIMITER + certificate_bytes + extras_bytes + DELIMITER)  # Envia a assinatura e o certificado do cliente

        await writer.drain()

        ##global password
        ##password = shared_key

        
        # Troca a chave e obtém o 'shared_key'
        shared_key = peer_private_key.exchange(server_public_key)
        print("Shared Key:", shared_key)

        ###############################################################################################################

async def tcp_echo_client():
    reader, writer = await asyncio.open_connection('127.0.0.1', conn_port)
    addr = writer.get_extra_info('peername')
    client = Client(addr)
    
    await handshake(writer,reader)
    
    msg = client.process()

    while msg:
       ## print('Sending: %r' % msg)
        writer.write(msg + DELIMITER) 
        await writer.drain()
        print('Waiting answer!')
        msg = await reader.readuntil(DELIMITER)
        if msg[0] == 10: 
            msg = msg[1:]  
        if msg:
            msg = client.process(msg)
        else:
            break
    writer.write(DELIMITER)
    print('Socket closed!')
    writer.close()

def run_client():
    asyncio.run(tcp_echo_client())


run_client()