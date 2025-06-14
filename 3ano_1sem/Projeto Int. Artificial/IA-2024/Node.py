
class Node:
    CONDICOES_VALIDAS = {"BOA","MODERADA","MÁ"}
    GRAVIDADES_VALIDAS = {"MÁXIMA","ALTA","MÉDIA","MÍNIMA"}

    
    def __init__(self,name,gravidade,necessidades : int,densidade_populacional,condicao_meteriologica,reabastecimento,janela_de_tempo,perecivel,inacessivel=False):  
        self.m_name = str(name)
        if gravidade not in Node.GRAVIDADES_VALIDAS:
            raise ValueError(f"Gravidade inválida: {gravidade}. "
                             f"Valores permitidos: {Node.GRAVIDADES_VALIDAS}")
        self.gravidade = gravidade
        self.necessidades = necessidades  ##inteiro
        self.inacessivel = inacessivel
        self.janela_de_tempo = janela_de_tempo  ##inteiro - (horas)
        self.reabastecimento = reabastecimento  ##boleano
        self.densidade_populacional = densidade_populacional  ##numero de habitantes
        if condicao_meteriologica not in Node.CONDICOES_VALIDAS:
            raise ValueError(f"Condição meteorológica inválida: {condicao_meteriologica}. "
                             f"Valores permitidos: {Node.CONDICOES_VALIDAS}")
        self.condicao_meteriologica = condicao_meteriologica
        self.perecivel = perecivel

        ##Janela de tempo relativa ao menor tempo sempre 

    def alterar_necessidades(self,novo_valor):
        self.necessidades = novo_valor

    def gravidade_heuristica(self):
        match (self.gravidade):
            case "MÁXIMA":
                return 1
            case "ALTA":
                return 1.5
            case "MÉDIA":
                return 2
            case "MÍNIMA":
                return 3
        
        
    def __str__(self):
        return  self.m_name

    def __repr__(self):
        return  self.m_name

    def getName(self):
        return self.m_name

    def __eq__(self, other):
         return isinstance(other, Node) and self.m_name == other.m_name

    def __hash__(self):
        return hash((self.m_name, self.gravidade, self.necessidades,
                 self.densidade_populacional, self.condicao_meteriologica,
                 self.reabastecimento, self.janela_de_tempo, self.inacessivel))
    