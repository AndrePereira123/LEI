import psutil
import time
import subprocess
import re 
import socket
import time
import json
import os
from Task import Device

PORT_ALERT = 50001
PORT_NET_TASK = 50000
IP_SERVIDOR = '10.0.6.10' #PC3
MTU = 500 #minimo de 46
TIME_OUT = 0.1
MAX_RETRIES = 1000
IPERF_UDP_PORT = 49000

class Get_ip:
    @staticmethod
    def get_local_ip():
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
                s.connect(("8.8.8.8", 80))  # Google public DNS server
                local_ip = s.getsockname()[0]  
            return local_ip
        except Exception as e:
            print("Erro a obter endereço IP :", e)
            return None

class Iperf_Starter:
    def start_iperf(transport_flag):
        try: # Servidor
            if (transport_flag == "-u"):
                porta = IPERF_UDP_PORT
            else:
                return 
            result = subprocess.run(
                ["iperf", "-s", transport_flag ,"-p",str(porta)], ##transport flag / -p / port 
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,  
                text=True
            )
            print("Servidor iperf3 iniciado.")

        except Exception as e:
            print("Servidor iperf não iniciou:", e)

class Fragmentacao:
    def fragment_message(msg,mtu):
        fragments = []
        total_size = len(msg)
        num_fragments = (total_size + mtu - 1) // mtu  # Número de fragmentos necessários
    
        for i in range(num_fragments):
            # Calcula o índice de início e fim do fragmento
            start = i * mtu
            end = start + mtu
            fragment = msg[start:end]
            
            fragments.append(fragment)
        
        return fragments
#Jitter
def process_jitter(is_jitter_server,iperf_jitter_destination,jitter_duration,flag_jitter):
    if (flag_jitter == "-u"):
        porta = IPERF_UDP_PORT
        comando = ["iperf","-c",iperf_jitter_destination,flag_jitter,"-p",str(porta),"-t",jitter_duration,'-f','m']
    else:
        return "-1"
    
    if (is_jitter_server == False):
        try:
            result = subprocess.run(comando,
            # -p porta   -t tempo 
            stdout=subprocess.PIPE,
            text=True
            ) 
           
            jitter_matches = re.findall(r'(\d+\.\d+)\s+ms', result.stdout)

            if len(jitter_matches) >= 1:  
                jitter = jitter_matches[0]  
                print(f"Jitter deu isto: {jitter} ms")
            else:
                print("Jitter não encontrado ou saída inesperada.")
                jitter = "unknown"
                msg = "Jitter extremamente alto ou falha na conexao " + jitter
                send_alert(msg)

        except Exception as e:
            print (f"{e} metricas jitter")
    else:
        print ("Sou servidor de jitter")
        jitter = "server"
    return jitter

#Packet Loss
def process_loss (is_loss_server,iperf_loss_destination,flag_loss,loss_duration):
    if (flag_loss == "-u"):
        porta = IPERF_UDP_PORT
        comando = ["iperf","-c",iperf_loss_destination,flag_loss,"-p",str(porta),"-t",loss_duration,'-f','m']
    else:
        return "-1"

    if (is_loss_server == False):
        try:
            result = subprocess.run(comando,
            stdout=subprocess.PIPE,
            text=True
            ) 
            packet_loss_match = re.findall(r'\((\d+\.?\d*)%\)', result.stdout)

            if len(packet_loss_match) >= 1:  
                packet_loss = packet_loss_match[0]
                print (f"Packet_loss deu isto: {packet_loss}%")
            else:
                print("Packet_loss não encontrado ou saída inesperada.")
                packet_loss = "unknown"
                msg = "Packet_loss extremamente alto ou falha na conexao " + packet_loss
                send_alert(msg)

        except Exception as e:
            print (f"{e} metricas packet loss")
    else:
        print ("Sou servidor de packet_loss")
        packet_loss = "server"
    return packet_loss

