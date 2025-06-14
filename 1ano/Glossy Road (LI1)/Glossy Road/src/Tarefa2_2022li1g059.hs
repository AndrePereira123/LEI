{- |
Module      : Tarefa2_2022li1g059
Description : Geração contínua de um mapa
Copyright   : André Filipe Soares Pereira  <a104275@alunos.uminho.pt>
              Leonardo Gomes Alves <a104093@alunos.uminho.pt>

Módulo para a realização da Tarefa 2 do projeto de LI1 em 2022/23.
-}
module Tarefa2_2022li1g059 where

import Tarefa1_2022li1g059
import LI12223
import System.Random

randomIntsL :: Int -> Int -> [Int]
randomIntsL seed len = take len $ randoms (mkStdGen seed)    -- da uma lista de numeros giagntes atraves de outro numero (seed) mas para cada numero corresponde 1 lista
                                                             -- logo n e totalemnte random um numero num mapa da sempre o mesmo desde q n se altere um deles      

{-| A função principal estendeMapa cria a proxima linha de um mapa valido bem como onde cada obstaculo que apresenta movimento (Troncos e Carros) vai estar apos uma ação
do jogador que sera também calculada  

1. Para determinar um proximo terreno a adicionar ao mapa a função "proximosTerrenosValidos" determina uma lista dos terrenos possiveis e a função "velocidadeTerrenos" seleciona 
semi aleatoriamente um deles atraves da seed inicial "i" e da função pre definida do "System.Random" "randomIntsL" ; também temos a função terrenos que cria uma lista apenas 
com os terrenos da lista de terrenos e obstaculos inicial de forma a comparar mais facil os mesmos e garantir as direções opostas de rios seguidos. 

2. No que toca aos obstaculos a função do mesmo nome aplica a função "proximosObstaculosValidos" de forma a construir uma lista completa dos mesmos dependendo do terreno determinado
anteriormente ; também usufrui de uma semi aleatoriadade pela seed e "randomIntsL".



===Função principal:

@
estendeMapa :: Mapa -> Int -> Mapa                                     
estendeMapa (Mapa l ((terr, (h:t)):t2)) i =
       Mapa l ((terreno, obstaculos i l (p, [])) : k2 )
       where m = (Mapa l ((terr, (h:t)):t2))                                        
             k = (terr, (h:t))                                                      
             k2 = ((terr, (h:t)):t2)                                                
             k3 = length ((proximosTerrenosValidos) m )
             k4 = (head (randomIntsL i 1)) * l
             p = (!!) (proximosTerrenosValidos (Mapa l ((terr, (h:t)):t2))) (mod k4 k3)
             terreno = velocidadeTerrenos (((!!) ((proximosTerrenosValidos) m ) (mod k4 k3)): (terrenos k2 )) i l
@ 

===Função auxiliar:

@
velocidadeTerrenos :: [Terreno] -> Int -> Int -> Terreno
velocidadeTerrenos (Rio v:Rio v2:t) i l = Rio ((mod((-v2) * (head (randomIntsL i 1))) l) - v2)              
velocidadeTerrenos (Rio v:t) i l =  Rio ((mod (head (randomIntsL i 1)) l) -1)                             
velocidadeTerrenos (Estrada v:t) i l = Estrada ((mod (head (randomIntsL i 1)) l) -1)
velocidadeTerrenos (Relva :t) i l = Relva 
@

===Função auxiliar2:

@
terrenos :: [(Terreno, [Obstaculo])] -> [Terreno]
terrenos [] = [] 
terrenos ((terr, (h:t)):t2) = terr : terrenos t2   
@

===Função auxiliar3:

@
obstaculos i l (p,obs) | obs == [] = obstaculos i l (p,[((!!) (proximosObstaculosValidos l (p,[])) (mod (k4) 2))])
                       | length obs < l = obstaculos ((mod (k4) 100)) l (p,(((!!) (proximosObstaculosValidos l (p,obs)) (mod (k4) 2)) : obs))
                       |otherwise = obs
                       where k4 = (head (randomIntsL i 1))
@

===Função randomIntsL:

@
randomIntsL :: Int -> Int -> [Int]
randomIntsL seed len = take len $ randoms (mkStdGen seed)
@


====Exemplos de utilização da função:
>>>estendeMapa (Mapa 3 [(Relva, [Arvore,Nenhum,Nenhum])]) 3
Mapa 3 [(Rio 2,[Tronco,Nenhum,Nenhum]),(Relva,[Arvore,Nenhum,Nenhum])]

>>>estendeMapa (Mapa 3 [(Rio (-2), [Tronco,Nenhum,Nenhum]), (Rio 1, [Nenhum,Tronco,Tronco]), (Rio (-3), [Tronco,Tronco,Nenhum])]) 72
Mapa 3 [(Estrada 1,[Carro,Carro,Nenhum]),(Rio (-2),[Tronco,Nenhum,Nenhum]),(Rio 1,[Nenhum,Tronco,Tronco]),(Rio (-3),[Tronco,Tronco,Nenhum])]

>>>estendeMapa (Mapa 5 [(Rio (-1), [Tronco,Nenhum,Nenhum,Tronco,Tronco]), (Rio 2, [Tronco,Tronco,Nenhum,Tronco,Nenhum]), (Estrada (-1), [Carro,Nenhum,Carro,Carro,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum,Arvore,Nenhum])]) 14
Mapa 5 [(Estrada (-1),[Nenhum,Carro,Carro,Carro,Nenhum]),(Rio (-1),[Tronco,Nenhum,Nenhum,Tronco,Tronco]),(Rio 2,[Tronco,Tronco,Nenhum,Tronco,Nenhum]),(Estrada (-1),[Carro,Nenhum,Carro,Carro,Nenhum]),(Relva,[Arvore,Nenhum,Nenhum,Arvore,Nenhum])]

-}
estendeMapa :: Mapa -> Int -> Mapa                                     
estendeMapa (Mapa l ((terr, (h:t)):t2)) i =  Mapa l ((terreno, obstaculos i l (p, [])) : k2 )

       where k = (Mapa l ((terr, (h:t)):t2))                                        --calcula primeiro um terreno valido random com velocidade randomizada 
             k2 = ((terr, (h:t)):t2)                                                 -- depois a lista de obstaculos q escolhe random tb 
             k3 = length ((proximosTerrenosValidos) k )
             k4 = ((!!) (randomIntsL i 12) (mod k3 l)) 
             p = (!!) (proximosTerrenosValidos (Mapa l ((terr, (h:t)):t2))) (mod k4 k3)
             terreno = velocidadeTerrenos (((!!) ((proximosTerrenosValidos) k ) (mod (k4) k3)): (terrenos k2 )) i l



