import socket
import threading
from Task import * 
from Metrics import Metricas
from Metrics import Fragmentacao
import os



PORT_ALERT = 50001
PORT_NET_TASK = 50000
IP_SERVIDOR = '10.0.6.10' #PC3
MTU = 500 #minimo de 46
MAX_RETRIES = 1000
TIME_OUT = 0.1
client_regists = {} 
metrics = {}
id = 0
acks = {}
sequences = {}


with open("tasks.json","r") as json_file:
    task_data = json.load(json_file)

task1 = Task.fromDict(Task,task_data)  
tasks = {task1}

def new_server(host = IP_SERVIDOR):
    net_task = new_NetTask (host,PORT_NET_TASK)
    alert_flow = new_AlertFlow(host,PORT_ALERT)
    return net_task,alert_flow

def new_NetTask(host, port):
    net_task = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    net_task.bind((host,port))
    return net_task

def new_AlertFlow(host, port):
    alert_flow = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    alert_flow.bind((host,port))
    alert_flow.listen(5)
    return alert_flow


def run_server(net_task,alert_flow):
    udp_thread = threading.Thread(target=run_UDP,args=(net_task,))
    tcp_thread = threading.Thread(target=run_TCP,args=(alert_flow,))

    udp_thread.start()
    tcp_thread.start()

    udp_thread.join()
    tcp_thread.join()



def run_UDP(net_task):
    #Esta funcao encarrega-se de receber mensagens do cliente e agir de acordo
    global acks
    global sequences
    try:
        while True:
            global task1
            from_client = ""
            data, addr = net_task.recvfrom(MTU) # aviso  # garantir q mensagens teem padding para este tamanho
            if not data: break                  
            header = data[:30].decode('utf-8').strip() 

            ##print (f'From client: .{header}.')

            if(header == "I want to register"):
                accept_regist_request_UDP(addr,net_task)

            if(header == "SYN"):
                handle_SYN(net_task,data,addr)
                
            if(header.strip() == "Metricas"):
                handle_Metricas(net_task,data,addr)

    except KeyboardInterrupt:
        socket.shutdown(socket.SHUT_RDWR)
        socket.close()


def accept_regist_request_UDP(addr,net_task):
    #Nesta funcao e aceite e registado o id associado a um agente que pretende registar-se
    global id 
    global client_regists

    nao_registado = True
    for k, v in client_regists.items():
        if (v == addr[0]):
            id_existente = k
            msg = "You are now registered".encode('utf-8').ljust(30,b' ') + str(id_existente).encode('utf-8')
            nao_registado = False
    
    if (nao_registado):
        id += 1
        client_regists[id] = addr[0]
        print (f'From server: Id {id} registered from {addr}')
        msg = "You are now registered".encode('utf-8').ljust(30,b' ') + str(id).encode('utf-8')

    net_task.sendto(msg,addr)

def handle_SYN(net_task,data,addr):
    #Nesta funcao e realizado o handshake por parte do servidor apos receber o "SYN" inicial
    global acks
    global sequences
    id = int(data[30:34].decode('utf8').strip())
    sequence = int((data[34:38]).decode('utf8').strip())
    sequences[id] = sequence
    acks[id] = sequence +1   ##recebe mensagem aumenta ack
    retry = 0
    net_task.settimeout(TIME_OUT)
    resposta_recebida = False
    while retry < MAX_RETRIES and not resposta_recebida:
        try:
            sequence_encoded = str(sequences[id]).encode('utf-8').ljust(4,b' ')
            ack_encoded = str(acks[id]).encode('utf-8').ljust(4,b' ')
            id_bytes = data[30:34]
            msg = "ACK".encode('utf-8').ljust(30,b' ') + id_bytes + sequence_encoded + ack_encoded
            net_task.sendto(msg,addr)                   
            data, addr = net_task.recvfrom(MTU)         
            confirmation = data[:30].decode('utf-8').strip()

            print (f"Recebi isto no loop: {data}")

            if(confirmation == "ACK"):
                resposta_recebida = True
                id = int(data[30:34].decode('utf8').strip())
                sequences[id] += 1               
                parser_devices(net_task,task1,id)

        except Exception as e:
            retry+=1
            print(f"{e} exception no 3 way handshake (servidor)")

    net_task.settimeout(None)

