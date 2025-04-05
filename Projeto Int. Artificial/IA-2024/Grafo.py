import math
from Veiculo import Veiculo
from typing import List

import networkx as nx  
import matplotlib.pyplot as plt 

from Node import Node

class Graph:
    ACESSIBILIDADES_VALIDAS = {"TUDO","MAR","AR","TERRA","TERRA-PESADO","MAR E AR","TERRA E AR"}

    def __init__(self):
        self.m_nodes = []  
        self.m_graph = {} 
        self.m_h = {}  # dicionario para posterirmente armazenar as heuristicas para cada nodo -> pesquisa informada

    def __str__(self):
        out = ""
        for key in self.m_graph.keys():
            out = out + "Localidade:" + str(key) + ": " + str(self.m_graph[key]) + "\n"
            return out

    def add_edge(self, node1 : Node, node2 : Node, weight,acessibilidade):
        if (node1 not in self.m_nodes):
            ##n1_id = len(self.m_nodes)  
            ##n1.setId(n1_id)
            self.m_nodes.append(node1)
            self.m_graph[node1.m_name] = []
        

        if (node2 not in self.m_nodes):
            ##n2_id = len(self.m_nodes) 
            ##n2.setId(n2_id)
            self.m_nodes.append(node2)
            self.m_graph[node2.m_name] = []
        
        if acessibilidade not in Graph.ACESSIBILIDADES_VALIDAS:
            raise ValueError(f"Acessibilidade inválida: {acessibilidade}. "
                             f"Valores permitidos: {Graph.ACESSIBILIDADES_VALIDAS}")
        self.m_graph[node1.m_name].append((node2,weight,acessibilidade))  


    def getNodes(self):
        return self.m_nodes

    def getNecessidades(self):
        res = 0
        for n in self.m_nodes:
            res += n.necessidades
        
        return res

    def get_arc_cost(self, node1 : Node, node2 : Node):
        if(node1 == node2):
            return 0
        
        custoT = math.inf
        a = self.m_graph[node1.m_name]
        for (nodo, custo, acessibilidade) in a:
            if nodo == node2:
                custoT = custo

        return custoT
    
    def get_arc_acessibilidade(self, node1 : Node, node2 : Node):
        if node1 == node2:
            return "TUDO"
        acessibilidadeT = None
        a = self.m_graph[node1.m_name] 
        for (nodo,_,acessibilidade) in a:
            if nodo == node2:
                acessibilidadeT = acessibilidade
                break
        return acessibilidadeT            

    def getNeighbours(self, nodo : Node):
        lista = []
        for (adjacente, peso, acessibilidade) in self.m_graph[nodo.m_name]:
            lista.append((adjacente, peso, acessibilidade))
        return lista

    def getNode(self,nome_nodo):
        for n in self.m_nodes:
            if n.m_name == nome_nodo:
                return n
        return None
   
    def desenha(self):
        # Criar o grafo
        g = nx.Graph()

        # Dicionário para armazenar a posição dos nós no layout
        pos = {}

        # Criar a lista de vértices
        lista_v = self.m_nodes
        c = 1
        for value in lista_v:
            if (c == 1):
                print(f"\033[34m{value}\033[0m", end=" ")
                c = 2
            else:
                print(f"\033[31m{value}\033[0m", end=" ")
                c = 1
        print("\n")

        # Loop através dos nós e suas listas de adjacência
        for nodo in lista_v:
            n = nodo.getName()

            # Se o nó não foi adicionado ao gráfico, adicione-o
            if n not in g:
                g.add_node(n)

            # Adicionar arestas de acordo com a lista de adjacência
            for (adjacente, peso, acessibilidade) in self.m_graph[nodo.m_name]:
                # Adicionar o nó adjacente se ainda não foi adicionado
                if adjacente not in g:
                    g.add_node(adjacente)

                # Adicionar a aresta entre o nó e o nó adjacente
                g.add_edge(n, adjacente, weight=peso)

        # Gerar layout para os nós
        pos = nx.spring_layout(g)

        # Desenhar o grafo com os nós e rótulos
        nx.draw_networkx(g, pos, with_labels=True, font_weight='bold')

        # Obter os rótulos das arestas (pesos) e desenhá-los
        labels = nx.get_edge_attributes(g, 'weight')
        nx.draw_networkx_edge_labels(g, pos, edge_labels=labels)

        # Exibir o gráfico
        plt.draw()
        plt.show()


    def calcula_custo(self, caminho):
        teste = caminho
        custo = 0
        i = 0
        while i + 1 < len(teste):
            custo = custo + self.get_arc_cost(teste[i], teste[i + 1])
            i = i + 1
        return custo
    
    def atualizar_grafo(self,caminho : List[Node],suplementos):
        i = 0
        while i < len(caminho) and suplementos > 0:
            nodo = caminho[i]
            suplementos_necessarios = nodo.necessidades 
            if (suplementos_necessarios > 0):
                if (suplementos > suplementos_necessarios):
                    suplementos -= suplementos_necessarios
                    nodo.necessidades = 0
                else: 
                    nodo.necessidades = (suplementos_necessarios - suplementos)
                    suplementos = 0
            i = i + 1
        return