velocidadeTerrenos :: [Terreno] -> Int -> Int -> Terreno
velocidadeTerrenos (Rio v:Rio v2:t) i l = if v2 > 0 then Rio ((- (mod (head (randomIntsL i 1)) 2)) -1 ) else Rio ((mod (head (randomIntsL i 1)) 2) +1)                        --atribui velocidades as listas de terrenos com a cena dos 
velocidadeTerrenos (Rio v:t) i l =  Rio ((mod (head (randomIntsL i 1)) 2) +1)                             --rios terem de ser opostos 2 a 2 e tb random por tentativas 
velocidadeTerrenos (Estrada v:t) i l = if (head (randomIntsL i 1)) > 5 then Estrada ((mod (head (randomIntsL i 1)) 2) +1) else Estrada (-((mod (head (randomIntsL i 1)) 2) +1))
velocidadeTerrenos (Relva :t) i l = Relva 


terrenos :: [(Terreno, [Obstaculo])] -> [Terreno]
terrenos [] = [] 
terrenos ((terr, (h:t)):t2) = terr : terrenos t2                             --lista dos terrenos 


obstaculos i l (p,obs) | obs == [] = obstaculos i l (p,[((!!) (proximosObstaculosValidos l (p,[])) (mod (k4) 2))])
                       | length obs < l = obstaculos ((mod (k4) 100)) l (p,(((!!) (proximosObstaculosValidos l (p,obs)) (mod (k4) 2)) : obs))
                       |otherwise = obs
                       where k4 = (head (randomIntsL i 1))

