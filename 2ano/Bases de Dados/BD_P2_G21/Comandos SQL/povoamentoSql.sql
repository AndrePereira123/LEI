USE AbrilEmFlor;

-- clientes
INSERT INTO Cliente (Nome, Sexo, Email) VALUES ('Marco Gonçalves', 'M', 'marco@email.com');
INSERT INTO Cliente (Nome, Sexo, Email) VALUES ('Ana Souza', 'F', 'ana@email.com');
INSERT INTO Cliente (Nome, Sexo) VALUES ('Carlos Silva', 'M');
INSERT INTO Cliente (Nome, Sexo, Email) VALUES ('André Pereira', 'M', 'andregrugru@email.com');
INSERT INTO Cliente (Nome, Sexo) VALUES ('Mariana Oliveira', 'F');

INSERT INTO Telefone_Cliente (Telefone, Cliente_Id) VALUES (123456789, 1);
INSERT INTO Telefone_Cliente (Telefone, Cliente_Id) VALUES (987654321, 2);
INSERT INTO Telefone_Cliente (Telefone, Cliente_Id) VALUES (555666777, 3);


-- tecnicos
INSERT INTO Técnico (Nome, Email, Telefone) VALUES ('Leonardo Alves', 'leo.alves@email.com', 112233445);
INSERT INTO Técnico (Nome, Email, Telefone) VALUES ('Maria Silva', 'maria.silva@email.com', 556677889);

-- categorias
INSERT INTO Categoria (Nome) VALUES ('Homicídio');
INSERT INTO Categoria (Nome) VALUES ('Roubo');
INSERT INTO Categoria (Nome) VALUES ('Sequestro');
INSERT INTO Categoria (Nome) VALUES ('Fraude');
INSERT INTO Categoria (Nome) VALUES ('Desaparecimento');
INSERT INTO Categoria (Nome) VALUES ('Agressão');


-- localidades
INSERT INTO Localidade (Nome) VALUES ('Braga');
INSERT INTO Localidade (Nome) VALUES ('Lisboa');
INSERT INTO Localidade (Nome) VALUES ('Porto');
INSERT INTO Localidade (Nome) VALUES ('Coimbra');
INSERT INTO Localidade (Nome) VALUES ('Faro');
INSERT INTO Localidade (Nome) VALUES ('Aveiro');


-- denuncias 
INSERT INTO Denúncia (Descrição, Data, Cliente_Id, Técnico_Id) VALUES ('Testemunha viu um indivíduo suspeito perto da cena do crime.', '2023-01-15', 1, 1);
INSERT INTO Denúncia (Descrição, Data, Cliente_Id, Técnico_Id) VALUES ('Familiar relatou que a pessoa desaparecida foi vista pela última vez na Avenida da Liberdade.', '2023-02-15', 2, 2); 
INSERT INTO Denúncia (Descrição, Data, Cliente_Id, Técnico_Id) VALUES ('Uma loja de conveniência foi encontrada com os vidros partidos e sem dinheiro na caixa.', '2023-03-08', 3, 1);
INSERT INTO Denúncia (Descrição, Data, Cliente_Id, Técnico_Id) VALUES ('Documento fraudulento foi encontrado durante uma inspeção a minha empresa.', '2023-04-05', 4, 2);
INSERT INTO Denúncia (Descrição, Data, Cliente_Id, Técnico_Id) VALUES ('Um cofre de um hotel foi encontrado vazio.', '2023-05-15', 5, 1);
INSERT INTO Denúncia (Descrição, Data, Cliente_Id, Técnico_Id) VALUES ('Agressão no extrior de um bar.', '2023-06-22', 4, 1); 


