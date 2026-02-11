# PL2025-A104275 ||  André Filipe Soares Pereira || A104275
# TPC-2 - Escola de Música (serviço em nodejs(2))


Para o segundo trabalho semanal foi proposto "Construir um serviço em nodejs, que consuma a API de dados servida pelo json-server da escola de música (implementada na segunda aula teórica) e sirva um website com as seguintes caraterísticas:

- Página principal: Listar alunos, Listar Cursos, Listar Instrumentos;

- Página de alunos: Tabela com a informação dos alunos (clicando numa linha deve saltar-se para a página de aluno);

- Página de cursos: Tabela com a informação dos cursos (clicando numa linha deve saltar-se para a página do curso onde deverá aparecer a lista de alunos a frequentá-lo);

- Página de instrumentos: Tabela com a informação dos instrumentos (clicando numa linha deve saltar-se para a página do instrumento onde deverá aparecer a lista de alunos que o tocam).

Mais uma vez o trabalho a realizar assemelha-se àquele desenvolvido nas aulas práticas pelo que pude recorrer ao mesmo e facilitar o processo de implementação.

O [dataset fornecido](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC2/dataset.json) apresenta uma lista de "alunos","cursos" e "instrumentos". Cada aluno guarda:
  - id
  - nome
  - dataNasc
  - curso
  - anoCurso
  - instrumento (apenas o nome)

Cada curso tem 3 campos:
  - id
  - designacao
  - duracao
  - instrumento (id e nome)
    
Cada instrumento apresenta apenas um "id" e nome que se denomina por "#text".


# Implementação
Baseado na implementação das aulas substitui todas as variáveis, URLs e expressões regulares de forma a corresponderem ao novo dataset. Comecei por implementar a lista de alunos com vários ajustes na interface para a tornar consistente e apelativa (a meu ver). A [página inicial](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/Inicio.html) permite aceder às listas requisitadas, lista de [alunos](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/alunos.html), lista de [cursos](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/Cursos.html) e lista de [instrumentos](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/instrumentos.html).  

Para facilitar a navegação é possível retornar à página inicial rapidamente ao clicar em "Início" no canto superior esquerdo em qualquer outra página; para além disto um botão "Voltar" foi implementado no fundo de todas as páginas em que é possível fazê-lo.

Na lista de alunos o nome de cada aluno permite redirecionar para a [página de aluno](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/aluno.html) associada, na lista de curso o nome do curso redireciona o utilizador para uma [página de curso](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/curso.html) com a lista de alunos, que por sua vez podem ser selecionados tal como na lista de alunos total, e na lista de instrumentos um comportamento similar permite selecionar o nome ("#text") de um instrumento para, de seguida, apresentar a [página associada](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/instrumento.html) onde são listados todos os alunos que praticam o uso desse mesmo instrumento.

# Usufruir do serviço

O serviço está implementado no ficheiro [Servidor.js](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC2/Servidor.js); para usufruir do mesmo é necessário inicializar o json-server sob o [dataset relevante](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC2/dataset.json) com: 
 - "json-server --watch dataset.json" 

Depois recorrendo ao nodejs, o serviço em si:
 - "node ...\Servidor.js" 

Agora deve ser possível aceder ao [URL da página inicial](http://localhost:12000/) e navegar com o auxílio dos menus implementados.

### Páginas implementadas:
 - [Inicial      --- "/"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/Inicio.html)
 - [alunos       --- "/alunos"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/alunos.html)
 - [cursos       --- "/cursos"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/Cursos.html)
 - [instrumentos --- "/instrumentos"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/instrumentos.html)
 - [aluno        --- "/alunos/{id_aluno}"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/aluno.html)
 - [curso        --- "/cursos/{id_curso}"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/curso.html)
 - [instrumento  --- "/instrumentos/{id_instrumento}"](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc2/instrumento.html)