{-| A função denomeada por proximosTerrenosValidos têm a função de verificar quais os próximos terrenos válidos de um mapa, tendo em conta
os terrenos já lá existentes. 

Para tal, a função definida trabalha com o comprimento da lista em que analisa os primeiros 4 ou 5 elementos de certa listas de terrenos, 
visto que os terrenos válidos são inseridos na cabeça da lista,
e de seguida compara-as com por exemplo uma lista unicamente compostas por "Rio". Nesta função utilizámos também a função isTerreno
que foi definida na tarefa anterior.

==Função principal:

@
proximosTerrenosValidos (Mapa l ((Rio _, _):t)) | take 3 (isTerreno t) == (replicate 3 1) = [Relva, Estrada 0]                      --4 rios
                                                | otherwise = [Relva, Rio 0, Estrada 0 , Relva]

proximosTerrenosValidos (Mapa l j@((Estrada _,_):t)) | take 4 (isTerreno t) == (replicate 4 2) = [Relva, Rio 0,Relva]                      --5 estradas 
                                                     | otherwise = [Relva, Rio 0, Estrada 0 , Relva]

proximosTerrenosValidos (Mapa l j@((Relva,_):t)) | take 4 (isTerreno t) == (replicate 4 3) = [Relva, Rio 0,Estrada 0]                      --5 relvas 
                                                 | otherwise = [Relva, Rio 0, Estrada 0 , Relva]
proximosTerrenosValidos _ = [Rio 0, Estrada 0 , Relva]
@

===Função auxiliar:

@
isTerreno ((Rio _,b):t) = 1 : isTerreno t
isTerreno ((Estrada _,b):t) = 2 : isTerreno t
isTerreno ((Relva,b):t) = 3 : isTerreno t
isTerreno [] = []
@

====Exemplos de utilização da função:

>>>proximosTerrenosValidos (Mapa 2 [(Rio 1, [Nenhum,Tronco]), (Rio (-1), [Nenhum,Tronco]), (Rio 2, [Tronco,Nenhum])])
[Relva,Rio 0,Estrada 0,Relva]

>>>proximosTerrenosValidos (Mapa 3 [(Estrada 1, [Carro,Carro,Nenhum]), (Estrada 2, [Nenhum,Carro,Nenhum]), (Estrada (-2), [Carro,Carro,Nenhum]), (Estrada (-1), [Nenhum,Carro,Carro]), (Estrada 1, [Carro,Carro, Nenhum])])
[Relva,Rio 0,Relva]

>>>proximosTerrenosValidos (Mapa 3 [(Estrada 2, [Carro,Carro,Nenhum]), (Relva, [Nenhum,Nenhum,Arvore]), (Rio (-1), [Nenhum,Nenhum,Tronco]), (Rio 2, [Nenhum,Tronco,Tronco]), (Rio 3, [Tronco,Tronco,Nenhum]), (Rio (-1), [Tronco,Nenhum,Nenhum])])
[Relva,Rio 0,Estrada 0,Relva]

-}

proximosTerrenosValidos :: Mapa -> [Terreno]
proximosTerrenosValidos (Mapa l ((Rio _, _):t)) | take 3 (isTerreno t) == (replicate 3 1) = [Relva, Estrada 0]                      --4 rios
                                                | otherwise = [Relva, Rio 0, Estrada 0 , Relva]

proximosTerrenosValidos (Mapa l j@((Estrada _,_):t)) | take 4 (isTerreno t) == (replicate 4 2) = [Relva, Rio 0,Relva]                      --5 estradas 
                                                     | otherwise = [Relva, Rio 0, Estrada 0 , Relva]

proximosTerrenosValidos (Mapa l j@((Relva,_):t)) | take 4 (isTerreno t) == (replicate 4 3) = [Relva, Rio 0,Estrada 0]                      --5 relvas 
                                                 | otherwise = [Relva, Rio 0, Estrada 0 , Relva]
proximosTerrenosValidos _ = [Rio 0, Estrada 0 , Relva]


