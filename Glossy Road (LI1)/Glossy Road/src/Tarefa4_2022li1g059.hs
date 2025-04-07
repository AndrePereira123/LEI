{- |
Module      : Tarefa4_2022li1g059
Description : Determinar se o jogo terminou
Copyright   : André Filipe Soares Pereira  <a104275@alunos.uminho.pt>
              Leonardo Gomes Alves <a104093@alunos.uminho.pt>

Módulo para a realização da Tarefa 4 do projeto de LI1 em 2022/23.
-}
module Tarefa4_2022li1g059 where

import LI12223
import Tarefa1_2022li1g059
{-| A função jogoTerminou tem o objetivo de, como o nome diz, verificar se o jogo terminou. 

Para tal usámos várias funções auxiliares, que nos especificam em que casos o jogador perde.

==Função principal:
@
jogoTerminou (Jogo jogador@(Jogador (x,y)) mapa@(Mapa l ((terr,(h:t)):t2)))
  | foraMapa jogador mapa = True                                                   
  | cairRio jogador mapa = True
  | atropelado jogador mapa = True 
  |otherwise = False 
@

====Exemplos de utilização da função:
>>>jogoTerminou (Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1, [Tronco,Nenhum,Tronco])]))
True

>>>jogoTerminou (Jogo (Jogador (0,0)) (Mapa 3 [(Estrada 1, [Carro,Nenhum,Nenhum])])) 
True

>>>jogoTerminou (Jogo (Jogador (2,0)) (Mapa 3 [(Rio 1, [Tronco,Nenhum,Tronco])]))
False

>>>jogoTerminou (Jogo (Jogador (1,0)) (Mapa 3 [(Estrada 1, [Carro,Nenhum,Nenhum])])) 
False

>>>jogoTerminou (Jogo (Jogador (0,2)) (Mapa 3 [(Relva, [Arvore,Nenhum,Nenhum]), (Rio 1, [Tronco,Nenhum,Nenhum]), (Estrada (-2), [Carro,Nenhum,Nenhum])]))
True

>>>jogoTerminou (Jogo (Jogador (1,3)) (Mapa 3 [(Relva, [Arvore,Nenhum,Nenhum]), (Rio 1, [Tronco,Nenhum,Nenhum]), (Estrada (-2), [Carro,Nenhum,Nenhum]), (Rio (-1), [Nenhum,Tronco,Nenhum])]))
False

>>>jogoTerminou (Jogo (Jogador (0,4)) (Mapa 3 [(Relva, [Arvore,Nenhum,Nenhum]), (Rio 1, [Tronco,Nenhum,Nenhum]), (Estrada (-2), [Carro,Nenhum,Nenhum]), (Rio (-1), [Nenhum,Tronco,Nenhum]), (Estrada 1, [Carro,Nenhum,Nenhum])]))
True

-}
jogoTerminou :: Jogo -> Bool
jogoTerminou (Jogo jogador@(Jogador (x,y)) mapa@(Mapa l ((terr,(h:t)):t2)))
  | foraMapa jogador mapa = True                                                   
  | cairRio jogador mapa = True
  | atropelado jogador mapa = True 
  |otherwise = False 

{-| A função foraMapa verifica se um jogador está fora dos limites do mapa por exemplo, se um jogador acompanha o tronco para fora do mapa, 
o jogador morre. Neste caso considerá-mos o x > 12 estar fora do mapa pois o nosso mapa original do jogo contém 13 posições, incluíndo o 0, no eixo do x. Por isso que, em alguns testes em que o mapa é menor que 12
e a posição dada é maior do que a largarura do mapa mas menor que 12, o resultado é False.

==Função principal:
@
foraMapa :: Jogador -> Mapa -> Bool
foraMapa (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))
     |x<0 || x > 12  = True
     |y<0 || y > 9 = True
     |otherwise = False
       where numerolinhas = length ((terr,(h:t)):t2)
@
 
====Exemplos de utilização da função:

>>>foraMapa (Jogador (1,0)) (Mapa 3 [(Rio 1, [Nenhum,Tronco,Tronco]), (Estrada (-1), [Carro,Nenhum,Nenhum]),(Relva, [Nenhum,Nenhum,Arvore])])
False

