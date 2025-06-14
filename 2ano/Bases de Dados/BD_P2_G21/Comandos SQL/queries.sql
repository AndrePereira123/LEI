USE AbrilEmFlor;

-- QUERIES
-- listar os investigadores associados a um dado caso
SELECT D.*
FROM Investigador AS D 
JOIN Caso_tem_Investigador AS CtD ON CtD.Investigador_Id = D.Id
WHERE CtD.Caso_Id = 1;


-- listar todos os casos de um suspeito
SELECT C.Id, Cc.Nome, C.Estado, L.Nome
FROM Caso AS C 
JOIN Localidade AS L ON L.Id = C.Localidade_Id
JOIN Categoria AS Cc ON Cc.Id = C.Categoria_Id
JOIN Caso_tem_Suspeito AS CtS ON CtS.Caso_Id = C.Id  
WHERE CtS.Suspeito_Id = 2;


-- listar todos os casos criados a partir de um dado cliente
SELECT C.Id, C.Estado, C.Data_Início, Cc.Nome AS Categoria, L.Nome AS Localidade
FROM Caso AS C
JOIN Categoria AS Cc ON Cc.Id = C.Categoria_Id
JOIN Localidade AS L ON L.Id = C.Localidade_Id
JOIN Denúncia AS D ON D.Id = C.Denúncia_Id
WHERE D.Cliente_Id = 4;


-- listar todos os casos entre duas datas
SELECT *
FROM Caso
WHERE Data_Início >= '2023-01-01' AND Data_Fim <= '2023-05-31'
ORDER BY Data_Início;


-- listar todos os suspeitos de um caso
SELECT S.* 
FROM Suspeito AS S
JOIN Caso_tem_Suspeito AS CtS ON CtS.Suspeito_Id = S.Id
WHERE CtS.Caso_Id = 6;



-- PROCEDURES
-- trocar o estado de um caso transaction atualiza tmb e tal
DELIMITER //
CREATE PROCEDURE encerraCaso(IN id_caso INT, IN data_final DATE)
BEGIN
    DECLARE erro BOOL DEFAULT 0;
    DECLARE data_inicial DATE;
    DECLARE estado CHAR(1);
    DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET erro = 1;

    START TRANSACTION;
    
    SELECT Data_Início, Estado INTO data_inicial, estado
    FROM Caso 
    WHERE Id = id_caso
    FOR UPDATE; 
    
    IF data_inicial > data_final OR estado = 'R' THEN
        SET erro = 1;
    ELSE
        UPDATE Caso
        SET Estado = 'R', Data_Fim = data_final
        WHERE Id = id_caso;
    END IF;
    
    IF erro THEN
        ROLLBACK;
    ELSE 
        COMMIT;
    END IF;
END //


-- teste
-- CALL encerraCaso(123, '2023-12-31');
-- CALL encerraCaso(5, '2023-01-12');
-- CALL encerraCaso(5, '2023-12-31');
-- SELECT * FROM Caso;


-- listar os investigadores com mais casos ativos
DELIMITER //
CREATE PROCEDURE investigadoresMTrabalho(IN numero INT)
BEGIN
    SELECT I.Id, I.Nome, I.Cargo, COALESCE(COUNT(C.Id), 0) AS 'Casos Ativos'
    FROM Investigador AS I
    LEFT JOIN Caso_tem_Investigador AS CtI ON CtI.Investigador_Id = I.Id
    LEFT JOIN Caso AS C ON CtI.Caso_Id = C.Id AND C.Estado = 'A'
    GROUP BY I.Id
    ORDER BY `Casos Ativos` DESC, I.Nome ASC
    LIMIT numero;
END //

-- teste
-- CALL investigadoresMTrabalho(5);