-- casos
INSERT INTO Caso (Estado, Data_Início, Data_Fim, Código_Postal, Rua, Categoria_Id, Localidade_Id, Denúncia_Id) VALUES ('R', '2023-01-16', '2023-01-20', 4700, 'Rua dos Detetives', 1, 1, 1);
INSERT INTO Caso (Estado, Data_Início, Data_Fim, Código_Postal, Rua, Categoria_Id, Localidade_Id, Denúncia_Id) VALUES ('A', '2023-02-16', NULL, 1200, 'Avenida da Liberdade, 456', 5, 1, 2);          
INSERT INTO Caso (Estado, Data_Início, Data_Fim, Código_Postal, Rua, Categoria_Id, Localidade_Id, Denúncia_Id) VALUES ('R', '2023-03-09', '2023-03-10', 4000, 'Rua de Santa Catarina, 789', 2, 2, 3);  
INSERT INTO Caso (Estado, Data_Início, Data_Fim, Código_Postal, Rua, Categoria_Id, Localidade_Id, Denúncia_Id) VALUES ('I', '2023-04-06', '2023-04-07', 3000, 'Rua de Ramalho, 101', 4, 2, 4);       
INSERT INTO Caso (Estado, Data_Início, Data_Fim, Código_Postal, Rua, Categoria_Id, Localidade_Id, Denúncia_Id) VALUES ('A', '2023-05-16', NULL, 8000, 'Rua de Portivelas, 202', 2, 3, 5);           
INSERT INTO Caso (Estado, Data_Início, Data_Fim, Código_Postal, Rua, Categoria_Id, Localidade_Id, Denúncia_Id) VALUES ('R', '2023-06-23', '2023-06-25', 3800, 'Rua de Leixões, 303', 6, 3, 6); 


-- suspeitos
INSERT INTO Suspeito (Nome, Data_de_Nascimento, Descrição, Sexo, Registo_Criminal, Código_Postal, Rua, Localidade, Email) VALUES ('Carlos Santos', '1985-03-12', 'Foi a última pessoa a sair do prédio.', 'M', 'Sem antecedentes criminais conhecidos.', 4700, 'Rua da Paz, 10', 'Braga', 'carlos@email.com');
INSERT INTO Suspeito (Nome, Data_de_Nascimento, Descrição, Sexo, Registo_Criminal, Código_Postal, Rua, Localidade, Email) VALUES ('Marta Ferreira', '1990-07-25', 'Testemunhas identificam-na como alguem com comportamento suspeito.', 'F', 'Antecedentes de pequenos delitos.', 4700, 'Rua Central, 5', 'Braga', 'marta@email.com');
INSERT INTO Suspeito (Nome, Data_de_Nascimento, Descrição, Sexo, Registo_Criminal, Código_Postal, Rua, Localidade, Email) VALUES ('Rui Oliveira', '1988-10-30', 'Ex-namorado da vítima.', 'M', 'Condenado por violência doméstica.', 1200, 'Rua dos Sonhos, 20', 'Braga', 'rui@email.com');
INSERT INTO Suspeito (Nome, Data_de_Nascimento, Descrição, Sexo, Registo_Criminal, Código_Postal, Rua, Localidade, Email) VALUES ('Sara Martins', '1976-05-15', 'Suspeita de liderar uma gangue local.', 'F', 'Sem registo criminal.', 4000, 'Rua dos Assaltos, 30', 'Lisboa', 'sara@email.com');
INSERT INTO Suspeito (Nome, Data_de_Nascimento, Descrição, Sexo, Registo_Criminal, Código_Postal, Rua, Localidade, Email) VALUES ('João Rodrigues', '1980-12-01', 'Funcionário da empresa envolvido em transações financeiras suspeitas.', 'M', 'Nenhum registo criminal conhecido.', 3000, 'Rua das Trapaças, 40', 'Lisboa', 'joao@email.com');
INSERT INTO Suspeito (Descrição, Sexo) VALUES ('Foi visto um vulto, aparentemente feminino, nas câmeras de vigilância do hotel.', 'F');
INSERT INTO Suspeito (Nome, Data_de_Nascimento, Descrição, Sexo, Registo_Criminal) VALUES ('Pedro Almeida', '1982-03-05', 'Fanático por futebol, foi visto a sair do bar pouco depois da vitima.', 'M', 'Sem antecedentes criminais.');
INSERT INTO Suspeito (Nome, Data_de_Nascimento, Descrição, Sexo, Registo_Criminal) VALUES ('David Araújo', '1989-12-15', 'Amigo de Pedro Almeida, foi visto a sair do bar ao mesmo tempo que Pedro ', 'M', 'Agressão a um indivíduo.');


-- telefones dos suspeitos
INSERT INTO Telefone_Suspeito (Telefone, Suspeito_Id) VALUES (345098732, 2);
INSERT INTO Telefone_Suspeito (Telefone, Suspeito_Id) VALUES (987654321, 2);
INSERT INTO Telefone_Suspeito (Telefone, Suspeito_Id) VALUES (345987537, 3);


