import json
from typing import List
from typing import Dict


class Device_Metrics:
    def __init__(self,cpu_usage,ram_usage,interface_stats):
        self.cpu_usage = cpu_usage
        self.ram_usage = ram_usage
        self.interface_stats = interface_stats
    
    def toBytes(self):
        cpu_usage_bytes = self.cpu_usage.encode('utf-8').ljust(4,b' ')
        ram_usage_bytes = self.ram_usage.encode('utf-8').ljust(4,b' ')
        interface_stats_bytes = self.interface_stats.encode('utf-8').ljust(48,b' ')
        return (cpu_usage_bytes + ram_usage_bytes + interface_stats_bytes) #"id_bytes +"
    
    def fromBytes(bytes):
        cpu_usage = bytes[:4].decode('utf-8').strip()        
        ram_usage = bytes[4:8].decode('utf-8').strip()
        interface_stats = bytes[8:56].decode('utf-8').strip()
        deserialized = Device_Metrics(cpu_usage,ram_usage,interface_stats) 
        return deserialized
    
    def fromDict(cls,data):
        return cls(cpu_usage = data["cpu_usage"],ram_usage = data["ram_usage"],interface_stats = data["interface_stats"])

class Jitter:
    def __init__(self,destiny,duration,transport):
        self.destiny = destiny
        self.duration = duration
        self.transport = transport

    def toBytes(self):
        destiny_bytes = self.destiny.encode('utf-8').ljust(16,b' ')
        duration_bytes = self.duration.encode('utf-8').ljust(16,b' ')
        transport_bytes = self.transport.encode('utf-8').ljust(4,b' ')
        return (destiny_bytes + duration_bytes + transport_bytes)

    def fromBytes(bytes):
        destiny = bytes[:16].decode('utf-8').strip()
        duration = bytes[16:32].decode('utf-8').strip()
        transport = bytes[32:36].decode('utf-8').strip()
        deserialized = Jitter(destiny,duration,transport)
        return deserialized

    def fromDict(cls,data):
        return cls(destiny = data["destiny"],duration = data["duration"],transport = data["transport"])

class Packet_loss:
    def __init__(self,destiny,duration,transport):
        self.destiny = destiny
        self.duration = duration
        self.transport = transport

    def toBytes(self):
        destiny_bytes = self.destiny.encode('utf-8').ljust(16,b' ')
        duration_bytes = self.duration.encode('utf-8').ljust(16,b' ')
        transport_bytes = self.transport.encode('utf-8').ljust(4,b' ')
        return (destiny_bytes + duration_bytes + transport_bytes)

    def fromBytes(bytes):
        destiny = bytes[:16].decode('utf-8').strip()
        duration = bytes[16:32].decode('utf-8').strip()
        transport = bytes[32:36].decode('utf-8').strip()
        deserialized = Packet_loss(destiny,duration,transport)
        return deserialized

    
    def fromDict(cls,data):
        return cls(destiny = data["destiny"],duration = data["duration"],transport = data["transport"])
    
class Latency:
    def __init__(self,destiny,package_count,frequency):
        self.destiny = destiny
        self.package_count = package_count
        self.frequency = frequency

    def toBytes(self):
        destiny_bytes = self.destiny.encode('utf-8').ljust(16,b' ')
        package_count_bytes = self.package_count.encode('utf-8').ljust(16,b' ')
        frequency_bytes = self.frequency.encode('utf-8').ljust(8,b' ')
        return (destiny_bytes + package_count_bytes + frequency_bytes)

    def fromBytes(bytes):
        destiny = bytes[:16].decode('utf-8').strip()
        package_count = bytes[16:32].decode('utf-8').strip()
        frequency = bytes[32:40].decode('utf-8').strip()
        deserialized = Latency(destiny,package_count,frequency)
        return deserialized

    def fromDict(cls,data):
        return cls(destiny = data["destiny"],package_count = data["package_count"],frequency = data["frequency"])


class Bandwith:
    def __init__(self,jitter:Jitter,packet_loss:Packet_loss,latency:Latency):
        self.jitter = jitter
        self.packet_loss = packet_loss
        self.latency = latency

    def toBytes(self):
        jitter_bytes = self.jitter.toBytes().ljust(36,b' ')
        packet_loss_bytes = self.packet_loss.toBytes().ljust(36,b' ')
        latency_bytes = self.latency.toBytes().ljust(40,b' ')
        return (jitter_bytes + packet_loss_bytes + latency_bytes)
    
    def fromBytes(bytes): ##TODO
        jitter = Jitter.fromBytes(bytes[:36])
        packet_loss = Packet_loss.fromBytes(bytes[36:72])
        latency = Latency.fromBytes(bytes[72:])
        deserialized = Bandwith(jitter,packet_loss,latency)
        return deserialized

    def fromDict(cls,data):
        jitter = Jitter.fromDict(Jitter,data["jitter"])
        packet_loss = Packet_loss.fromDict(Packet_loss,data["packet_loss"])
        latency = Latency.fromDict(Latency,data["latency"])
        return cls(jitter = jitter,packet_loss = packet_loss,latency = latency)

    def toDict(self):
        return {
            "jitter": self.jitter.__dict__,
            "packet_loss": self.packet_loss.__dict__,
            "latency": self.latency.__dict__
        }


