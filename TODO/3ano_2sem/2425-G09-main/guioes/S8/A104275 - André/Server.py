# Código baseado em https://docs.python.org/3.6/library/asyncio-stream.html#tcp-echo-client-using-streams
import asyncio
from Auxiliares import *
from authenticate import derive_key, decode_msg, encode_msg
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import dh, padding
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.serialization import pkcs12



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
        if msg.endswith(DELIMITER):  
            msg = msg.replace(DELIMITER, b"") 
        if not msg: continue
        if msg[:1] == DELIMITER: break
        msg = srvwrk.process(msg)
        writer.write(msg + DELIMITER)
        await writer.drain()
        if not msg: break
    print("[%d]" % srvwrk.id)
    
    writer.close()


async def handshake(server_public_bytes,writer, reader, my_private_key):
    # Lê os bytes da chave pública do cliente até ao fim do PEM
    peer_public_bytes = await reader.readuntil(b"-----END PUBLIC KEY-----\n")

    # Envia chave pública do servidor
    print("Sending Server Public Key...")
    print("Server Public Key:", server_public_bytes.decode())


    with open("VAULT_SERVER.p12", "rb") as p12_file:  
        p12_data = p12_file.read()

    
    p12_private_key, certificate, extras = pkcs12.load_key_and_certificates(p12_data, b"")
    
    extras_info = ""
    for extra_cert in extras:
        subject = extra_cert.subject.rfc4514_string()  
        issuer = extra_cert.issuer.rfc4514_string()   
        extras_info += f"subject: {subject}\nissuer: {issuer}\n"

    
    extras_bytes = extras_info.encode("utf-8")
    

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
    
    writer.write(server_public_bytes + signature + DELIMITER + certificate_bytes + extras_bytes + DELIMITER)

    await writer.drain()

    # Converte bytes para chave pública
    peer_public_key = serialization.load_pem_public_key(peer_public_bytes)


    
    client_signature = await reader.readuntil(DELIMITER)
    client_certificate_bytes = await reader.readuntil(b"-----END CERTIFICATE-----")  # Lê o certificado do cliente
    extras_with_delimiter = await reader.readuntil(DELIMITER)
    extras = extras_with_delimiter[:-len(DELIMITER)]


    client_certificate = x509.load_pem_x509_certificate(client_certificate_bytes)

        
    with open("Client.crt", "wb") as ficheiro:
        ficheiro.write(extras)
        ficheiro.write(client_certificate.public_bytes(encoding=serialization.Encoding.PEM))



    valida_certALICE("VAULT_CA.crt",file_name = "Client.crt",common_name="User 1 (SSI Vault Client 1)")  


    ##global password
    ##password = shared_key
    
    # Gera a chave partilhada
    shared_key = my_private_key.exchange(peer_public_key)
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