##--------------------------------------------------Algoritmo de procura informada--------------------------------------------------##


    def heuristica(self,nodo_atual:Node,nodo_objetivo:Node,veiculo:Veiculo,ignorar_necessidades : bool):
        heuristica = 0
        
        if(not ignorar_necessidades and nodo_objetivo.necessidades == 0 and nodo_atual != nodo_objetivo): ## -2 indica que o nodo nao tem necessidades 
            return -2                                                                                     

        distancia = self.get_arc_cost(nodo_atual,nodo_objetivo) ##DISTANCIA
        heuristica += distancia

        var = veiculo.adaptar_tempo_a_condicao_meteriologica(nodo_objetivo.condicao_meteriologica) ##TEMPO DE VIAGEM 
        tempo_viagem = (distancia/veiculo.tempo_de_viagem) * var  +  veiculo.tempo_desde_inicio
        if(tempo_viagem > nodo_objetivo.janela_de_tempo):   ##SE ultrapassa janela de tempo retorna
            return -1
        else:
            heuristica *= nodo_objetivo.janela_de_tempo     ##Penalizar janelas de tempo maiores (menos criticas)
            if (heuristica > veiculo.tempo_de_viagem):      
                heuristica -= veiculo.tempo_de_viagem   ##Beneficiar quando tempo de viagem (velocidade) é maior
            if nodo_objetivo.perecivel == True:
                heuristica /= veiculo.tempo_de_viagem   ##Beneficiar se houver suprimentos pereciveis em causa

        acessibilidade_arco = self.get_arc_acessibilidade(nodo_atual,nodo_objetivo) ##ACESSIBILIDADE
        acessibilidade_nodo = nodo_objetivo.inacessivel
        if(acessibilidade_nodo or not veiculo.veiculo_tem_acesso(acessibilidade_arco)): 
            return -1
        else:
            heuristica *= veiculo.consumo * 2              ##Penalizar consumos de combustivel mais altos

        
            
        viagens_necessarias = 1
        if veiculo.cap_max < nodo_objetivo.necessidades:   
                viagens_necessarias = math.ceil(nodo_objetivo.necessidades / veiculo.cap_max)   ##Penalizar quando temos poucos suprimentos para o total necessario no nodo objetivo
                heuristica *= viagens_necessarias
        
        if nodo_objetivo.reabastecimento == True:
            combustivel_necessario = 1 * (veiculo.consumo * distancia)      ## Se poder reabastecer no nó objetivo só precisa de combustivel para ir
        else:                                                                 
            combustivel_necessario = 2 * (veiculo.consumo * distancia)      ## Senão precisa de 2x (ir e voltar)

        if veiculo.combustivel_disponivel < combustivel_necessario: ##Verificar se há combustivel suficiente
            return -1
        else:
            heuristica += (combustivel_necessario * 5)  ##Penalizar gasto de combustivel
            
        gravidade_heuristica = nodo_objetivo.gravidade_heuristica()  
        heuristica *= gravidade_heuristica              ##Penalizar gravidades baixas (pouca gravidade retorna multiplicador maior)

        fator_critico = nodo_objetivo.densidade_populacional * gravidade_heuristica  ##MAIOR PRIORIDADE
        fator_critico /= 10000
        heuristica = max(1,heuristica - fator_critico)  ## Garantir que a heuristica é >0 , Beneficiar maior fator critico
    
        return heuristica
        




    






 