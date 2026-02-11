# PL2025-A104275 ||  André Filipe Soares Pereira || A104275
# TPC-1 - Oficina de Reparações (serviço em nodejs(1))


Para o primeiro trabalho semanal foi proposta a implementação de "um serviço em nodejs que consuma a API de dados servida pelo json-server da oficina de reparações e responda com as páginas web do site"
de forma similar ao [serviço implementado durante as aulas práticas](https://github.com/AndrePereira123/EngWeb2025-A104275/tree/main/TPC1/codigo_produzido_na_aula)(sob o qual baseei o meu código).

O [dataset fornecido](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC1/dataset_reparacoes.json) apresenta uma única lista "reparacoes" onde cada objeto guarda:
  - nome
  - nif
  - viatura
  - nr_intervencoes
  - intervencoes
    
de cada cliente, para além de uma data.

Cada "viatura" tem 3 campos:

  - marca
  - modelo
  - matricula
    
Cada lista "intervencoes" guarda objetos com 3 campos:
  - codigo
  - nome
  - descricao

O principal objetivo da tarefa é ler os dados fornecidos pela API do dataset e permitir acessar não só a lista de todas as reparações como todas as viaturas e intervenções, que não podem ser diretamente listadas através de URL's com o json-server.

# Implementação
Comecei por simplesmente substituir os URL's utilizados no código produzido na aula para os novos na porta 12000 (escolhida arbitrariamente), atualizando alguns títulos e a manipulação realizada sob os objetos para garantir a impressão correta dos dados. Para além disto realizei algumas alterações de UI durante toda a implementação como:
- Aumentar tamanho de fonte do texto
- Adicionar espaço entre linhas
- Adicionar mudança de cor ao pairar sobre o texto selecionável
- Centralizar títulos
- Adicionar um menu lateral para facilitar navegação
- Integrar um alfabeto selecionável para filtrar resultados por letra inicial (nome de cliente ou marca de viatura dependendo da página)
  
entre outros.

Com o menu lateral disponibilizei 3 opções: "Reparações", "Viaturas" e "Intervenções". Estas permitem listar os dados com os formatos relevantes.

Ao clicar em "Reparações", o utilizador é direcionado para "[http://localhost:12000/reparacoes](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc1/reparacoes.htm)", onde são listados todos os clientes num formato "nome - marca de viatura - modelo de viatura" ordenados alfabeticamente. ([Filtro de reparações para letra "J"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc1/reparacoes_filtro_J.htm))

Ao clicar em "Viaturas", o utilizador é direcionado para "[http://localhost:12000/viaturas](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc1/viaturas.htm)", a interface é similar, mas desta vez são listados os veículos num formato "marca - modelo - matricula" ordenados alfabeticamente pela marca. ([Filtro de viaturas para letra "J"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc1/viaturas_filtro_J.htm))

Finalmente com a opção "Intervenções" é disponibilizada uma listagem de todas as intervenções em subconjuntos dependendo do cliente associado, ou seja, primeiro vemos o nome do cliente e por baixo dele de forma indentada todas as intervenções associadas ao mesmo num formato "codigo - nome", na página "[http://localhost:12000/intervencoes](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc1/intervencoes.htm)"; mais uma vez os nomes estão ordenados alfabeticamente. ([Filtro de intervenções para a letra "J"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc1/intervencoes_filtro_J.htm))

Em qualquer um dos 3 modos de listagem selecionar o cliente/viatura direciona o utilizador para "http://localhost:12000/reparacoes/x" onde x representa o nif do cliente associado. Aqui todos os detalhes da "reparação" da lista de reparações são impressos e temos a opção de "Voltar" que re direciona o utilizador para a página anterior (Exemplo: [nif-516835029](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc1/516835029.htm) || [nif-414845795](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc1/414845795.htm)).
A impressão é "rude", direta do ficheiro json associado ao usufruir da função "JSON.stringify()" para o efeito.

# Usufruir do serviço

O serviço está implementado no ficheiro [Servidor.js](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC1/Servidor.js); para usufruir do mesmo é necessário inicializar o json-server sob o [dataset relevante](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC1/dataset_reparacoes.json) com: 
 - "json-server --watch dataset_reparacoes.json" 

Depois recorrendo ao nodejs, o serviço em si:
 - "node ...\Servidor.js" 

Agora deve ser possível aceder ao [URL da página inicial](http://localhost:12000/reparacoes) e navegar com o auxílio dos menus implementados.
