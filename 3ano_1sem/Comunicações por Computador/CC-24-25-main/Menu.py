import NMS_Server
import NMS_Agent
import json
from pathlib import Path
import threading
import subprocess

class Menu:
    
    @staticmethod
    def print_estatisticas(filepath):
        with open(filepath,'r') as f:
            data = json.load(f)
        print(json.dumps(data, indent=4))
        
    @staticmethod    
    def menu_estatisticas():
        folder_path = Path("metricas")
        file_count = len([f for f in folder_path.iterdir() if f.is_file()])        
        if(file_count == 0):
            print("Não existem métricas para visualizar")
            return
        else:
            id_cliente = input("Introduza o ID do cliente que deseja visitar:\n(Ex: 1, 2, etc...)\n")
            filepath = "metricas/metricas_cliente_" + id_cliente + ".json"
            Menu.print_estatisticas(filepath)
        
    def menu_visualizar_clientes():
        folder_path = Path("metricas")
        file_count = len([f for f in folder_path.iterdir() if f.is_file()])  
        for i in range (file_count):
            id = i + 1
            print("Client " + str(id))
        input("Pressione qualquer tecla para sair")    
        return

    @staticmethod   
    def menu():
        while True:
            print("-----------MENU-----------:\n")
            print("1 -> Visualizar estatísticas\n")
            print("2 -> Visualizar lista de clientes\n")
            print("3 -> Encerrar Servidor:\n")
            opcao = input("Selecione uma das seguintes opções: ")
            print(f'Opção escolhida {opcao}\n')
            if(opcao == "1"):
                Menu.menu_estatisticas()
            elif(opcao == "2"):
                Menu.menu_visualizar_clientes()
            elif(opcao == "3"):
                break
            

if __name__ == "__main__":
    Menu.menu()