#PING - latency
def process_latency(ping_package_count,ping_frequency,ping_destination):
        try:
        # Executa o comando ping
            latency = subprocess.run(
                ["ping", "-c", ping_package_count, "-i", ping_frequency, ping_destination],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE, 
                text=True,
                check=True               
            )
            matches = re.findall(r'time=([\d\.]+) ms', latency.stdout)
            if matches:
                times = [float(time) for time in matches]
                total_time = sum(times)
                latency = total_time / len(times)
                print(f"Latency: {latency} ms")
            else:
                latency = -1
                print("Regex não encontrou nada")
        except Exception as e:
            latency = -1
            print("Erro inesperado durante o cálculo da latência:")
            print(e)
        return latency

class Metricas:
    def __init__(self,timestamp,cpu_usage,ram_usage,interface_stats,jitter,packet_loss,latency):
        self.timestamp = timestamp
        self.cpu_usage = cpu_usage
        self.ram_usage = ram_usage
        self.interface_stats = interface_stats
        self.jitter = jitter
        self.packet_loss = packet_loss
        self.latency = latency

    def toBytes(self):
        timestamp_bytes = self.timestamp.encode('utf-8').ljust(30,b' ')
        cpu_usage_bytes = self.cpu_usage.encode('utf-8').ljust(4,b' ')
        ram_usage_bytes = self.ram_usage.encode('utf-8').ljust(10,b' ')
        interface_stats_bytes = self.interface_stats.encode('utf-8').ljust(48,b' ')
        jitter_bytes = self.jitter.encode('utf-8').ljust(36,b' ')
        packet_loss_bytes = self.packet_loss.encode('utf-8').ljust(36,b' ')
        latency_bytes = self.latency.encode('utf-8').ljust(40,b' ')
        return (timestamp_bytes + cpu_usage_bytes + ram_usage_bytes + interface_stats_bytes + jitter_bytes + packet_loss_bytes + latency_bytes)

    def fromBytes(bytes):
        timestamp = bytes[:30].decode('utf-8').strip()   
        cpu_usage = bytes[30:34].decode('utf-8').strip()        
        ram_usage = bytes[34:44].decode('utf-8').strip()
        interface_stats = bytes[44:92].decode('utf-8').strip()
        jitter = bytes[92:128].decode('utf-8').strip()
        packet_loss = bytes[128:164].decode('utf-8').strip()
        latency = bytes[164:].decode('utf-8').strip() #204
        deserialized = Metricas(timestamp,cpu_usage,ram_usage,interface_stats,jitter,packet_loss,latency) 
        return deserialized

    def fromDict(cls,data):
        return cls(timestamp = data["timestamp"],cpu_usage = data["cpu_usage"],ram_usage = data["ram_usage"],interface_stats = data["interface_stats"],
        jitter = data["jitter"], packet_loss = data["packet_loss"], latency = data["latency"])
    
    def save_metrics_to_json(self, file_path):
        with open(file_path, 'w') as file:
            json.dump(self.__dict__, file, indent=4)


    def save_metrics_to_json(self, filename):
        try:
            with open(filename, "r") as f:
                data = json.load(f)
        except (FileNotFoundError, json.JSONDecodeError):
            data = []
        
        data.append(self.__dict__)

        with open(filename, "w") as f:
            json.dump(data, f, indent=4)

        
def measure_packets_per_second(interface, interval=1):
    # Obtém as estatísticas iniciais
    net_stats_start = psutil.net_io_counters(pernic=True)
    if interface not in net_stats_start:
        print (f"A interface {interface} não foi encontrada.")
        return -1, -1 
    
    start_packets_recv = net_stats_start[interface].packets_recv
    start_packets_sent = net_stats_start[interface].packets_sent

    # Espera pelo intervalo de tempo definido
    time.sleep(interval)

    # Obtém as estatísticas finais
    net_stats_end = psutil.net_io_counters(pernic=True)
    end_packets_recv = net_stats_end[interface].packets_recv
    end_packets_sent = net_stats_end[interface].packets_sent

    # Calcula pacotes por segundo
    pps_recv = (end_packets_recv - start_packets_recv) / interval
    pps_sent = (end_packets_sent - start_packets_sent) / interval

    return pps_recv, pps_sent        


