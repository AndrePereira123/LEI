# PL2025-A104275 ||  André Filipe Soares Pereira || A104275
# TPC-3 - Gestor de Alunos (serviço em nodejs(3))


Para a terceira semana foi pedida a conclusão da implementação de um serviço em nodejs similar aos desenvolvidos nas semanas anteriores ([TPC1](https://github.com/AndrePereira123/EngWeb2025-A104275/tree/main/TPC1)/[TPC2](https://github.com/AndrePereira123/EngWeb2025-A104275/tree/main/TPC2)) mas, agora, com "Envio de informação com HTML Forms: GET e POST", "Recuperação da informação do body do pedido" e 
"Tratamento de recursos estáticos" para poder suportar as páginas a implementar:

- [Página Inicial com a lista de alunos](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc3/pagina_inicial.html)

- [Página de aluno com o seu nome, número e git](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc3/pagina_aluno.html)

- [Página de registo de aluno](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc3/pagina_registo.html)

- [Página de edição de aluno](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc3/pagina_edicao.html)

- [Página de remover aluno](https://andrepereira123.github.io/EngWeb2025-A104275/paginas_tpc3/pagina_remocao.html)

O serviço deve permitir distinguir os alunos e os "TPC" realizados por cada um deles.

O [dataset](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC2/dataset.json) utilizado.


# Usufruir do serviço

O serviço está implementado no ficheiro [alunos_server.js](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC3/alunos_server.js); para usufruir do mesmo é necessário inicializar o json-server sob o [dataset relevante](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC3/dataset_alunos.json) com: 
 - "json-server --watch dataset_alunos.json" 

(O dataset pode ser alterado com a edição e remoção de alunos, logo existe um [backup](https://github.com/AndrePereira123/EngWeb2025-A104275/blob/main/TPC3/dataset_preservado)) 


Depois recorrendo ao nodejs, o serviço em si:
 - "node ...\alunos_server.js" 

Agora deve ser possível aceder ao [URL da página inicial](http://localhost:7777/) e navegar com o auxílio dos menus implementados.