class AlertFlow_conditions:
    def __init__(self,cpu_usage,ram_usage,interface_stats,packet_loss,jitter):
        self.cpu_usage = cpu_usage
        self.ram_usage = ram_usage
        self.interface_stats = interface_stats
        self.packet_loss = packet_loss
        self.jitter = jitter

    def toBytes(self):
        cpu_usage_bytes = self.cpu_usage.encode('utf-8').ljust(4,b' ')
        ram_usage_bytes = self.ram_usage.encode('utf-8').ljust(4,b' ')
        interface_stats_bytes = self.interface_stats.encode('utf-8').ljust(48,b' ')
        jitter_bytes = self.jitter.encode('utf-8').ljust(4,b' ')
        packet_loss_bytes = self.packet_loss.encode('utf-8').ljust(4,b' ')     
        return (cpu_usage_bytes + ram_usage_bytes + interface_stats_bytes + jitter_bytes + packet_loss_bytes)
    
    def fromBytes(bytes):
        cpu_usage = bytes[:4].decode('utf-8').strip()        
        ram_usage = bytes[4:8].decode('utf-8').strip()
        interface_stats = bytes[8:56].decode('utf-8').strip()
        jitter = bytes[56:60].decode('utf-8').strip()
        packet_loss = bytes[60:64].decode('utf-8').strip()   
        deserialized = AlertFlow_conditions(cpu_usage,ram_usage,interface_stats,packet_loss,jitter)
        return deserialized
    
    
    def fromDict(cls,data):
        return cls(cpu_usage = data["cpu_usage"],ram_usage = data["ram_usage"],interface_stats = data["interface_stats"],
                   jitter = data["jitter"],packet_loss = data["packet_loss"])


class Link_Metrics:
    def __init__(self,bandwidth:Bandwith,alertflow_conditions:AlertFlow_conditions):
        self.bandwidth = bandwidth
        self.alertflow_conditions = alertflow_conditions

    def toBytes(self):
        bandwidth_bytes = self.bandwidth.toBytes().ljust(112,b' ')
        alertflow_condition_bytes = self.alertflow_conditions.toBytes().ljust(64,b' ')
        return ( bandwidth_bytes + alertflow_condition_bytes)
    
    def fromBytes(bytes):
        bandwidth = Bandwith.fromBytes(bytes[:112])   
        alertflow_conditions = AlertFlow_conditions.fromBytes(bytes[112:])
        deserialized = Link_Metrics(bandwidth,alertflow_conditions)
        return deserialized
    
    def toDict(self):
        return {
            "bandwidth": self.bandwidth.toDict(),
            "alertflow_conditions": self.alertflow_conditions.__dict__
        }

    def fromDict(cls,data):
        bandwidth = Bandwith.fromDict(Bandwith,data["bandwidth"])
        alertflow_conditions = AlertFlow_conditions.fromDict(AlertFlow_conditions,data["alertflow_conditions"])
        return cls(bandwidth = bandwidth, alertflow_conditions = alertflow_conditions)

    
class Device:
    def __init__(self,id,device_metrics:Device_Metrics,link_metrics:Link_Metrics):
        self.id = id
        self.device_metrics = device_metrics
        self.link_metrics = link_metrics

    def toBytes(self):
        id_bytes = self.id.encode('utf-8').ljust(16,b' ')
        device_metrics_bytes = self.device_metrics.toBytes().ljust(56,b' ') 
        link_Metrics_bytes_ = self.link_metrics.toBytes().ljust(176,b' ')
        return (id_bytes + device_metrics_bytes + link_Metrics_bytes_)

    def fromBytes(bytes):
        id = bytes[:16].decode('utf-8').strip()
        device_metrics = Device_Metrics.fromBytes(bytes[16:72])   
        link_metrics = Link_Metrics.fromBytes(bytes[72:248])
        deserialized = Device(id,device_metrics,link_metrics)
        return deserialized
    
    def toDict(self):
        return {
            "id": self.id,  
            "device_metrics": self.device_metrics.__dict__,  
            "link_metrics": self.link_metrics.toDict()  
        }
    
    def fromDict(cls,data):
        link_metrics = Link_Metrics.fromDict(Link_Metrics,data["link_metrics"])
        device_metrics = Device_Metrics.fromDict(Device_Metrics,data["device_metrics"])
        return cls(id = data["id"],device_metrics = device_metrics, link_metrics = link_metrics)

        

class Task:
    def __init__(self,id,frequency,devices:List[Device]):
        self.id = id
        self.frequency = frequency
        self.devices = devices

    def toBytes(self):
        id_bytes = self.id.encode('utf-8').ljust(16,b' ')
        frequency_bytes = self.frequency.encode('utf-8').ljust(16,b' ')
        devices_bytes = b''.join(device.toBytes() for device in self.devices)
        return (id_bytes + frequency_bytes + devices_bytes)

    def fromBytes(self,bytes):
        id = bytes[:16].decode('utf-8').strip()
        frequency = bytes[16:32].decode('utf-8').strip()
        devices = []
        for i in range (32,len(bytes),248):
            chunk = bytes[i:i+248]

            if len(chunk) == 248:
                device = Device.fromBytes(chunk)
                devices.append(device)
        
        task = Task(id,frequency,devices)
        return task

    def toDict(self):
        return{
            "id": self.id,
            "frequency": self.frequency,
            "devices": [device.toDict() for device in self.devices]
        }

    def fromDict(cls, data: Dict) -> 'Task':
        devices = [Device.fromDict(Device,device) for device in data["devices"]]
        return cls(id=data["id"], frequency=data["frequency"], devices=devices)