def measure_cpu():
    cpu_usage = psutil.cpu_percent(interval=0)
    return cpu_usage

def collect_metrics(device : Device,is_jitter_server,is_loss_server,my_id,client,seq,ack):   
    #Esta funcao realiza toda a coleta de metricas com a ajuda de varias funcoes auxiliares 

    
    cpu_usage = 0
    cpu_times = 0

    condition_cpu = (device.device_metrics.cpu_usage == "1")
    if(condition_cpu):
        cpu_usage += measure_cpu()
        cpu_times += 1

    ## RAM #########################################################################
    
    
    if(condition_cpu):
        cpu_usage += measure_cpu()
        cpu_times += 1

    if(device.device_metrics.ram_usage == "1"):
        ram_usage = psutil.virtual_memory().percent
        ram_usage = round(ram_usage, 5)
        if(ram_usage > float(device.link_metrics.alertflow_conditions.ram_usage)): 
            msg = "O limite de RAM foi atingido: " + str(ram_usage) + "/" + (device.link_metrics.alertflow_conditions.ram_usage)
            send_alert(msg)
    else:
        ram_usage = -1
    print(f"Ram Usage : {ram_usage}")    
    
    ## Interface Stats ###############################################################


    interface_stats_str = device.device_metrics.interface_stats
    interface_stats = interface_stats_str.split(",")

    if(condition_cpu):
        cpu_usage += measure_cpu()
        cpu_times += 1
    for interface in interface_stats:
        recv_pps, sent_pps = measure_packets_per_second(interface,1)
        if(recv_pps > float(device.link_metrics.alertflow_conditions.interface_stats)):
            msg = "O limite de pps foi atingido: " + str(recv_pps) + "/" + (device.link_metrics.alertflow_conditions.interface_stats)
            send_alert(msg)

    print (f"Interface Stats: {interface_stats}")


    iperf_jitter_destination = device.link_metrics.bandwidth.jitter.destiny
    jitter_duration = device.link_metrics.bandwidth.jitter.duration
    flag_jitter = device.link_metrics.bandwidth.jitter.transport #-u para udp ou vazio

    iperf_loss_destination = device.link_metrics.bandwidth.packet_loss.destiny 
    loss_duration = device.link_metrics.bandwidth.packet_loss.duration 
    flag_loss = device.link_metrics.bandwidth.packet_loss.transport #-u para udp ou vazio

    ping_destination = device.link_metrics.bandwidth.latency.destiny
    ping_package_count = device.link_metrics.bandwidth.latency.package_count
    ping_frequency = device.link_metrics.bandwidth.latency.frequency


    ## Jitter #########################################################################
    jitter = process_jitter(is_jitter_server,iperf_jitter_destination,jitter_duration,flag_jitter)

    if(condition_cpu):
        cpu_usage += measure_cpu()
        cpu_times += 1

    if(jitter != "server" and jitter != "unknown"):
        if(float(jitter) > float(device.link_metrics.alertflow_conditions.jitter)):
            msg = "O limite de jitter foi atingido: " + str(jitter) + "/" + (device.link_metrics.alertflow_conditions.jitter)
            send_alert(msg)
    

    ## Packet_Loss ########################################################################
    packet_loss = process_loss(is_loss_server,iperf_loss_destination,flag_loss,loss_duration)

    if(condition_cpu):
        cpu_usage += measure_cpu()
        cpu_times += 1

    if(packet_loss != "server" and packet_loss != "unknown"):
        if(float(packet_loss) > float(device.link_metrics.alertflow_conditions.packet_loss)):
            msg = "O limite de loss foi atingido: " + str(packet_loss) + "/" + (device.link_metrics.alertflow_conditions.packet_loss)
            send_alert(msg)
    
    ## Latency #############################################################################
    
    latency = process_latency(ping_package_count,ping_frequency,ping_destination)

    ## CPU  #########################################################################

    if(condition_cpu):
        cpu_usage += measure_cpu()
        cpu_times += 1
        cpu_usage /= cpu_times
        cpu_usage = round(cpu_usage, 1)
        if(cpu_usage > float(device.link_metrics.alertflow_conditions.cpu_usage)):
            msg = "O limite de CPU foi atingido: " + str(cpu_usage) + "/" + (device.link_metrics.alertflow_conditions.cpu_usage)
            send_alert(msg)
    else:
        cpu_usage = -1;         
    
    print (f"Cpu Usage : {cpu_usage}")
    
    


    ## Timestamp para identificar coleta ###############################################
    timestamp = time.time()
    timestamp_string = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(timestamp))

    
    ## Criaçao de objeto Metricas ##################################################################
    metricas = Metricas(timestamp_string,str(cpu_usage),str(ram_usage),str(interface_stats),jitter,packet_loss,str(latency))
    msg = Metricas.toBytes(metricas)

    
    
    tipo_msg = "Metricas".encode('utf-8').ljust(30,b' ')
    id_cliente = str(my_id).encode('utf-8').ljust(4,b' ')


    tuplo = (IP_SERVIDOR,PORT_NET_TASK)

    
    client.settimeout(TIME_OUT)

    if(len(msg)+46 > MTU):

        seq = envio_fragmentado_metricas(tipo_msg,tuplo,id_cliente,msg,client,seq)     

    else:

        seq = envio_metricas(tipo_msg,tuplo,id_cliente,msg,client,seq) 

    client.settimeout(None) 
        
    return seq,ack

