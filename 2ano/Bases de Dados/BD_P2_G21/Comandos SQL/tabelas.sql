CREATE DATABASE AbrilEmFlor;
USE AbrilEmFlor;


-- DROP DATABASE AbrilEmFlor;
-- DROP INDEX datasordenadas ON Caso;

CREATE TABLE Cliente (
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Nome VARCHAR(45) NOT NULL,
    Sexo CHAR(1) NOT NULL,
    Email VARCHAR(75) 
);


CREATE TABLE Telefone_Cliente (
    Telefone INT PRIMARY KEY,
    Cliente_Id INT,
    FOREIGN KEY (Cliente_Id) REFERENCES Cliente(Id)
);


CREATE TABLE  Técnico (
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Nome VARCHAR(45) NOT NULL,
    Email VARCHAR(75) NOT NULL,
    Telefone INT NOT NULL
);


CREATE TABLE  Categoria(
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Nome VARCHAR(45) NOT NULL
);


CREATE TABLE  Localidade(
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Nome VARCHAR(45) NOT NULL 
);

CREATE TABLE Denúncia (
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Descrição TEXT NOT NULL,
    Data DATE NOT NULL,
    Cliente_Id INT,
    Técnico_Id INT,
    FOREIGN KEY (Cliente_Id) REFERENCES Cliente(Id),
    FOREIGN KEY (Técnico_Id) REFERENCES Técnico(Id)
);


CREATE TABLE  Caso (
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Estado CHAR(1) NOT NULL,
    Data_Início DATE NOT NULL, 
    Data_Fim DATE,
    Código_Postal INT NOT NULL,
    Rua VARCHAR(75) NOT NULL,
    Categoria_Id INT,
    Localidade_Id INT,
    Denúncia_Id INT,
    FOREIGN KEY (Denúncia_Id) REFERENCES Denúncia(Id),
    FOREIGN KEY (Categoria_Id) REFERENCES Categoria(Id),
    FOREIGN KEY (Localidade_Id) REFERENCES Localidade(Id)
);


CREATE TABLE Suspeito (
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Nome VARCHAR(45) ,
    Data_de_Nascimento DATE ,
    Descrição TEXT NOT NULL,
    Sexo CHAR(1) ,
    Registo_Criminal TEXT ,
    Código_Postal INT ,
    Rua VARCHAR(75) ,
    Localidade VARCHAR(45) ,
    Email VARCHAR(75)   
);


CREATE TABLE Telefone_Suspeito (
    Telefone INT PRIMARY KEY,
    Suspeito_Id INT,
    FOREIGN KEY (Suspeito_Id) REFERENCES Suspeito(Id)
);


CREATE TABLE Caso_tem_Suspeito (
    Caso_Id INT, 
    Suspeito_Id INT,
    PRIMARY KEY(Caso_Id, Suspeito_Id),
    FOREIGN KEY (Caso_Id) REFERENCES Caso(Id),
    FOREIGN KEY (Suspeito_Id) REFERENCES Suspeito(Id)
);


CREATE TABLE  Vítima (
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Nome VARCHAR(45),
    Data_de_Nascimento DATE,
    Descrição TEXT NOT NULL,
    Sexo CHAR(1),
    Código_Postal INT,
    Rua VARCHAR(75),
    Localidade VARCHAR(45),
    Email VARCHAR(75),
    Caso_Id INT,
    FOREIGN KEY (Caso_Id) REFERENCES Caso(Id)
);


CREATE TABLE  Telefone_Vítima (
    Telefone INT PRIMARY KEY,
    Vítima_Id INT,
    FOREIGN KEY (Vítima_Id) REFERENCES Vítima(Id)
);


CREATE TABLE  Testemunha(
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Nome VARCHAR(45),
    Testemunho TEXT NOT NULL,
    Código_Postal INT,
    Rua VARCHAR(75),
    Localidade VARCHAR(45),
    Caso_Id INT,
    FOREIGN KEY (Caso_Id) REFERENCES Caso(Id)
);


CREATE TABLE  Telefone_Testemunha (
    Telefone INT PRIMARY KEY,
    Testemunha_Id INT,
    FOREIGN KEY (Testemunha_Id) REFERENCES Testemunha(Id)
);


CREATE TABLE Investigador (
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Nome VARCHAR(45) NOT NULL,
    Cargo CHAR(1) NOT NULL,
    Email VARCHAR(75) NOT NULL,
    Telefone INT NOT NULL,
    Localidade_Id INT,
    FOREIGN KEY (Localidade_Id) REFERENCES Localidade(Id)
);


CREATE TABLE Caso_tem_Investigador(
    Caso_Id INT, 
    Investigador_Id INT,
    PRIMARY KEY(Caso_Id, Investigador_Id),
    FOREIGN KEY (Caso_Id) REFERENCES Caso(Id),
    FOREIGN KEY (Investigador_Id) REFERENCES Investigador(Id)
);


CREATE TABLE Prova(
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Descrição TEXT NOT NULL,
    Caso_Id INT,
    FOREIGN KEY (Caso_Id) REFERENCES Caso(Id)
);


CREATE TABLE Fotografias (
    Id INT PRIMARY KEY AUTO_INCREMENT,
    Caminho VARCHAR(100) NOT NULL,
    Prova_Id INT,
    FOREIGN KEY (Prova_Id) REFERENCES Prova(Id)
);

CREATE INDEX datasordenadas ON Caso(Data_Início);