def handle_Metricas(net_task,data,addr):
            #Esta funcao recebe os dados relativos as metricas de algum agente
            #Pode receber os dados fragmentados ou nao e regista-os num ficheiro JSON
            #associado ao cliente
            num_fragments = int(data[30:34].decode('utf-8').strip())   
            id_cliente = int(data[34:38].decode('utf-8').strip())

            agent_sequence = int(data[38:42].decode('utf-8').strip())
            agent_ack = int(data[42:46].decode('utf-8').strip())


            if (agent_sequence < acks[id_cliente]):
               id_bytes = data[34:38]
               sequence_encoded = str(sequences[id_cliente]).encode('utf-8').ljust(4,b' ')
               ack_encoded = str(acks[id_cliente]).encode('utf-8').ljust(4,b' ')
               msg = "ACK".encode('utf-8').ljust(30,b' ') + id_bytes + sequence_encoded + ack_encoded
               ## ack + id cliente + seq + ack
               net_task.sendto(msg,addr)

            else:
                acks[id_cliente] += 1  ## ack deve ser seq + 1

                metricas_bytes = b""

                if (num_fragments == 1):
                      metricas_bytes = data[46:]

                      id_bytes = data[34:38]
                      sequence_encoded = str(sequences[id_cliente]).encode('utf-8').ljust(4,b' ')
                      ack_encoded = str(acks[id_cliente]).encode('utf-8').ljust(4,b' ')
                      msg = "ACK".encode('utf-8').ljust(30,b' ') + id_bytes + sequence_encoded + ack_encoded
                      ## ack + id cliente + seq + ack
                      net_task.sendto(msg,addr)

                else:
                        metricas_bytes += data[46:]
                        recebidos = 0
                        while(recebidos < num_fragments - 1):

                            print (f"Recebi fragmento {recebidos}\n")

                            id_bytes = data[34:38]
                            sequence_encoded = str(sequences[id_cliente]).encode('utf-8').ljust(4,b' ')
                            ack_encoded = str(acks[id_cliente]).encode('utf-8').ljust(4,b' ')
                            msg = "ACK".encode('utf-8').ljust(30,b' ') + id_bytes + sequence_encoded + ack_encoded
                            ## ack + id cliente + seq + ack
                            net_task.settimeout(TIME_OUT)
                            retry = 0
                            resposta_recebida = False
                            while retry < MAX_RETRIES and not resposta_recebida:
                                try:
                                    net_task.sendto(msg,addr)  
                                    fragment = net_task.recv(MTU)
                                    resposta_recebida = True
                                except Exception as e:
                                        retry += 1
                                        print(f"{e} exception a receber metricas de agente fragmentadas")
                            net_task.settimeout(None)

                            print (f"Recebi este fragmento {fragment}\n")
                            
                            seq_agent = int(fragment[38:42].decode('utf-8').strip())
                            if (seq_agent == acks[id_cliente]):
                                acks[id_cliente] += 1
                                recebidos += 1
                                metricas_bytes += fragment[46:]

                            id_bytes = data[34:38]
                            sequence_encoded = str(sequences[id_cliente]).encode('utf-8').ljust(4,b' ')
                            ack_encoded = str(acks[id_cliente]).encode('utf-8').ljust(4,b' ')
                            msg = "ACK".encode('utf-8').ljust(30,b' ') + id_bytes + sequence_encoded + ack_encoded
                            ## ack + id cliente + seq + ack
                            net_task.sendto(msg,addr)

                metricas = Metricas.fromBytes(metricas_bytes)

                folder = "metricas"
                os.makedirs(folder, exist_ok=True)

                filename = "metricas/metricas_cliente_" + str(id_cliente) + ".json"
                metricas.save_metrics_to_json(filename)
                print ("Metricas do cliente " + str(id_cliente) + " guardadas em json")