-- casos tem suspeitos
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (1, 1); 
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (1, 2);  
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (2, 3); 
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (2, 2); 
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (3, 4);  
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (4, 5); 
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (5, 6);  
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (6, 7);
INSERT INTO Caso_tem_Suspeito (Caso_Id, Suspeito_Id) VALUES (6, 8);


-- vitimas 
INSERT INTO Vítima (Nome, Data_de_Nascimento, Descrição, Sexo, Código_Postal, Rua, Localidade, Email, Caso_Id) VALUES ('João Oliveira', '1995-10-23', 'Calvo, olhos verdes', 'M', 4700, 'Rua dos Detetives, 123', 'Braga', NULL, 1);
INSERT INTO Vítima (Nome, Data_de_Nascimento, Descrição, Sexo, Código_Postal, Rua, Localidade, Email, Caso_Id) VALUES ('Maria Pires', '2001-01-13', 'Olhos verdes, sardas, cabelos longos e castanhos, cerca de 1m50', 'F', 1200, 'Avenida da Liberdade, 456', 'Braga', NULL, 2);
INSERT INTO Vítima (Nome, Data_de_Nascimento, Descrição, Sexo, Código_Postal, Rua, Localidade, Email, Caso_Id) VALUES ('António Robalo', '1955-03-18', 'Senhor idoso, dono da loja de conveniênica roubada', 'M',4000, 'Rua de Santa Catarina, 789', 'Lisboa', NULL, 3);
INSERT INTO Vítima (Nome, Data_de_Nascimento, Descrição, Sexo, Código_Postal, Rua, Localidade, Email, Caso_Id) VALUES ('Miguel Santos', '1978-08-10', 'Dono da empresa.', 'M', 3000, 'Rua de Ramalho, 101', 'Lisboa', NULL, 4);
INSERT INTO Vítima (Nome, Data_de_Nascimento, Descrição, Sexo, Código_Postal, Rua, Localidade, Email, Caso_Id) VALUES ('Sofia Pereira', '1990-05-25', 'Dona do hotel', 'F', 8000, 'Rua de Portivelas, 202', 'Porto', NULL, 5);
INSERT INTO Vítima (Nome, Data_de_Nascimento, Descrição, Sexo, Código_Postal, Rua, Localidade, Email, Caso_Id) VALUES ('Pedro Coelho', '1989-05-01', 'Foi agredido a saida de um bar no Porto', 'M', 3800, 'Rua de Leixões, 303', 'Porto', NULL, 6);


-- telefones das vitimas
INSERT INTO Telefone_Vítima (Telefone, Vítima_Id) VALUES (354634568, 2);
INSERT INTO Telefone_Vítima (Telefone, Vítima_Id) VALUES (215439525, 3);
INSERT INTO Telefone_Vítima (Telefone, Vítima_Id) VALUES (245234345, 4);
INSERT INTO Telefone_Vítima (Telefone, Vítima_Id) VALUES (984961235, 5);
INSERT INTO Telefone_Vítima (Telefone, Vítima_Id) VALUES (290980457, 6);


-- testemunhas
INSERT INTO Testemunha (Nome, Testemunho, Código_Postal, Rua, Localidade, Caso_Id) VALUES (NULL, 'Encontrei o corpo da vítima debaixo escadas do nosso prédio.', 4700, 'Rua dos Detetives, 120', 'Braga', 1);
INSERT INTO Testemunha (Nome, Testemunho, Código_Postal, Rua, Localidade, Caso_Id) VALUES ('Gabriel Pires', 'Não vejo a minha irmã à muito tempo, tentei contacta-la mas está impossível desde o dia em que acabou com o ex-namorado', NULL, NULL, NULL, 2);
INSERT INTO Testemunha (Nome, Testemunho, Código_Postal, Rua, Localidade, Caso_Id) VALUES ('António Robalo', 'Quando cheguei à minha loja de manhã encontrei-a naquele estado', 4000, 'Rua de Santa Catarina, 789', 'Lisboa', 3);
INSERT INTO Testemunha (Nome, Testemunho, Código_Postal, Rua, Localidade, Caso_Id) VALUES ('Miguel Santos', 'Há dinheiro a desaparecer da minha empresa, as contas não batem certo, eu suspeito do João.', NULL, NULL, 'Lisboa', 4);
INSERT INTO Testemunha (Nome, Testemunho, Código_Postal, Rua, Localidade, Caso_Id) VALUES ('Sara Carvalho', 'Sou colega do João há muitos anos, ele era incapaz de fazer algo do género', NULL, NULL, 'Lisboa', 4);
INSERT INTO Testemunha (Nome, Testemunho, Código_Postal, Rua, Localidade, Caso_Id) VALUES ('Sofia Pereira', 'Quando fui fazer buscar dinheiro ao cofre para pagar as contas, o cofre estava aberto e vazio', NULL, NULL, 'Porto', 5);
INSERT INTO Testemunha (Nome, Testemunho, Código_Postal, Rua, Localidade, Caso_Id) VALUES ('Pedro Coelho', 'Eu não vi quem me agrediu, eu apenas fiz um comentário sobre futebol no bar e quando saí fui agredido por 2 pessoas', NULL, NULL, 'Porto', 6);