-- FUNCTIONS
-- calcula a percentagem de casos ativos
DELIMITER //
CREATE FUNCTION percentResolvidos()
RETURNS DECIMAL(5,2)
READS SQL DATA
BEGIN
    DECLARE totalCasos INT DEFAULT 0;
    DECLARE resolvidos INT DEFAULT 0;
    DECLARE perc DECIMAL(5,2) DEFAULT 0;
    
    SELECT COUNT(*) INTO totalCasos
    FROM Caso;
    
    IF totalCasos > 0 THEN
		SELECT COUNT(*) INTO resolvidos
		FROM Caso
		WHERE Estado = 'R';
    
		SET perc = (resolvidos / totalCasos) * 100;
    END IF;
    
    RETURN perc;
END //

-- teste
-- SELECT percentResolvidos() AS 'Percentagem de Casos resolvidos';



-- TRIGGERS
-- triger que atualiza a tabela de fotografias quando uma prova é deletada
CREATE TRIGGER antes_delete_prova
BEFORE DELETE ON Prova
FOR EACH ROW
BEGIN
	DELETE FROM Fotografias WHERE Prova_Id = OLD.Id;
END//

-- teste
-- SELECT * FROM Prova;
-- DELETE FROM Prova WHERE Id = 9;



-- VIEWS
-- listar todos os casos ordenados por localidade
DELIMITER ;
CREATE VIEW casosLocalidade AS
	SELECT C.*, L.Nome AS Localidade
    FROM Caso AS C
    JOIN Localidade AS L ON L.Id = C.Localidade_Id
    ORDER BY L.Nome;

-- teste
-- SELECT * FROM casosLocalidade;
-- SELECT * FROM casosLocalidade WHERE Estado = 'I';
    
    
-- agrupar detetives por localidade
DELIMITER ;
CREATE VIEW investigadoresLocalidade AS
	SELECT L.Nome AS 'Localidade', I.Nome AS 'Investigador', I.Cargo
    FROM Investigador AS I
    JOIN Localidade AS L ON L.Id = I.Localidade_Id
    ORDER BY L.Nome;

-- teste
-- SELECT * FROM investigadoresLocalidade;
-- SELECT * FROM investigadoresLocalidade WHERE Cargo = 'C';


-- resumo de um caso
DELIMITER ;
CREATE VIEW resumoCasos AS
    SELECT C.Id, C.Estado, C.Data_Início, 
        Cc.Nome AS 'Categoria', 
        L.Nome AS 'Localidade',
        (SELECT COUNT(*) FROM Vítima WHERE Caso_Id = C.Id) AS 'Nº Vítimas', 
        (SELECT COUNT(*) FROM Caso_tem_Suspeito WHERE Caso_Id = C.Id) AS 'Nº Suspeitos', 
        (SELECT COUNT(*) FROM Testemunha WHERE Caso_Id = C.Id) AS 'Nº Testemunhas', 
        (SELECT COUNT(*) FROM Prova WHERE Caso_Id = C.Id) AS 'Nº Provas', 
        (SELECT COUNT(*) FROM Caso_tem_Investigador WHERE Caso_Id = C.Id) AS 'Nº Investigadores'
    FROM Caso AS C
    JOIN Categoria AS Cc ON Cc.Id = C.Categoria_Id
    JOIN Localidade AS L ON L.Id = C.Localidade_Id;

-- teste
-- SELECT * FROM resumoCasos;
-- SELECT Id, `Nº Suspeitos` FROM resumoCasos;


-- lista de todos os funcionarios
DELIMITER ;
CREATE VIEW funcionarios AS
	SELECT I.Nome, I.Cargo, I.Email, I.Telefone
    FROM Investigador AS I
    UNION 
    SELECT T.Nome, 'T' AS Cargo, T.Email, T.Telefone
    FROM Técnico as T;

-- teste
-- SELECT * FROM funcionarios ORDER BY Cargo;




-- drops
DROP PROCEDURE IF EXISTS encerraCaso;
DROP PROCEDURE IF EXISTS investigadoresMTrabalho;
DROP FUNCTION IF EXISTS percentResolvidos;
DROP TRIGGER IF EXISTS antes_delete_prova;
DROP VIEW IF EXISTS casosLocalidade; 
DROP VIEW IF EXISTS investigadoresLocalidade;
DROP VIEW IF EXISTS resumoCasos;
DROP VIEW IF EXISTS funcionarios;