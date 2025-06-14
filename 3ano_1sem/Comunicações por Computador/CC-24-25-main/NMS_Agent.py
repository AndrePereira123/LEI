import socket
from Task import * 
from Metrics import *
import time
import select
import threading

PORT_ALERT = 50001
PORT_NET_TASK = 50000
IP_SERVIDOR = '10.0.6.10' #PC3
IP_CLIENTE = Get_ip.get_local_ip() #my ip
MTU = 500
MAX_RETRIES = 1000
TIME_OUT = 0.1
my_id = -1
sequence = ack = 0


def send_regist_request_UDP():
    #Nesta funcao o cliente envia um pedido de registo inicial ao servidor
    #O servidor deve responder "You are now registered" seguido do id atribuido a este agente
    client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client.bind((IP_CLIENTE, PORT_NET_TASK))
    global ack
    retry = 0

    client.settimeout(TIME_OUT) 
    resposta_recebida = False
    while retry < MAX_RETRIES and not resposta_recebida:
        try:
            msg = "I want to register".encode().ljust(30,b' ')
            print(f"A minha mensagem: {msg}")
            client.sendto(msg,(IP_SERVIDOR,PORT_NET_TASK))
            data = client.recv(MTU)
            confirmation = (data[:30]).decode('utf8').strip()        
            if(confirmation == "You are now registered"):
                resposta_recebida = True
                id = int((data[30:]).decode('utf8').strip()) 
                global my_id 
                my_id = id 
                print(f"My id is {my_id}")
                return client
        except Exception as e:
            retry += 1
            print(f"{e} exception de registo")
    client.settimeout(None)


def send_UDP(socket):
    #Funcao principal de gestao da comunicao UDP agente - servidor
    #Tambem chama funcoes para calcular e enviar as metricas
    global ack
    global sequence
    client = socket

    try:
        
        msg = three_way_handshake(client)   
        #hanshake inicial para q ack e seq entre servidor e cliente 
        

        from_server = None
        from_server = client.recv(MTU) 
        while ((from_server[:30]).decode('utf-8').strip() != "DATA"):
            client.sendto(msg,(IP_SERVIDOR,PORT_NET_TASK))
            from_server = client.recv(MTU)   
            #Caso em que o servidor nao reconheceu o ack final do hanshake (retransmissao) 
        
        ack += 1 ##ack inicial para o primeiro pacote "DATA" (pacote com dados do device)

        mensagem_inteira = recolher_tarefa(from_server,client)

    except KeyboardInterrupt:
        socket.shutdown(socket.SHUT_RDWR)
        socket.close()
       



    print(f"Mensagem inteira recebida: {mensagem_inteira}")
    ## mensagem com tarefa


    device = unpack_task_request(mensagem_inteira)

    frequency = int(device.id)
    is_jitter_server = False
    is_loss_server = False
    
    try:
        iperf_jitter_destination = device.link_metrics.bandwidth.jitter.destiny
        iperf_loss_destination = device.link_metrics.bandwidth.packet_loss.destiny 

        jitter_flag = device.link_metrics.bandwidth.jitter.transport 
        loss_flag = device.link_metrics.bandwidth.packet_loss.transport 

        Meu_ip = Get_ip.get_local_ip()
        

        if (iperf_jitter_destination == Meu_ip): 
            is_jitter_server = True
            iperf_thread_0 = threading.Thread(target=Iperf_Starter.start_iperf,args = (jitter_flag,))
            iperf_thread_0.start()
            print("Servidor iperf iniciado.")

        if (iperf_loss_destination == Meu_ip):   
            is_loss_server   = True
            if (is_jitter_server != True or ((is_jitter_server == True)  and (jitter_flag != loss_flag))):
                iperf_thread = threading.Thread(target=Iperf_Starter.start_iperf,args = (loss_flag,))
                iperf_thread.start()
            if (is_jitter_server != True): print("Servidor iperf iniciado.")
            
        
        
            
        last_execution = time.time()

        while True:
            
            ready_to_read, _, _ = select.select([client], [], [], 0)  # Timeout 0 para não bloquear
            if ready_to_read:
                from_server = client.recv(MTU)
                header = (from_server[:30]).decode('utf-8').strip()
                if (header == "DATA"):
                    id = str(my_id).encode('utf-8').ljust(4,b' ')
                    sequence_number = str(sequence).encode('utf-8').ljust(4,b' ')
                    ack_number = str(ack).encode('utf-8').ljust(4,b' ')
                    msg = "ACK".encode('utf-8').ljust(30,b' ') + id + sequence_number + ack_number
                    client.sendto(msg,(IP_SERVIDOR,PORT_NET_TASK))

            if time.time() - last_execution >= frequency:
                sequence,ack = collect_metrics(device,is_jitter_server,is_loss_server,my_id,client,sequence,ack)
                last_execution = time.time()

    except KeyboardInterrupt:
        print("Execução interrompida pelo usuário.\n")
        print("Conexão vai terminar.\n")

    client.close()

