# Código baseado em https://docs.python.org/3.6/library/asyncio-stream.html#tcp-echo-client-using-streams
import asyncio
import socket
from authenticate import derive_key, decode_msg, encode_msg
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import dh
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.serialization import load_pem_parameters


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
         ##   print('Received message: %r' % msg)
            if len(msg) < 28:
                print("Received message is too short!")
            else:
                if msg.endswith(DELIMITER):  # Check if the message ends with the DELIMITER
                    msg = msg.replace(DELIMITER, b"")  # Replace the DELIMITER with an empty byte string
                received_msg = (decode_msg(msg,password))
                print('Received (%d): %r' % (self.msg_cnt ,received_msg))
            
        print('Input message to send (empty to finish)')
        new_msg = input()
        if new_msg == "":
            return None
        msg_to_send = encode_msg(new_msg.encode(),password)
        return msg_to_send if len(new_msg)>0 else None

        
        



#
#
# Funcionalidade Cliente/Servidor
#
# obs: não deverá ser necessário alterar o que se segue
#
 
async def handshake(writer, reader):
    
    
    print("Handshake iniciado...")
    params_data = await reader.readuntil(b"-----END DH PARAMETERS-----")  # Lê até o final dos parâmetros DH
    

    print("RECEBIDO (parâmetros adicionais):\n", params_data.decode())

    if b"PARAMS:" in params_data:

        params_str = params_data.decode().replace("PARAMS: ", "")  #
        parameters = load_pem_parameters(params_str.encode())

        peer_private_key = parameters.generate_private_key()

        # Converte a chave privada para bytes PEM
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
        print("RECEBIDO (chave pública do servidor):\n", server_public_key_bytes.decode())

        # Converte a chave pública recebida para o formato correto
        server_public_key = serialization.load_pem_public_key(server_public_key_bytes)
        

        # Troca a chave e obtém o 'shared_key'
        shared_key = peer_private_key.exchange(server_public_key)
        print("Shared Key:", shared_key)




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
            msg = msg[1:]  # Remove o primeiro byte se for igual a 10 (newline)
        ##    print('Received message: %r' % msg)
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