from Grafo import Graph
from Node import Node
from Veiculo import Veiculo
from Greedy import Greedy
from Algoritmo_personalizado import Algoritmo_p
from Depth_First import DFS
from typing import List
import copy



def main():
    g = Graph()
    ##Variaveis para alterar comportamento##
    M1 = 1 ##Janela de Tempo
    M2 = 1 ##Necessidades
    R = True ## Por em false para que ninguem reabasteça
    ###############################################

    Aveiro = Node(name="Aveiro", gravidade="MÉDIA", necessidades=2380*M2, densidade_populacional=68560, 
              condicao_meteriologica="MODERADA", reabastecimento=True & R, janela_de_tempo=26*M1,perecivel = True)
    Funchal = Node(name="Funchal", gravidade="ALTA", necessidades=1300*M2, densidade_populacional=105701, 
                condicao_meteriologica="BOA", reabastecimento=True & R, janela_de_tempo=30*M1,perecivel = False)
    Ponta_Delgada = Node(name="Ponta Delgada", gravidade="ALTA", necessidades=1250*M2, densidade_populacional=67229, 
                        condicao_meteriologica="BOA", reabastecimento=True & R, janela_de_tempo=40*M1,perecivel = False)
    Angra_do_Heroísmo = Node(name="Angra do Heroísmo", gravidade="MÉDIA", necessidades=1150*M2, densidade_populacional=7953, 
                            condicao_meteriologica="MODERADA", reabastecimento=False & R, janela_de_tempo=32*M1,perecivel = True)
    Horta = Node(name="Horta", gravidade="MÍNIMA", necessidades=1100*M2, densidade_populacional=14356, 
                condicao_meteriologica="MÁ", reabastecimento=False & R, janela_de_tempo=34*M1,perecivel = False)
    Lisboa = Node(name="Lisboa", gravidade="MÍNIMA", necessidades=0*M2, densidade_populacional=545142, 
                condicao_meteriologica="BOA", reabastecimento=True & R, janela_de_tempo=300*M1,perecivel = False)
    Porto = Node(name="Porto", gravidade="ALTA", necessidades=3120*M2, densidade_populacional=231800, 
                condicao_meteriologica="MODERADA", reabastecimento=True & R, janela_de_tempo=30*M1,perecivel = True)
    Coimbra = Node(name="Coimbra", gravidade="MÁXIMA", necessidades=1200*M2, densidade_populacional=106768, 
                condicao_meteriologica="MÁ", reabastecimento=False & R, janela_de_tempo=18*M1,perecivel = False)
    Faro = Node(name="Faro", gravidade="MÍNIMA", necessidades=1100*M2, densidade_populacional=67859, 
                condicao_meteriologica="BOA", reabastecimento=True & R, janela_de_tempo=30*M1,perecivel = True)
    Braga = Node(name="Braga", gravidade="ALTA", necessidades=2800*M2, densidade_populacional=148977, 
             condicao_meteriologica="MODERADA", reabastecimento=True & R, janela_de_tempo=22*M1,perecivel = False)
    Évora = Node(name="Évora", gravidade="MÉDIA", necessidades=1590*M2, densidade_populacional=53577, 
                condicao_meteriologica="MÁ", reabastecimento=False & R, janela_de_tempo=25*M1,perecivel = False)
    Guarda = Node(name="Guarda", gravidade="MÍNIMA", necessidades=1800*M2, densidade_populacional=25833, 
                condicao_meteriologica="BOA", reabastecimento=False & R, janela_de_tempo=35*M1,perecivel = True)
    Viseu = Node(name="Viseu", gravidade="MÁXIMA", necessidades=1400*M2, densidade_populacional=110000, 
                condicao_meteriologica="MODERADA", reabastecimento=True & R, janela_de_tempo=16*M1,perecivel = False)
    Leiria = Node(name="Leiria", gravidade="ALTA", necessidades=1220*M2, densidade_populacional=128616, 
                condicao_meteriologica="BOA", reabastecimento=False & R, janela_de_tempo=28*M1,perecivel = False)
    Aveiro = Node(name="Aveiro", gravidade="MÉDIA", necessidades=3120*M2, densidade_populacional=68560, 
                condicao_meteriologica="MODERADA", reabastecimento=True & R, janela_de_tempo=26*M1,perecivel = True)
    Castelo_Branco = Node(name="Castelo Branco", gravidade="ALTA", necessidades=1200*M2, densidade_populacional=56000, 
                        condicao_meteriologica="MÁ", reabastecimento=False & R, janela_de_tempo=33*M1,perecivel = False)
    Póvoa_de_Varzim = Node(name="Póvoa de Varzim", gravidade="MÍNIMA", necessidades=1120*M2, densidade_populacional=63408, 
                        condicao_meteriologica="BOA", reabastecimento=True & R, janela_de_tempo=30*M1,perecivel = False)
    Covilhã = Node(name="Covilhã", gravidade="MÁXIMA", necessidades=1180*M2, densidade_populacional=46457, 
                condicao_meteriologica="MODERADA", reabastecimento=False & R, janela_de_tempo=35*M1,perecivel = True)
    Washington = Node(name="Washington", gravidade="MÁXIMA", necessidades=100*M2, densidade_populacional=678972, 
                condicao_meteriologica="MODERADA", reabastecimento=False & R, janela_de_tempo=45*M1,perecivel = False)
    Cuba = Node(name="Cuba", gravidade="ALTA", necessidades=100*M2, densidade_populacional=11190000, 
                    condicao_meteriologica="MODERADA", reabastecimento=True & R, janela_de_tempo=49*M1,perecivel = False)
    NovaYorkae = Node(name="Nova York", gravidade="MÉDIA", necessidades=100*M2, densidade_populacional=8258000, 
                            condicao_meteriologica="BOA", reabastecimento=True & R, janela_de_tempo=42*M1,perecivel = True)
    SãoPaulo = Node(name="São Paulo", gravidade="MÁXIMA", necessidades=100*M2, densidade_populacional=11450000, 
                condicao_meteriologica="MÁ", reabastecimento=False & R, janela_de_tempo=56*M1,perecivel = False)


    g.add_edge(Lisboa, Porto, 312, "TERRA-PESADO")
    g.add_edge(Lisboa,Braga, 364, "TERRA-PESADO")
    g.add_edge(Lisboa, Coimbra, 205, "TERRA E AR")

    g.add_edge(Lisboa, Faro, 277, "TERRA E AR")
    g.add_edge(Lisboa, Évora, 133, "TERRA E AR")
    g.add_edge(Lisboa, Guarda, 319, "TERRA E AR")
    g.add_edge(Lisboa, Viseu, 293, "TERRA E AR")

    g.add_edge(Lisboa,Covilhã, 278, "AR")
    g.add_edge(Lisboa,Póvoa_de_Varzim, 347, "AR")  ##estradas cortadas 
    g.add_edge(Lisboa,Castelo_Branco, 224, "AR")

    g.add_edge(Coimbra, Évora, 242, "TERRA E AR")
    g.add_edge(Évora,Porto, 402, "TERRA-PESADO")
    g.add_edge(Braga, Guarda, 252, "TERRA E AR")
    g.add_edge(Guarda, Braga, 252, "TERRA E AR")

    g.add_edge(Guarda, Faro, 555, "TERRA E AR")
    
    g.add_edge(Lisboa, Ponta_Delgada, 1458, "MAR E AR")
    g.add_edge(Ponta_Delgada, Washington, 4400, "MAR E AR")
    g.add_edge(Washington, NovaYorkae, 330, "TERRA E AR")

    g.add_edge(NovaYorkae, Cuba, 2145, "MAR E AR")

    g.add_edge(Cuba ,SãoPaulo, 6000, "MAR E AR")
    g.add_edge(Ponta_Delgada, Funchal, 39, "MAR E AR")
    g.add_edge(Funchal, Angra_do_Heroísmo, 1192, "MAR E AR")
    g.add_edge(Ponta_Delgada, Horta, 292, "MAR E AR")
    g.add_edge(Angra_do_Heroísmo, Horta, 138, "MAR E AR")


    g.add_edge(Faro, Leiria, 381, "TERRA E AR")
    g.add_edge(Leiria, Lisboa, 147, "TERRA E AR")
    g.add_edge(Porto, Coimbra, 118, "TERRA E AR")
    g.add_edge(Porto, Faro, 549, "TERRA E AR")

    g.add_edge(Porto, Braga, 54, "TERRA E AR")
    g.add_edge(Porto, Viseu, 128, "TERRA E AR")

    g.add_edge(Porto, Aveiro, 75, "TERRA E AR")
    g.add_edge(Coimbra, Faro, 441, "TERRA E AR")
    g.add_edge(Coimbra, Braga, 169, "TERRA E AR")
    g.add_edge(Coimbra, Leiria, 73, "TERRA E AR")
    g.add_edge(Coimbra, Aveiro, 60, "TERRA E AR") 
    g.add_edge(Braga, Faro, 601, "TERRA E AR")
    g.add_edge(Braga, Évora, 456, "TERRA E AR")
    
    g.add_edge(Braga, Viseu, 178, "TERRA E AR")
    g.add_edge(Faro, Aveiro, 490, "TERRA E AR")

    g.add_edge(Évora, Guarda, 290, "TERRA E AR")
    g.add_edge(Évora, Viseu, 282, "TERRA E AR")
    g.add_edge(Leiria, Aveiro, 124, "TERRA E AR")
    g.add_edge(Castelo_Branco, Lisboa, 224, "TERRA E AR")
    g.add_edge(Póvoa_de_Varzim, Porto, 34, "TERRA E AR")
    g.add_edge(Covilhã, Guarda, 52, "TERRA E AR")
    
    ##TERRA
    Carro1 = Veiculo("TERRA","Carro1",1500,60,100,0.07) ## 1500 kilos carga  60 litros de combus.   100 km/h   0.07 litros /km
    Carro2 = Veiculo("TERRA","Carro2",1500,60,100,0.07)
    Carro3 = Veiculo("TERRA","Carro3",1500,60,100,0.07) 
    Carro4 = Veiculo("TERRA","Carro4",1500,60,100,0.07)
    Carro5 = Veiculo("TERRA","Carro5",1500,60,100,0.07) 
    Carro6 = Veiculo("TERRA","Carro6",1500,60,100,0.07)
    TERRA = [Carro1,Carro2,Carro3,Carro4,Carro5,Carro6]

    ##TERRA PESADO
    Camiao1 = Veiculo("TERRA-PESADO","Camiao1",10000,300,80,0.35)
    Camiao2 = Veiculo("TERRA-PESADO","Camiao2",10000,300,80,0.35)
    Camiao3 = Veiculo("TERRA-PESADO","Camiao3",10000,300,80,0.35)
    Camiao4 = Veiculo("TERRA-PESADO","Camiao4",10000,300,80,0.35)
    Camiao5 = Veiculo("TERRA-PESADO","Camiao5",10000,300,80,0.35)
    Camiao6 = Veiculo("TERRA-PESADO","Camiao6",10000,300,80,0.35)
    TERRA_PESADO = [Camiao1,Camiao2,Camiao3,Camiao4,Camiao5,Camiao6]

    ##AR
    Avioneta1 = Veiculo("AR","Avioneta1",1000,200,200,0.1)
    Avioneta2 = Veiculo("AR","Avioneta2",1000,200,200,0.1)
    Avioneta3 = Veiculo("AR","Avioneta3",1000,200,200,0.1)
    Avioneta4 = Veiculo("AR","Avioneta4",1000,200,200,0.1)
    Avioneta5 = Veiculo("AR","Avioneta5",1000,200,200,0.1)
    Avioneta6 = Veiculo("AR","Avioneta6",1000,200,200,0.1)
    Drone1    = Veiculo("AR","Drone1"   ,50,15,400,0.008)
    Drone2    = Veiculo("AR","Drone2"   ,50,15,400,0.008)
    AR = [Avioneta1,Avioneta2,Avioneta3,Avioneta4,Avioneta5,Avioneta6,Drone1,Drone2]

    ##MAR
    Barquinho1 = Veiculo("MAR","Barquinho1",2000,450,55,0.1)
    Barquinho2 = Veiculo("MAR","Barquinho2",2000,450,55,0.1)
    Barquinho3 = Veiculo("MAR","Barquinho3",2000,450,55,0.1)
    Barquinho4 = Veiculo("MAR","Barquinho4",2000,450,55,0.1)
    Barquinho5 = Veiculo("MAR","Barquinho5",2000,450,55,0.1)
    MAR = [Barquinho1,Barquinho2,Barquinho3,Barquinho4,Barquinho5]

 
    Veiculos = TERRA + TERRA_PESADO + AR + MAR

    def imprimir_resultados(g :Graph,tipo_procura,Veiculos : List[Veiculo],alterar_grafo):
            if alterar_grafo == False:
                instancia_grafo = copy.deepcopy(g)
            else:
                instancia_grafo = g
            suplementos_necessarios = instancia_grafo.getNecessidades()
            if tipo_procura == "personalizado":
                resultado = Algoritmo_p.algoritmo_total(instancia_grafo,instancia_grafo.getNode("Lisboa"),Veiculos,suplementos_necessarios)
            elif tipo_procura == "pers_greedy":
                resultado = Greedy.algoritmo_total_greedy(instancia_grafo,instancia_grafo.getNode("Lisboa"),Veiculos,suplementos_necessarios)
            else:
                resultado = DFS.algoritmo_total_DFS(instancia_grafo,instancia_grafo.getNode("Lisboa"),Veiculos,suplementos_necessarios)

            print(f"\033[34m\nVeiculo   \033[0m" + f"\033[31m Heuristica \033[0m" + f"\033[32m Distancia \033[0m" + f"\033[34m Entregas      \033[0m" + f"\033[31m  Horas \033[0m" + f"\033[33m Caminho\033[0m\n")
            for i, r in enumerate(resultado):
                if i != len(resultado) - 1:  
                    if len(r) != 2:
                        eficiencia = round(r[2]/r[6] * 100,1)
                        print(f"\033[34m{r[5]:10}\033[0m" + f"\033[31m {r[0]:<11}\033[0m" + f"\033[32m {r[1]:<10}\033[0m" + f"\033[34m {r[2]:<7} {eficiencia:>5}% \033[0m" + f"\033[31m {r[3]:<6}\033[0m" + f"\033[33m {r[4]}\033[0m")
                        if (alterar_grafo == True):
                            for v in Veiculos:
                                if v.nome == r[5]:
                                    if (v.tempo_desde_inicio == 0):
                                        v.tempo_desde_inicio += r[3]*2
                                    else:
                                        v.tempo_desde_inicio += (r[3] - v.tempo_desde_inicio)*2
                                    
                                

                else:                                                   ## Último elemento - Aqui o r tem "estatisticas"
                    supl_entregues = suplementos_necessarios - r[0]
                    entrega_max = 0
                    for v in Veiculos:
                        if v.get_name() not in r[2]:
                            entrega_max += v.cap_max
                        
                    eficiencia = 0
                    if entrega_max > 0:
                        eficiencia = round(supl_entregues/entrega_max * 100,2)
                    
                    print(f"\n\033[1mSuplementos entregues = {supl_entregues}/{suplementos_necessarios} - Capacidade dos veiculos = {entrega_max}({eficiencia}% eficiencia)")
                    print(f"Nodos nao visitados: {r[1]}")
                    print(f"Veiculos nao usados: {r[2]}")
                    print(f"Nodos com necessidades: {r[3]}\033[0m")
                    
                        

    saida = -1
    Iterativo = False
    while saida != 0:
        print("1-Imprimir Grafo")
        print("2-Desenhar Grafo")
        print("3-Imprimir Nomes de Nodos")
        print("4-Algoritmo Personalizado")
        print("5-Algoritmo Personalizado Greedy (best)")
        print("6-Algoritmo DFS")
        if Iterativo == False:
            print("7-Seleciona para alterar", end= " ")
            print("\033[32m O grafo não será alterado\033[0m")
        else:
            print("7-Seleciona para alterar", end= " ")
            print("\033[31m O grafo será alterado\033[0m")
        print("0-Sair")
  
        saida = int(input("introduza a sua opcao-> "))
        if saida == 0:
            print("A sair......")
        elif saida == 1:
            c = 1
            for key, value in g.m_graph.items():
                if (c == 1):
                    print(f"\033[34m{key}: {value}\033[0m")
                    c = 2
                else:
                    print(f"\033[31m{key}: {value}\033[0m")
                    c = 1
            l = input("Prima algo para continuar")
        elif saida == 2:
            g.desenha()
        elif saida == 3:
            print(g.m_graph.keys())
            l = input("Prima algo para continuar")
        elif saida == 4:
            imprimir_resultados(g,"personalizado",Veiculos,Iterativo)
            l = input("Prima algo para continuar")
        elif saida == 5:
            imprimir_resultados(g,"pers_greedy",Veiculos,Iterativo)
            l = input("Prima algo para continuar")
        elif saida == 6:
            imprimir_resultados(g,"DFS",Veiculos,Iterativo)
            l = input("Prima algo para continuar")
        elif saida == 7:
            Iterativo = not Iterativo
        else:
            print("Input Invalido.")
            l = input("Prima algo para continuar")

if __name__ == "__main__":
    main()