def three_way_handshake(client):
        #Nesta funcao queremos implementar um handshake similar ao do TCP 
        #O cliente comeca por enviar um "SYN" com o valor de sequencia inicial 
        #O servidor usa esse valor para determinar o seq e ack associados ao nosso agente
        #O servidor envia um ACK com esses valores e o agente envia o ACK final terminando o aperto de mao
        global sequence
        global ack
        retry = 0
        client.settimeout(TIME_OUT)
        resposta_recebida = False
        while retry < MAX_RETRIES and not resposta_recebida:
            try:
                id_encoded = str(my_id).encode('utf-8').ljust(4,b' ')
                sequence_encoded = str(sequence).encode('utf-8').ljust(4,b' ')
                msg = "SYN".encode('utf-8').ljust(30,b' ') + id_encoded + sequence_encoded
                client.sendto(msg,(IP_SERVIDOR,PORT_NET_TASK))
                data = client.recv(MTU)
                confirmation = (data[:30]).decode('utf8').strip()

                if(confirmation == "ACK" and (my_id == int((data[30:34]).decode('utf8').strip()))):
                    seq_number = int((data[34:38]).decode('utf8').strip()) #deverá ser 0
                    ack_number = int((data[38:]).decode('utf8').strip())
                    ack = seq_number + 1
                    sequence = ack_number
                    id_encoded = str(my_id).encode('utf-8').ljust(4,b' ')
                    sequence_encoded = str(sequence).encode('utf-8').ljust(4,b' ')
                    ack_Encoded = str(ack).encode('utf-8').ljust(4,b' ')
                    msg = "ACK".encode('utf-8').ljust(30,b' ') + id_encoded + sequence_encoded + ack_Encoded
                    client.sendto(msg,(IP_SERVIDOR,PORT_NET_TASK))
                    resposta_recebida = True 

            except Exception as e:
                retry+=1
                print(f"{e} agente exception de 3 way handshake")

        client.settimeout(None)
        return msg


def recolher_tarefa(from_server,client):
    #Esta funcao serve para receber e confirmar a receão da tarefa atribuida pelo servidor 
    # Funciona com e sem fragmentacao de dados
    global sequence
    global ack
    num_fragments = int(from_server[34:38].decode('utf-8').strip())
    server_sequence = int(from_server[38:42].decode('utf-8').strip())
    server_ack = int(from_server[42:46].decode('utf-8').strip())
    mensagem_inteira = b""

    if (num_fragments == 1):
        mensagem_inteira = from_server[46:]

        id_encoded = str(my_id).encode('utf-8').ljust(4,b' ')
        sequence_encoded = str(sequence).encode('utf-8').ljust(4,b' ')
        ack_encoded = str(ack).encode('utf-8').ljust(4,b' ')
        msg = "ACK".encode('utf-8').ljust(30,b' ') + id_encoded + sequence_encoded + ack_encoded
        client.sendto(msg,(IP_SERVIDOR,PORT_NET_TASK))
    else: 

        mensagem_inteira += from_server[46:]
        recebidos = 0
        while(recebidos < num_fragments - 1):
            print (f"Recebi fragmento {recebidos}\n")

            id_encoded = str(my_id).encode('utf-8').ljust(4,b' ')
            sequence_encoded = str(sequence).encode('utf-8').ljust(4,b' ')
            ack_encoded = str(ack).encode('utf-8').ljust(4,b' ')
            msg = "ACK".encode('utf-8').ljust(30,b' ') + id_encoded + sequence_encoded + ack_encoded

            client.settimeout(TIME_OUT)
            retry = 0
            resposta_recebida = False
            while retry < MAX_RETRIES and not resposta_recebida:
                try:
                    client.sendto(msg,(IP_SERVIDOR,PORT_NET_TASK))  
                    fragment = client.recv(MTU)
                    resposta_recebida = True
                except Exception as e:
                        retry += 1
                        print(f"{e} exception de fragmento de tarefa")
            client.settimeout(None)

            print (f"Recebi este fragmento {fragment}\n")
            
            seq_server = int(fragment[38:42].decode('utf-8').strip())
            if (seq_server == ack):
                ack += 1
                recebidos += 1
                mensagem_inteira += fragment[46:]

        id_encoded = str(my_id).encode('utf-8').ljust(4,b' ')
        sequence_encoded = str(sequence).encode('utf-8').ljust(4,b' ')
        ack_encoded = str(ack).encode('utf-8').ljust(4,b' ')
        msg = "ACK".encode('utf-8').ljust(30,b' ') + id_encoded + sequence_encoded + ack_encoded
        client.sendto(msg,(IP_SERVIDOR,PORT_NET_TASK))
    return mensagem_inteira


def unpack_task_request(msg):
    device = Device.fromBytes(msg)
    frequency = device.id
    print(f"Frequency: {frequency}")
    return device


def main():
    socket = send_regist_request_UDP()
    send_UDP(socket)
    socket.close()

if __name__ == "__main__":
    main()