# Meu Eu Digital
- Salvador Duarte Magalhães Barreto - A104520
- André Filípe Soares Pereira - A104275

---

Este relatório apresenta o desenvolvimento de uma aplicação Web que consiste na criação de um **"eu digital"**, ou seja, um diário digital que permite ao utilizador registar diferentes tipos de conteúdos (fotografias, pensamentos, eventos, entre outros) de forma cronológica.

## Como executar com Docker
1. **Executar o Docker Compose**:
   ```bash
   docker-compose up -d
   ```

2. **Aceder à aplicação**:
   - Frontend: [http://localhost:3002](http://localhost:3002)
   - Backend API: Porta 3001 


---
## Funcionalidades Principais

### Sistema de Utilizadores

#### **Tipos de Conta**
- **Produtor**: Utilizador padrão que pode criar e gerir os seus próprios conteúdos, ver o conteúdo público dos outros e comentar.
- **Administrador**: Utilizador com privilégios para gestão de utilizadores e conteúdo.
#### **Autenticação e Registo**
- Registo com nome, username, email e password
- Login com username e password
- Autenticação via JWT tokens
- Gestão de sessões seguras
- Logout com limpeza de tokens

#### **Gestão de Perfis**
- **Produtores**: Podem editar o próprio perfil (nome, email, username)
- **Produtores**: Podem alterar a própria password
- **Administradores**: Podem editar perfis de qualquer utilizador
- **Administradores**: Podem eliminar utilizadores e todos os seus posts

### Sistema de Posts/Items

#### **Criação e Gestão**
- Upload de ficheiros SIP (Submission Information Package)
- Suporte para múltiplos tipos de ficheiros (imagens, documentos)
- Sistema de visibilidade (público/privado)

#### **Visualização e Interação**
- Homepage com feed de posts públicos
- Filtragem por classificadores
- Pesquisa avançada por título e classificadores
- Sistema de comentários 
- Perfis públicos de utilizadores

#### **Permissões**
- **Proprietários** (de posts): Podem eliminar os próprios posts e alterar visibilidade.
- **Administradores**: Podem eliminar qualquer post e alterar visibilidade dos mesmos. Podem editar e eliminar qualquer perfil exceto palavra-passse.
- **Guests**: Podem ver apenas posts públicos.
- **Utilizadores autenticados**: Podem comentar em posts públicos e editar ou eliminar o próprio perfil.

### Sistema SIP (Submission Information Package)



#### **Upload e Processamento**
- Interface de upload de ficheiros SIP em ZIP.
- Validação automática da estrutura SIP e processamento de manifesto JSON.
- Extração e armazenamento de ficheiros. Os ficheiros são armazenados no file system do Docker de backend e os metadados na base de dados mongo.

#### **Exportação**
- Exportação de posts como pacotes SIP.
- Geração automática de lista de utilizadores (para administradores).

#### **Estrutura SIP Suportada**

Aqui vale ressaltar que é obrigatório o nome do zip e da pasta serem "SIP", não podem tomar qualquer outro valor senão inconsistências na base de dados e persistência de ficheiros são garantidas.

```
SIP.zip
├── SIP/
│   ├── manifesto-SIP.json          # Manifesto principal
│   └── data/
│       └── [item]/    # 1 pasta por post
│           ├── [ficheiro1]              # Ficheiro de dados
│           └── [ficheiro1].json         # Metadados do item
```

###### Especificacao de formato de metadados -> "manifesto-SIP.txt"

### **Trabalho Futuro**

As principais funcionalidades que quereríamos implementar remetem para o enunciado original onde são mencionadas, mas, infelizmente, não conseguimos cumprir:
- Edição de posts 
- Login através de facebook/google e partilha automatizada
- Adicionar suporte de múltiplos ficheiros por post 
- Backend com maior privacidade (é possível aceder a ficheiros privados)
- Taxonomia definida 
- Estatísticas (visualização, descarregamento, etc.)