def parser_devices(net_task,task : Task,id):
    #Esta funcao e chamada logo apos confirmar o aperto de mao com um agente novo
    # Ela percorre a tarefa e envia o dispositivo relevante ao nosso cliente
    global client_regists
    frequency = task.frequency
    devices = task.devices
    
    for device in devices:
        if (device.id == str(id)):
            addr = client_regists.get(id)
        
            if (addr != None):
                msg = Device.toBytes(device)
                frequency = str(task.frequency).encode('utf-8').ljust(16, b' ')
                msg = frequency + msg[16:]
                tuplo = (addr,PORT_NET_TASK)
                retry = 0
                

                net_task.settimeout(TIME_OUT)

                if(len(msg)+46 > MTU):
                   fragments = Fragmentacao.fragment_message(msg,MTU-46)
                   for fragment in fragments:
                       fragments_length = str(len(fragments)).encode('utf-8').ljust(4,b' ')
                       sequence_encoded = str(sequences[id]).encode('utf-8').ljust(4,b' ')
                       ack_encoded = str(acks[id]).encode('utf-8').ljust(4,b' ')
                       id_bytes = str(id).encode('utf-8').ljust(4,b' ')
                       header = "DATA".encode('utf-8').ljust(30,b' ') + id_bytes + fragments_length + sequence_encoded + ack_encoded
                       resposta_recebida = False
                       while retry < MAX_RETRIES and not resposta_recebida: 
                            try:
                                net_task.sendto(header+fragment,tuplo)
                                data, addr = net_task.recvfrom(MTU)
                                confirmation = data[:30].decode('utf-8').strip()

                                if(confirmation == "ACK"):
                                    seq = int(data[34:38].decode('utf-8'))
                                    ack = int(data[38:42].decode('utf-8'))
                                    if(ack >= sequences[id] + 1):
                                        sequences[id]+=1
                                        resposta_recebida = True
                            except Exception as e:
                                retry+=1
                                print(f"{e} exception a enviar tarefa a cliente fragmentada")           
                else:
                    resposta_recebida = False
                    while retry < MAX_RETRIES and not resposta_recebida:    
                        try:                
                            one_package = "1".encode('utf-8').ljust(4,b' ')
                            sequence_encoded = str(sequences[id]).encode('utf-8').ljust(4,b' ')
                            ack_encoded = str(acks[id]).encode('utf-8').ljust(4,b' ')
                            id_bytes = str(id).encode('utf-8').ljust(4,b' ')
                            header = "DATA".encode().ljust(30,b' ') + id_bytes + one_package + sequence_encoded + ack_encoded
                            net_task.sendto(header+msg,tuplo)
                            data, addr = net_task.recvfrom(MTU)
                            confirmation = data[:30].decode('utf-8').strip()
                            print (f"Recebi: {confirmation}")

                            if(confirmation == "ACK"):
                                seq = int(data[34:38].decode('utf-8'))
                                ack = int(data[38:42].decode('utf-8'))
                                if(ack == sequences[id] + 1):
                                    sequences[id] += 1
                                    resposta_recebida = True
                        except Exception as e:
                            retry+=1
                            print(f"{e} exception a enviar tarefa a cliente inteira")
                net_task.settimeout(None)



def run_TCP(alert_flow):
    print("Servidor TCP em execução...")
    while True:
        conn, addr = alert_flow.accept()
        from_client = ""
        while True:
            data = conn.recv(4096)  
            if not data: 
                break  
            from_client += data.decode('utf-8')
            print(f"Alerta recebido: {from_client} ({addr[0]})")
        conn.send("Alerta processado pelo servidor\n".encode())
        conn.close()


def main():
    net_task, alert_flow = new_server()
    run_server(net_task, alert_flow)
    net_task.close()

if __name__ == "__main__":
    main()