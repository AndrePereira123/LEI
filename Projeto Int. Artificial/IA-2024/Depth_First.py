
from Grafo import Graph
from Node import Node
from Veiculo import Veiculo
from typing import List
import copy

JANELA_PERECIVEL = 100

class DFS:
##--------------------------------------------------Algoritmo de procura cega--------------------------------------------------##

    def procura_DFS(g:Graph,start:Node,veiculo:Veiculo,suprimentos_inicial,suprimentos_normal,suprimentos_pereciveis,janela_perecivel,path,visited,tempo_percorrido):  ##fazer com memoria
    
        path.append(start)
        visited.append(start)
        veiculo.tempo_desde_inicio = tempo_percorrido
        
        if(janela_perecivel < tempo_percorrido):
            suprimentos_inicial -= suprimentos_pereciveis                   ##verificacoes de janelas e de combustivel e de vizinhos
            suprimentos_pereciveis = 0

        if(start.janela_de_tempo >= tempo_percorrido and not start.perecivel):
            suprimentos_normal = max(0,suprimentos_normal - start.necessidades)
        
        if(start.perecivel):
            suprimentos_pereciveis = max(0,suprimentos_pereciveis - start.necessidades)
            
        if (suprimentos_normal + suprimentos_pereciveis) == 0 or veiculo.combustivel_disponivel == 0 or g.getNeighbours(start) == []:
            custoT = g.calcula_custo(path)
            return (custoT,path,suprimentos_inicial - (suprimentos_normal + suprimentos_pereciveis))

        if(start.reabastecimento):
            veiculo.combustivel_disponivel = veiculo.autonomia
        
        
        for(adjacente,distancia,acessibilidade) in g.m_graph[start.m_name]: ##tenta visitar vizinhos
                if adjacente not in visited:
                    if veiculo.veiculo_tem_acesso(acessibilidade):
                        var = veiculo.adaptar_tempo_a_condicao_meteriologica(adjacente.condicao_meteriologica)
                        combustivel_necessario = 2 * veiculo.consumo * distancia
                       
                        if(veiculo.combustivel_disponivel >= combustivel_necessario):
                            veiculo.combustivel_disponivel -= combustivel_necessario 
                        
                            tempo_percorrido += (distancia / veiculo.tempo_de_viagem) * var
                            resultado = DFS.procura_DFS(g,adjacente,veiculo,suprimentos_inicial,suprimentos_normal,suprimentos_pereciveis,janela_perecivel,path,visited,tempo_percorrido)
                        
                            return resultado
                        
        if (len(path) > 1):  ##nao consegue visitar ninguem mas visitou antes
            custoT = g.calcula_custo(path)
            return (custoT,path,suprimentos_inicial - (suprimentos_normal + suprimentos_pereciveis))
        return None
    
    def algoritmo_total_DFS(g:Graph,nodo_inicial,veiculos_input:List[Veiculo],suplementos_requisitados):

        veiculos = copy.deepcopy(veiculos_input)
        veiculos_nao_usados = []
        nodos_a_visitar = [nodo.m_name for nodo in g.m_nodes]
        nodos_com_necessidades = []
        rotas = []
        
        if (suplementos_requisitados > 0):
            for v in veiculos:
                resultado = DFS.procura_DFS(g,nodo_inicial,v,v.cap_max,0.7*v.cap_max,0.3*v.cap_max,JANELA_PERECIVEL,[],[],0) 
                if (resultado != None):
                    (custo_caminho,path,suplementos_distribuidos) = resultado
                    if suplementos_distribuidos == 0:
                        veiculos_nao_usados.append(v.get_name())
                    else:
                        g.atualizar_grafo(path,suplementos_distribuidos)

                        suplementos_requisitados -= suplementos_distribuidos ## atualizar grafo e suplementos que ainda precisamos
                        for n in path:
                            if n.m_name in nodos_a_visitar: nodos_a_visitar.remove(n.m_name)

                        rotas.append((0,custo_caminho,suplementos_distribuidos,round(v.tempo_desde_inicio,2),path,v.nome,v.cap_max))
                else:
                    veiculos_nao_usados.append(v.get_name())
        else:
            for v in veiculos:
                if v.get_name() not in veiculos_nao_usados:
                    veiculos_nao_usados.append(v.get_name())

                
        for n in g.m_nodes:
            if n.necessidades > 0:
                nodos_com_necessidades.append((n.m_name,n.necessidades))
        
        
        rotas.append((suplementos_requisitados,nodos_a_visitar,veiculos_nao_usados,nodos_com_necessidades))

        return rotas