-- telefones testemunhas
INSERT INTO Telefone_Testemunha (Telefone, Testemunha_Id) VALUES (398475093, 2);
INSERT INTO Telefone_Testemunha (Telefone, Testemunha_Id) VALUES (215439551, 3);
INSERT INTO Telefone_Testemunha (Telefone, Testemunha_Id) VALUES (245234453, 4);
INSERT INTO Telefone_Testemunha (Telefone, Testemunha_Id) VALUES (984962352, 6);
INSERT INTO Telefone_Testemunha (Telefone, Testemunha_Id) VALUES (290904576, 7);


-- investigadores
INSERT INTO Investigador (Nome, Cargo, Email, Telefone, Localidade_Id) VALUES ('Anabela Oliveira', 'C', 'anabelaOli@email.com', 209837598, 1);
INSERT INTO Investigador (Nome, Cargo, Email, Telefone, Localidade_Id) VALUES ('Henrique Oliveira', 'F', 'riqueOliveira@email.com', 539587348, 1);
INSERT INTO Investigador (Nome, Cargo, Email, Telefone, Localidade_Id) VALUES ('Horácio Oliveira', 'C', 'horacio.oliveira@email.com', 245982374, 1);
INSERT INTO Investigador (Nome, Cargo, Email, Telefone, Localidade_Id) VALUES ('Geraldo Rosa', 'C', 'geraldoAldo@email.com', 387632498, 3);
INSERT INTO Investigador (Nome, Cargo, Email, Telefone, Localidade_Id) VALUES ('Rute Furtado', 'F', 'furtadoRute@email.com', 928347982, 3);
INSERT INTO Investigador (Nome, Cargo, Email, Telefone, Localidade_Id) VALUES ('Salvador Barreto', 'F', 'salvaBarretes@email.com', 398472234, 2);
INSERT INTO Investigador (Nome, Cargo, Email, Telefone, Localidade_Id) VALUES ('Andreia Martins', 'C', 'andreia@email.com', 328947802, 2);


-- caso tem investigador
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (1, 1);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (1, 2);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (2, 3);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (2, 2);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (3, 6);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (3, 7);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (4, 6);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (5, 4);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (5, 5);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (6, 4);
INSERT INTO Caso_tem_Investigador (Caso_Id, Investigador_Id) VALUES (6, 5);


-- provas
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('Faca encontrada no corpo da vítima.', 1);
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('Relatório forense que relata que o sangue pertence à vitima e que não ha impressões digitais.', 1);
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('Uma biata de cigarro encontrada perto do corpo.', 1);
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('Impressões digitais encontradas na caixa da loja.', 3);
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('Janela da loja partida.', 3);
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('Sangue encontrado na janela partida.', 3);
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('Uma série de documentos fraudulentos, cujo o sumatório das faturas difere do valor esperado', 4);
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('Um brinco encontrado perto do cofre', 5);
INSERT INTO Prova (Descrição, Caso_Id) VALUES ('PROVA FALSA !!!!!!!', 6);

-- fotografias
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova1_img1.jpg', 1);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova1_img2.jpg', 1);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova2_img2.jpg', 2);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova3_img1.jpg', 3);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova3_img2.jpg', 3);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova3_img3.jpg', 3);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova4_img1.jpg', 4);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova4_img2.jpg', 4);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova5_img1.jpg', 5);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova5_img2.jpg', 5);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova5_img3.jpg', 5);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova6_img2.jpg', 6);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova6_img3.jpg', 6);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova6_img4.jpg', 6);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova7_img1.jpg', 7);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova7_img3.jpg', 7);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova8_img2.jpg', 8);
INSERT INTO Fotografias (Caminho, Prova_Id) VALUES ('../imagens/prova_falsa.jpg', 9);