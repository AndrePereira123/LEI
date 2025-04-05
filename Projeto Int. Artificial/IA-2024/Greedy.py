
from Grafo import Graph
from Node import Node
from Veiculo import Veiculo
from typing import List
import copy
import random

class Greedy:
    def algoritmo_personalizado_greedy(grafo:Graph,nodo_inicial:Node,veiculo:Veiculo,suplementos):
        suplementos_inicial = suplementos ##suplementos q vai entregar

        heuristica_total = 0
        heuristica_n = 0

        open_list = {nodo_inicial}
        closed_list = set([])
        n_anterior = nodo_inicial
        g = {}  
        g[nodo_inicial] = 0    # distancias ate nodos
        
        parents = {}
        parents[nodo_inicial] = nodo_inicial

        total_nodes = len(grafo.getNodes())
        can_stop = False

        while len(open_list) > 0:
            tam_openlist = len(open_list)
            visinhos_locais_necessidades = False
            visinhos_possiveis = [] 
            n = None
            for v in open_list:
                heuristica_vizinho = grafo.heuristica(n_anterior,v,veiculo,False)
                if (heuristica_vizinho == -2):                
                    visinhos_possiveis.append(v) ## juntamos visinhos sem necessidades
                if(heuristica_vizinho > 0):
                    visinhos_locais_necessidades = True
                    if n == None or heuristica_vizinho < grafo.heuristica(n_anterior,n,veiculo,False):  
                        heuristica_n = heuristica_vizinho
                        n = v
            
            if (not visinhos_locais_necessidades):  ## nenhum visinho foi selecionado - procuramos um dos que nao tem necessidades
                lista_de_opcoes = []
                for v in visinhos_possiveis:
                    heuristica_vizinho = grafo.heuristica(n_anterior,v,veiculo,True)
                    if(not (heuristica_vizinho < 0)):
                        lista_de_opcoes.append(v)
                if(lista_de_opcoes != []):
                    n = random.choice(lista_de_opcoes)
                    distancia = grafo.get_arc_cost(n_anterior,n) 

                    heuristica_total += grafo.heuristica(n_anterior,n,veiculo,True)

                    if(n.reabastecimento == True): 
                        veiculo.combustivel_disponivel = veiculo.autonomia ## sabemos q é possivel chegar ao nodo pela heuristica e que vamos reabastecer entao
                    else:
                        combustivel_necessario = 2 * (veiculo.consumo * distancia)  ##
                        veiculo.combustivel_disponivel -= combustivel_necessario

                    var = veiculo.adaptar_tempo_a_condicao_meteriologica(n.condicao_meteriologica) ##Tempo até um nó não deve ultrapassar o limite
                    tempo_gasto = (distancia/veiculo.tempo_de_viagem) * var
                    veiculo.tempo_desde_inicio += tempo_gasto

                    resultado = Greedy.algoritmo_personalizado_greedy(grafo,n,veiculo,suplementos_inicial)
                    if resultado == None: return None
                    (heuristica_total,custo,caminho,suplementos_entregues) = resultado

                    reconst_path = []

                    while parents[n_anterior] != n_anterior:
                        reconst_path.append(n_anterior)
                        n_anterior = parents[n_anterior]

                    reconst_path.append(nodo_inicial)

                    reconst_path.reverse()
                    cam = reconst_path[:]
                    cam.append(caminho[0])
                    return (heuristica_total,custo + grafo.calcula_custo(cam), reconst_path + caminho, suplementos_entregues)
            

            heuristica_total += heuristica_n
            if n == None:
                can_stop = True
                n = n_anterior

            if not can_stop:
                for (m, weight, acessibilidade) in grafo.getNeighbours(n):  
                # se o nó atual não estiver em open_list e closed_list, adicione-se a open_list 
                    if m not in open_list and m not in closed_list:
                        open_list.add(m)
                        parents[m] = n
                        g[m] = g[n] + weight

                    else:
                        if g[m] > g[n] + weight:
                            g[m] = g[n] + weight
                            parents[m] = n

                            if m in closed_list:
                                closed_list.remove(m)
                                open_list.add(m)
                                  
                if(len(open_list) == tam_openlist):
                    can_stop = True

                distancia = grafo.get_arc_cost(n_anterior,n) 

                if(n.reabastecimento == True): 
                    veiculo.combustivel_disponivel = veiculo.autonomia ## sabemos q é possivel chegar ao nodo pela heuristica e que vamos reabastecer entao
                else:
                    combustivel_necessario = 2 * (veiculo.consumo * distancia)  ##
                    veiculo.combustivel_disponivel -= combustivel_necessario

                var = veiculo.adaptar_tempo_a_condicao_meteriologica(n.condicao_meteriologica) ##Tempo até um nó não deve ultrapassar o limite
                tempo_gasto = (distancia/veiculo.tempo_de_viagem) * var
                veiculo.tempo_desde_inicio += tempo_gasto

                if (suplementos > n.necessidades):
                    suplementos -= n.necessidades
                else:
                    suplementos = 0

            if suplementos == 0 or len(closed_list) == total_nodes or can_stop:

                suplementos_entregues = suplementos_inicial - suplementos

                eficiencia = round(suplementos_entregues/suplementos_inicial * 100,1)
                if (eficiencia == 0): 
                    return None
                
                reconst_path = []

                while parents[n] != n:
                    reconst_path.append(n)
                    n = parents[n]

                reconst_path.append(nodo_inicial)

                reconst_path.reverse()
                
                heuristica_total *= (1/eficiencia)  ##Beneficiar eficiencia (aproveitar capacidade do veiculo)
                

                return (heuristica_total,grafo.calcula_custo(reconst_path), reconst_path, suplementos_entregues)
            

            
            n_anterior = n
            open_list.remove(n)
            closed_list.add(n)

        return None


    def algoritmo_total_greedy(grafo:Graph,nodo_inicial,veiculos_input:List[Veiculo],suplementos_requisitados):

        veiculos = copy.deepcopy(veiculos_input)
        n_veiculos = len(veiculos)
        veiculos_nao_usados = []
        nodos_a_visitar = [nodo.m_name for nodo in grafo.m_nodes]
        nodos_com_necessidades = []
        rotas = []
        prim_itera = 0
        melhor_path = []
        melhor_distribuicao = 0
        for i in range (n_veiculos):
            if (suplementos_requisitados > 0):
                melhor_heuristica = 0
                melhor_veiculo = None
                prim_itera = True
                for v in veiculos[:]:  ##calculamos a heuristica de todos os veiculos e guardamos a melhor(menor)ç
                    tempo_salvo = v.tempo_desde_inicio
                    if (v in veiculos):
                        resultado = Greedy.algoritmo_personalizado_greedy(grafo,nodo_inicial,v,v.cap_max)  ##altera tempo desde inicio e combustivel gasto do veiculo 
                        if (resultado != None):
                            (nova_h,custo_caminho,path,suplementos_distribuidos) = resultado
                            if prim_itera == True or nova_h < melhor_heuristica:
                                prim_itera = False
                                melhor_heuristica = nova_h
                                custo_melhor_caminho = custo_caminho
                                melhor_path = path
                                if melhor_veiculo is not None:
                                    melhor_veiculo.baseline(melhor_tempo_salvo)
                                melhor_tempo_salvo = tempo_salvo
                                melhor_veiculo = v
                                melhor_distribuicao = suplementos_distribuidos
                            else:
                                v.baseline(tempo_salvo)
                        else:
                            veiculos_nao_usados.append(v.get_name())
                            veiculos.remove(v)
                
                
                if(melhor_veiculo != None):
                    veiculos.remove(melhor_veiculo)
                    
                    if melhor_distribuicao == 0:
                        veiculos_nao_usados.append(melhor_veiculo.get_name())
                    else:
                        grafo.atualizar_grafo(melhor_path,melhor_distribuicao)

                        suplementos_requisitados -= melhor_distribuicao ## atualizar grafo e suplementos que ainda precisamos
                        
                        for n in melhor_path:
                            if n.m_name in nodos_a_visitar: nodos_a_visitar.remove(n.m_name)

                        rotas.append((round(melhor_heuristica,2),custo_melhor_caminho,melhor_distribuicao,round(melhor_veiculo.tempo_desde_inicio,2),melhor_path,melhor_veiculo.nome,melhor_veiculo.cap_max))
            else:
                for v in veiculos:
                    if v.get_name() not in veiculos_nao_usados:
                        veiculos_nao_usados.append(v.get_name())

        for n in grafo.m_nodes:
            if n.necessidades > 0:
                nodos_com_necessidades.append((n.m_name,n.necessidades))

        
        rotas.append((suplementos_requisitados,nodos_a_visitar,veiculos_nao_usados,nodos_com_necessidades))
        return rotas
