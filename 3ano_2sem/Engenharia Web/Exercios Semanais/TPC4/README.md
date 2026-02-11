# PL2025-A104275 ||  André Filipe Soares Pereira || A104275
# TPC-4 -  Cinema (serviço em express)


Para a quarta semana foi pedida a conclusão da implementação de um serviço dependente do express, onde utilizamos um dataset ["cinema"](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC4/dataset_inicial.json) onde existe uma lista de filmes com os atributos:
 - title 
 - year
 - cast
 - genres

Vale ressaltar que não existe um id associado a cada filmes, logo, para facilitar na utilização do axios principalmente no que toca ao remover (DELETE) de entradas, adicionei ids a todos os filmes através de um [script](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC4/add_id_json.py) em python.

Uma boa parte do serviço foi implementado no decorrer da aula teórica, deste modo as tarefas a realizar eram:
 - Adicionar edição e remoção de filmes;
 - Adicionar uma página de ator onde se listem todos os filmes em que participa.

# Implementação

Para permitir editar e apagar filmes do dataset, adicionei 2 botões, "Editar" e "Apagar", na página de lista de filmes. 

 - [Editar](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc4/editar.htm)
Ao clicar em editar é apresentado um formulário pré-preenchido com os dados atuais do filme e botões adicionais para adicionar atores ao elenco(cast) ou novos gêneros(genres).


 - [Apagar](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc4/apagar.htm)
Ao clicar em "Apagar" o filme é imediatamente removido do dataset e uma página simples avisa o utilizador que o filme foi removido permitindo apenas voltar à lista de filmes. 


 - [Ator](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc4/arnold.htm)
A página de ator é acessível ao clicar no nome de qualquer ator no "cast" de qualquer filme da lista de filmes; nesta página temos uma lista de filmes identica, mas filtrada para incluir apenas os filmes relevantes e onde não é possível clicar no nome de atores para aceder às suas respetivas páginas.

# Usufruir do serviço

 - O serviço tem diversas dependências que não estão incluídas e devem ser instaladas com um comando ```npm install```;
 - Para ter acesso ao dataset é necessário inicializar o json-server sob o [dataset relevante]() ``` json-server --watch dataset_cinema.json" ``` ;
 - Agora com o comando ```npm start``` o serviço deve iniciar e ser acessível pelo [URL da página inicial](http://localhost:2510)

(O dataset pode ser alterado com a edição e remoção de filmes, logo existe um [backup](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC4/dataset_inicial.json))

## Lista de páginas

 - [Página Inicial](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc4/inicio.html)
 - [Página Filmes](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc4/lista_filmes.htm)
 - [Página de Ator](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc4/arnold.htm)
 - [Página de Editar](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc4/editar.htm)
 - [Página de Apagar](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc4/apagar.htm)