def envio_metricas(tipo_msg,tuplo,id_cliente,msg,client,seq):
    #esta funcao envia as metricas ao servidor sem fragmentacao 
    retry = 0
    resposta_recebida = False
    while retry < MAX_RETRIES and not resposta_recebida:    
        try:           
            tamanho_1 = "1".encode('utf-8').ljust(4,b' ')
            seq_bytes = str(seq).encode('utf-8').ljust(4,b' ')
            ack_bytes = str(seq).encode('utf-8').ljust(4,b' ')

            header = (tipo_msg + tamanho_1 + id_cliente + seq_bytes + ack_bytes) 
            client.sendto(header+msg,tuplo)

            data, addr = client.recvfrom(MTU)
            confirmation = data[:30].decode('utf-8').strip()
            print (f"Recebi: {confirmation}")

            if(confirmation == "ACK"):
                seq_server = int(data[34:38].decode('utf-8'))
                ack_server = int(data[38:42].decode('utf-8'))
                if(ack_server == seq + 1):
                    seq += 1
                    resposta_recebida = True
        except Exception as e:
            retry+=1
            print(f"{e} metricas mensagem inteira")
    return seq

def envio_fragmentado_metricas(tipo_msg,tuplo,id_cliente,msg,client,seq):
    #esta funcao envia as metricas ao servidor com fragmentacao 
    fragmentos = Fragmentacao.fragment_message(msg,MTU-46)
    num_fragmentos = str(len(fragmentos)).encode('utf-8').ljust(4,b' ')
    for fragment in fragmentos:
        seq_bytes = str(seq).encode('utf-8').ljust(4,b' ')
        ack_bytes = str(seq).encode('utf-8').ljust(4,b' ')
        header = (tipo_msg + num_fragmentos + id_cliente + seq_bytes + ack_bytes) # 30 4 4 4 4

        retry = 0

        resposta_recebida = False
        while retry < MAX_RETRIES and not resposta_recebida: 
             try:
                 client.sendto(header+fragment,tuplo)
                 data, addr = client.recvfrom(MTU)
                 confirmation = data[:30].decode('utf-8').strip()

                 if(confirmation == "ACK"):
                     seq_server = int(data[34:38].decode('utf-8'))
                     ack_server = int(data[38:42].decode('utf-8'))
                     if(ack_server >= seq + 1):
                         seq +=1
                         resposta_recebida = True

             except Exception as e:
                 retry+=1
                 print(f"{e} metricas fragmentado")
    return seq

def send_alert(message):
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((IP_SERVIDOR, PORT_ALERT))  
        client.send(message.encode('utf-8'))  
    except Exception as e:
        print(f"Erro ao enviar alerta: {e}")
    finally:
        client.close()



     

