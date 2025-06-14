USE AbrilEmFlor;

CREATE USER 'Diretor'@'localhost';
CREATE USER 'Geraldo'@'localhost';
CREATE USER 'Rute'@'localhost';
CREATE USER 'tecnico_generico'@'localhost';
CREATE USER 'cliente_generico'@'localhost';

SET PASSWORD FOR 'Diretor'@'localhost' = 'diretor';
SET PASSWORD FOR 'Geraldo'@'localhost' = 'geraldo';
SET PASSWORD FOR 'Rute'@'localhost' = 'rute';
SET PASSWORD FOR 'tecnico_generico'@'localhost' = 'tecnico';
SET PASSWORD FOR 'cliente_generico'@'localhost' = 'cliente';


-- premissoes do diretor
GRANT ALL ON AbrilEmFlor.* TO 'Diretor'@'localhost';

-- permissões do Geraldo
GRANT SELECT, INSERT, UPDATE, DELETE ON AbrilEmFlor.Vítima TO 'Geraldo'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON AbrilEmFlor.Caso_tem_Suspeito TO 'Geraldo'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON AbrilEmFlor.Testemunha TO 'Geraldo'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON AbrilEmFlor.Prova TO 'Geraldo'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON AbrilEmFlor.Suspeito TO 'Geraldo'@'localhost';
GRANT SELECT ON AbrilEmFlor.Caso TO 'Geraldo'@'localhost';
GRANT SELECT ON resumoCasos TO 'Geraldo'@'localhost';
GRANT SELECT ON casosLocalidade TO 'Geraldo'@'localhost';
GRANT EXECUTE ON PROCEDURE encerraCaso TO 'Geraldo'@'localhost';

-- permissões da Rute
GRANT SELECT, INSERT, UPDATE, DELETE ON AbrilEmFlor.Prova TO 'Rute'@'localhost';

-- permissões do técnicos
GRANT SELECT, DELETE ON AbrilEmFlor.Denúncia TO 'tecnico_generico'@'localhost';
GRANT SELECT, INSERT, UPDATE ON AbrilEmFlor.Caso TO 'tecnico_generico'@'localhost';
GRANT SELECT, INSERT, UPDATE ON AbrilEmFlor.Categoria TO 'tecnico_generico'@'localhost';
GRANT SELECT, INSERT, UPDATE ON AbrilEmFlor.Caso_tem_Investigador TO 'tecnico_generico'@'localhost';
GRANT EXECUTE ON PROCEDURE investigadoresMTrabalho TO 'tecnico_generico'@'localhost';

-- permissões dos clientes
GRANT INSERT ON AbrilEmFlor.Denúncia TO 'cliente_generico'@'localhost';



-- drops
DROP USER IF EXISTS 'Diretor'@'localhost';
DROP USER IF EXISTS 'Geraldo'@'localhost';
DROP USER IF EXISTS 'Rute'@'localhost';
DROP USER IF EXISTS 'tecnico_generico'@'localhost';
DROP USER IF EXISTS 'cliente_generico'@'localhost';
FLUSH PRIVILEGES;
