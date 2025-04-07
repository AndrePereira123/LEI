{- |
Module      : Tarefa5_2022li1g059
Description : Deslize do mapa
Copyright   : André Filipe Soares Pereira  <a104275@alunos.uminho.pt>
              Leonardo Gomes Alves <a104093@alunos.uminho.pt>

Módulo para a realização da Tarefa 5 do projeto de LI1 em 2022/23.
-}
module Tarefa5_2022li1g059 where

import LI12223
import Tarefa2_2022li1g059
import Tarefa3_2022li1g059
import Tarefa4_2022li1g059
{-|  A função deslizaJogo tem como objetivo fazer com que o mapa seja formado, apagando sempre as linhas mais antigas e gerando simultaniamente novas linhas.

     Para tal, fizemos uma função que nos pede um dado valor n, o qual é um valor totalmente aleatório e que, a cada número é atribuída uma diferente linha; recorremos
     também à função estendeMapa a qual gera as linhas de jogo.

     Na função, basicamente o jogador vai aumentando sempre uma posição para cima e conforme aumenta, é retirada a linha correspondente à posição anterior do jogador.



==Função principal:

@
deslizaJogo :: Int -> Jogo -> Jogo
deslizaJogo n (Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2)))
                                    = (Jogo (Jogador (x,(y+1))) (Mapa l ((nova):(init (obst)))))
                                          where obst = ((terr,(h:t)):t2)
                                                ((Mapa l2 ((nova):t3))) = estendeMapa (Mapa l ((terr,(h:t)):t2)) n
@       

===Exemplos de utilização da função:
>>>deslizaJogo 1 (Jogo (Jogador (2,0)) (Mapa 4 [(Relva, [Arvore,Arvore,Nenhum,Nenhum]), (Estrada 1, [Nenhum,Carro,Carro,Nenhum])]))
Jogo (Jogador (2,1)) (Mapa 4 [(Rio 2,[Nenhum,Tronco,Nenhum,Nenhum]),(Relva,[Arvore,Arvore,Nenhum,Nenhum])])

>>>deslizaJogo 2 (Jogo (Jogador (2,0)) (Mapa 4 [(Relva, [Arvore,Arvore,Nenhum,Nenhum]), (Estrada 1, [Nenhum,Carro,Carro,Nenhum])]))
Jogo (Jogador (2,1)) (Mapa 4 [(Relva,[Arvore,Arvore,Nenhum,Arvore]),(Relva,[Arvore,Arvore,Nenhum,Nenhum])])

>>>deslizaJogo 3 (Jogo (Jogador (2,0)) (Mapa 4 [(Relva, [Arvore,Arvore,Nenhum,Nenhum]), (Estrada 1, [Nenhum,Carro,Carro,Nenhum])]))
Jogo (Jogador (2,1)) (Mapa 4 [(Rio 2,[Nenhum,Tronco,Nenhum,Nenhum]),(Relva,[Arvore,Arvore,Nenhum,Nenhum])])

-}

deslizaJogo :: Int -> Jogo -> Jogo
deslizaJogo n (Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2)))
                                    = (Jogo (Jogador (x,(y+1))) (Mapa l ((nova):(init (obst)))))
                                          where obst = ((terr,(h:t)):t2)
                                                ((Mapa l2 ((nova):t3))) = estendeMapa (Mapa l ((terr,(h:t)):t2)) n 

--opcao de seed
--mapa inicial com relva
--estender com isso 