{-| A função definida por proximosObstaculosValidos tem o objetivo de verificar quais os próximos obstáculos válidos num certo terreno,
dada a largura do mapa, que corresponde também ao comprimento da lista de obstáculos. 

Desta forma, a função verifica qual o comprimento da lista de obstáculos e depois compara tanto como os primeiros, como os últimos elementos
da lista, e também os que estão no meio da lista, para verificar se por exemplo existem 5 troncos seguidos e, caso tiverem, o resultado
terá que ser obrigatoriamente o obstáculo "Nenhum". 

Desta forma, a função analisa também para os outros terrenos.

==Função principal:

@
proximosObstaculosValidos n (Rio _, []) = [Tronco,Nenhum]
proximosObstaculosValidos n j@(Rio _, (h:t)) | length (h:t) == n = []
                                             | length (h:t) > 5 && (last (h:t)) /= Nenhum = proximosObstaculosValidos n (Rio 0, u:(init (h:t)))
                                             | last (h:t) == Nenhum && take 5 (h:t) == (replicate 5 Tronco) = [Nenhum,Nenhum]
                                             | otherwise = [Nenhum,Tronco,Tronco]
                                             where u = last (h:t)

proximosObstaculosValidos n (Estrada _, []) = [Carro,Nenhum]
proximosObstaculosValidos n j@(Estrada _, (h:t)) | length (h:t) == n = []                                                                                
                                                 | length (h:t) > 3 && (last (h:t)) /= Nenhum = proximosObstaculosValidos n (Estrada 0, u:(init (h:t)))
                                                 | last (h:t) == Nenhum && take 3 (h:t) == (replicate 3 Carro) = [Nenhum,Nenhum]
                                                 | otherwise = [Nenhum,Carro,Nenhum]
                                                 where u = last (h:t)

proximosObstaculosValidos n (Relva, []) = [Nenhum,Arvore]
proximosObstaculosValidos n j@(Relva, (h:t)) | (length (h:t)) == n = [] 
                                             | elem Nenhum (h:t) == False = [Nenhum,Nenhum]
                                             | otherwise = [Nenhum,Arvore,Nenhum]
@

====Exemplos de utilização da função:

>>>proximosObstaculosValidos 5 (Rio 2, [Tronco,Tronco,Nenhum,Tronco,Tronco])
[]

>>>proximosObstaculosValidos 10 (Relva, [Nenhum,Arvore,Nenhum,Nenhum,Arvore,Arvore,Arvore,Nenhum])
[Nenhum,Arvore,Nenhum]

>>>proximosObstaculosValidos 9 (Rio 2, [Tronco,Tronco,Tronco,Nenhum,Nenhum,Tronco,Tronco])
[Nenhum,Nenhum]

>>>proximosObstaculosValidos 5 (Rio 2, [Nenhum,Tronco,Nenhum,Nenhum])
[Nenhum,Tronco,Tronco]
-}
                                    
proximosObstaculosValidos :: Int -> (Terreno, [Obstaculo]) -> [Obstaculo]
proximosObstaculosValidos n (Rio _, []) = [Tronco,Nenhum]
proximosObstaculosValidos n j@(Rio _, (h:t)) | length (h:t) == n = []
                                             | length (h:t) > 5 && (last (h:t)) /= Nenhum = proximosObstaculosValidos n (Rio 0, u:(init (h:t)))
                                             | last (h:t) == Nenhum && take 5 (h:t) == (replicate 5 Tronco) = [Nenhum,Nenhum]
                                             | otherwise = [Nenhum,Tronco,Tronco]
                                             where u = last (h:t)

proximosObstaculosValidos n (Estrada _, []) = [Carro,Nenhum]
proximosObstaculosValidos n j@(Estrada _, (h:t)) | length (h:t) == n = []                                                                                
                                                 | length (h:t) > 3 && (last (h:t)) /= Nenhum = proximosObstaculosValidos n (Estrada 0, u:(init (h:t)))
                                                 | last (h:t) == Nenhum && take 3 (h:t) == (replicate 3 Carro) = [Nenhum,Nenhum]
                                                 | otherwise = [Nenhum,Carro,Nenhum]
                                                 where u = last (h:t)

proximosObstaculosValidos n (Relva, []) = [Nenhum,Arvore]
proximosObstaculosValidos n j@(Relva, (h:t)) | (length (h:t)) == n = [] 
                                             | elem Nenhum (h:t) == False = [Nenhum,Nenhum]
                                             | otherwise = [Nenhum,Arvore,Nenhum]
                                             