>>>foraMapa (Jogador ((-1),2)) (Mapa 3 [(Rio 1, [Nenhum,Tronco,Tronco]), (Estrada (-1), [Carro,Nenhum,Nenhum]),(Relva, [Nenhum,Nenhum,Arvore])])
True

>>>foraMapa (Jogador (1,3)) (Mapa 3 [(Estrada 1, [Nenhum,Tronco,Tronco]), (Estrada (-1), [Carro,Nenhum,Nenhum]),(Relva, [Nenhum,Nenhum,Arvore])])
False

>>>foraMapa (Jogador (2,4)) (Mapa 4 [(Relva,[Arvore,Arvore,Nenhum,Nenhum]), (Estrada 1, [Carro,Carro,Carro,Nenhum]), (Rio (-1), [Tronco,Nenhum,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum,Nenhum]), (Estrada (-1), [Carro,Nenhum,Nenhum,Carro])])
False

-}

foraMapa :: Jogador -> Mapa -> Bool
foraMapa (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))
     |x<0 || x > 12  = True
     |y<0 || y > 9 = True
     |otherwise = False
       where numerolinhas = length ((terr,(h:t)):t2)

{-| A função cairRio verifica se um jogador cai no rio e, caso cair, o jogador perde. 

Para tal, esta função analisa quando o terreno é "Nenhum" na posição dada do jogador.

==Função principal:
@
cairRio :: Jogador -> Mapa -> Bool
cairRio jogador@(Jogador (x,y)) mapa@(Mapa l ((terr,(h:t)):t2))
   | y /= 0 = cairRio (Jogador (x,y-1)) (Mapa l (t2))
   |isTerreno [(terr,h:t)] == [1] && (!!) (h:t) x == Nenhum = True
   |otherwise = False
@

====Exemplo de utilização da função:
>>>cairRio (Jogador (2,0)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum])])
True

>>>cairRio (Jogador (0,1)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum]), (Rio (-2), [Tronco,Nenhum,Nenhum])])
False

>>>cairRio (Jogador (1,1)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum]), (Rio (-2), [Tronco,Nenhum,Nenhum])])
True

>>>cairRio (Jogador (2,2)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum]), (Rio (-1), [Nenhum,Tronco,Tronco]), (Estrada 1, [Carro,Carro,Nenhum])])
False

-}

cairRio :: Jogador -> Mapa -> Bool
cairRio jogador@(Jogador (x,y)) mapa@(Mapa l ((terr,(h:t)):t2))
   | y /= 0 = cairRio (Jogador (x,y-1)) (Mapa l (t2))
   |isTerreno [(terr,h:t)] == [1] && (!!) (h:t) x == Nenhum = True
   |otherwise = False 


{-| A função atropelado tem o objetivo de verificar se o jogador é atropelado por um carro e, como tal é parecida com a função anterior, 
cairRio.

Desta forma, a função analisa se a posição do Jogador coincide com a posição do carro.

==Função principal:
@
atropelado :: Jogador -> Mapa -> Bool
atropelado (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))
   | y /= 0 = atropelado (Jogador (x,y-1)) (Mapa l (t2))
   |(!!) (h:t) x == Carro = True
   |otherwise = False
@

====Exemplos de utilização da função:
>>>atropelado (Jogador (1,3)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum]), (Rio (-1), [Nenhum,Tronco,Tronco]), (Estrada 1, [Carro,Carro,Nenhum])])
True

>>>atropelado (Jogador (0,0)) (Mapa 3 [(Estrada 1, [Nenhum,Carro,Nenhum])])
False

>>>atropelado (Jogador (2,3)) (Mapa 3 [(Estrada 1, [Nenhum,Carro,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum]), (Rio (-1), [Tronco,Nenhum,Tronco]), (Estrada 2, [Carro,Carro,Nenhum])])
False

>>>atropelado (Jogador (0,3)) (Mapa 3 [(Estrada 1, [Nenhum,Carro,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum]), (Rio (-1), [Tronco,Nenhum,Tronco]), (Estrada 2, [Carro,Carro,Nenhum])])
True

-}
 
atropelado :: Jogador -> Mapa -> Bool
atropelado jogador@(Jogador (x,y)) mapa@(Mapa l ((terr,(h:t)):t2))
   | y /= 0 = atropelado (Jogador (x,y-1)) (Mapa l (t2))
   |(!!) (h:t) x == Carro = True
   |otherwise = False


