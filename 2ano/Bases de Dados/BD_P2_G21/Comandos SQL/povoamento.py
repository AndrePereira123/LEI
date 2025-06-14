import random
import mysql.connector

def gerar_numero_telefone():
    prefixo = random.choice([91, 92, 93])
    sufixo = random.randint(1000000, 9999999)
    numero = int(f"{prefixo}{sufixo}")
    return numero

def insert_tecnico(sql, total):
    for x in range(0, total): #ciclo que repete total vezes para inserir diferentes técnicos
        nome_aleatorio = random.choice(nome) #nome aleatório utilizando a biblioteca random
        sobrenome_aleatorio = random.choice(sobrenome) #sobrenome aleatório

        #campos Nome, Email e Telefone, um por cada linha
        val = [(
            nome_aleatorio + " " + sobrenome_aleatorio,
            nome_aleatorio + sobrenome_aleatorio + str(random.randint(1, 40)) + random.choice(email),
            gerar_numero_telefone()
        )
        ]

        #executa o comando sql na base de dados dada
        mycursor.executemany(sql, val)


def insert_client(sql, total):
    for x in range(0, total): #ciclo que repete total vezes para inserir diferentes clientes
        nome_aleatorio = random.choice(nome) #nome aleatório
        sobrenome_aleatorio = random.choice(sobrenome) #sobrenome aleatório
        sexo = sexo_por_nome.get(nome_aleatorio, 0) #vai buscar o sexo conforme o nome gerado aleatoriamente

        #campos Nome, Sexo e Email, um por cada linha
        val = [(
            nome_aleatorio + " " + sobrenome_aleatorio,
            sexo,
            nome_aleatorio + sobrenome_aleatorio + str(random.randint(1, 40)) + random.choice(email),
        )
        ]

        #executa o comando sql na base de dados dada
        mycursor.executemany(sql, val)


#emails
email = [
    "@gmail.com",
    "@hotmail.com",
    "@sapo.pt",
    "@uminho.pt",
    "@icloud.com"
]

#nomes
nome = [
    "João",
    "Pedro",
    "José",
    "António",
    "Manuel",
    "Francisco",
    "Luís",
    "Carlos",
    "Miguel",
    "Rafael",
    "Maria",
    "Ana",
    "Joana",
    "Sofia",
    "Margarida",
    "Beatriz",
    "Inês",
    "Carolina",
    "Matilde",
    "Catarina"
]

#sexo por nome
sexo_por_nome = {
    "João": 'M', "Pedro": 'M', "José": 'M', "António": 'M', "Manuel": 'M', "Francisco": 'M', "Luís": 'M', "Carlos": 'M', "Miguel": 'M', "Rafael": 'M',
    "Maria": 'F', "Ana": 'F', "Joana": 'F', "Sofia": 'F', "Margarida": 'F', "Beatriz": 'F', "Inês": 'F', "Carolina": 'F', "Matilde": 'F', "Catarina": 'F'
}

#sobrenomes
sobrenome = [
    "Silva",
    "Santos",
    "Ferreira",
    "Pereira",
    "Oliveira",
    "Costa",
    "Rodrigues",
    "Martins",
    "Jesus",
    "Sousa",
    "Gomes",
    "Marques",
    "Almeida",
    "Ribeiro",
    "Carvalho",
    "Lopes",
    "Pinho",
    "Azevedo",
    "Nunes",
    "Mendes",
    "Vieira",
    "Monteiro",
    "Cardoso",
    "Rocha",
    "Correia",
    "Faria",
    "Simões",
    "Fonseca",
    "Teixeira",
    "Barbosa"
]

mydb = mysql.connector.connect( #conectar à nossa base de dados
    host="localhost",
    user="root",
    password="root",
    database="AbrilEmFlor"
)

mycursor = mydb.cursor()

sql = "INSERT INTO Técnico (Nome, Email, Telefone) VALUES (%s, %s, %s)" #tipo de operação para inserir dados na entidade técnico 

insert_tecnico(sql, 10) #função para inserir 10 técnicos aleatórios

sql_cliente = "INSERT INTO Cliente (Nome, Sexo, Email) VALUES (%s, %s, %s)" #tipo de operação para inserir dados na entidade cliente 

insert_client(sql_cliente, 10) #função para inserir 10 clientes aleatórios


mydb.commit() #insere os dados na base de dados

