# Código baseado em https://docs.python.org/3.6/library/asyncio-stream.html#tcp-echo-client-using-streams
import asyncio
from authenticate import derive_key, decode_msg, encode_msg
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import dh
from cryptography.hazmat.primitives.kdf.hkdf import HKDF

conn_cnt = 0
conn_port = 7777
password = "teste"
DELIMITER = b'<END>'

class ServerWorker(object):
    """ Classe que implementa a funcionalidade do SERVIDOR. """
    def __init__(self, cnt, addr=None):
        """ Construtor da classe. """
        self.id = cnt
        self.addr = addr
        self.msg_cnt = 0
    def process(self, msg):
        
        self.msg_cnt += 1


        txt = (decode_msg(msg,password)).decode()
        print('%d : %r' % (self.id,txt))

        new_msg = encode_msg((txt.upper()).encode(),password)
        print('Message to send (%d) : %r' % (self.id,(txt.upper())))
        return new_msg if len(new_msg)>0 else None


#
#
# Funcionalidade Cliente/Servidor
#
# obs: não deverá ser necessário alterar o que se segue
#


async def handle_echo(reader, writer):
    
    print("Generating Parameters DH...")
    parameters = dh.generate_parameters(generator=2, key_size=512)
    print("Parameters DH gerados.")

    global conn_cnt
    conn_cnt += 1
    addr = writer.get_extra_info('peername')
    srvwrk = ServerWorker(conn_cnt, addr)

    # Gerar parâmetros DH (exemplo com seed fixa, se for necessário)
    

    server_private_key = parameters.generate_private_key()

    # Obter chave pública do servidor em formato PEM
    server_public_bytes = server_private_key.public_key().public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

    parameters_pem = parameters.parameter_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.ParameterFormat.PKCS3
    )

    # Enviar os parâmetros DH primeiro
    print(parameters_pem.decode())
    writer.write(b"PARAMS: " + parameters_pem)
    await writer.drain()
    print("Parameters DH Sent.")

    # Chama o handshake para a troca da chave compartilhada
    await handshake(server_public_bytes, writer, reader, server_private_key)

    print("Handshake Done.")
    
    while True:
        msg = await reader.readuntil(DELIMITER)
       ## print('Received bytes with ' + str(len(msg)) + " length")
       ## print('Received message: %r' % msg)
        if msg.endswith(DELIMITER):  # Check if the message ends with the DELIMITER
            msg = msg.replace(DELIMITER, b"")  # Replace the DELIMITER with an empty byte string
        if not msg: continue
        if msg[:1] == DELIMITER: break
        msg = srvwrk.process(msg)
       ## print("Data to send: ")
       ## print(msg)
        writer.write(msg + DELIMITER)
        await writer.drain()
        if not msg: break
    print("[%d]" % srvwrk.id)
    
    writer.close()


async def handshake(server_public_bytes,writer, reader, private_key):
    # Lê os bytes da chave pública do cliente até ao fim do PEM
    peer_public_bytes = await reader.readuntil(b"-----END PUBLIC KEY-----\n")

    # Envia chave pública do servidor
    print("Sending Server Public Key...")
    print("Server Public Key:", server_public_bytes.decode())
    writer.write(server_public_bytes)
    await writer.drain()

    # Converte bytes para chave pública
    peer_public_key = serialization.load_pem_public_key(peer_public_bytes)

    # Gera a chave partilhada
    shared_key = private_key.exchange(peer_public_key)
    print("Shared key (server):", shared_key)

   


    
def run_server():
    async def main():
        server = await asyncio.start_server(handle_echo, '127.0.0.1', conn_port)
        print('Serving on {}'.format(server.sockets[0].getsockname()))
        print('  (type ^C to finish)\n')

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