
class Veiculo:
    VEICULOS_VALIDOS = {"TERRA","TERRA-PESADO","AR","MAR","MAR E AR","TERRA E AR"}
    def __init__(self,tipo,nome,cap_max,autonomia,tempo_de_viagem,consumo):
        if tipo not in Veiculo.VEICULOS_VALIDOS:
            raise ValueError(f"Tipo inválido: {tipo}. "
                        f"Valores permitidos: {Veiculo.VEICULOS_VALIDOS}")
        self.nome = nome
        self.tipo = tipo
        self.cap_max = cap_max
        self.autonomia = autonomia
        self.combustivel_disponivel = autonomia
        self.tempo_de_viagem = tempo_de_viagem  ##Tempo de viagem = velocidade km/hora
        self.tempo_desde_inicio = 0 ## usado para comparar com janelas de tempo de nodos
        self.consumo = consumo
    
    def adaptar_tempo_a_condicao_meteriologica(self,condicao_meteriologica):
        match condicao_meteriologica:
            case "BOA":
                return 1
            case "MODERADA":
                return 1.5
            case "MÁ":
                return 2.5

    def veiculo_tem_acesso(self,acessibilidade):
        if(acessibilidade == "TUDO"):
            return True
        if(self.tipo == "TERRA" and (acessibilidade == "TUDO" or acessibilidade == "TERRA" or acessibilidade == "TERRA-PESADO" or acessibilidade == "TERRA E AR")):
            return True
        if(self.tipo == "TERRA-PESADO" and (acessibilidade == "TUDO" or acessibilidade == "TERRA-PESADO" or acessibilidade == "TERRA E AR")):
            return True
        if(self.tipo == "AR" and (acessibilidade == "TUDO" or acessibilidade == "AR" or acessibilidade == "MAR E AR" or acessibilidade == "TERRA E AR")):
            return True
        if(self.tipo == "MAR" and (acessibilidade == "TUDO" or acessibilidade == "MAR" or acessibilidade == "MAR E AR")):
            return True
        return False

    def baseline(self,x):
        self.combustivel_disponivel = self.autonomia
        self.tempo_desde_inicio = x
    
    def get_name(self):
        return self.nome


    
