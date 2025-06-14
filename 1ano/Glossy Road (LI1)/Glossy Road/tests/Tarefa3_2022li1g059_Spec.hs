module Tarefa3_2022li1g059_Spec where

import LI12223
import Tarefa3_2022li1g059
import Test.HUnit

testsT3 :: Test
testsT3 = TestLabel "Testes Tarefa 3" $ test ["Teste 1" ~: Jogador (0,-1) ~=? moverJogador (Jogador (0,0)) (Move Cima),
                                              "Teste 2" ~: Jogador (0,1) ~=? moverJogador (Jogador (0,0)) (Move Baixo),
                                              "Teste 3" ~: Jogador (1,0) ~=? moverJogador (Jogador (0,0)) (Move Direita),
                                              "Teste 4" ~: Jogador (-1,0) ~=? moverJogador (Jogador (0,0)) (Move Esquerda),
                                              "Teste 5" ~: (Jogador (0,0),-1) ~=? seTronco (Jogador (1,0)) [(Rio (-1), [Nenhum,Tronco,Tronco])] (0),
                                              "Teste 6" ~: (Jogador (2,0),1) ~=? seTronco (Jogador (1,0)) [(Rio 1, [Tronco,Tronco,Nenhum])] (0),
                                              "Teste 7" ~: (Jogador (1,2),-1) ~=? seTronco (Jogador (1,2)) [(Rio (-1), [Tronco,Nenhum,Tronco])] (0),
                                              "Teste 8" ~: (Jogador (0,2),-1) ~=? seTronco (Jogador (1,2)) [(Rio (-1), [Nenhum,Tronco,Nenhum])] (0),
                                              "Teste 9" ~: False ~=? verLimites (Jogador (1,2)) [(Rio 1, [Tronco,Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum])],
                                              "Teste 10" ~: False ~=? verLimites (Jogador (1,3)) [(Rio 1, [Tronco,Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum])],
                                              "Teste 11" ~: False ~=? verLimites (Jogador (4,3)) [(Rio 1, [Tronco,Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum])],
                                              "Teste 12" ~: False ~=? verLimites (Jogador ((-1),1)) [(Rio 1, [Tronco,Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum])],
                                              "Teste 13" ~: True ~=? verLimites (Jogador (1,2)) [(Rio 1, [Tronco,Tronco,Nenhum]),(Relva, [Arvore,Nenhum,Nenhum]), (Estrada 2, [Carro,Nenhum,Nenhum])],
                                              "Teste 14" ~: False ~=? verLimites (Jogador (1,0)) [(Relva, [Arvore,Arvore,Nenhum]), (Estrada 1, [Carro,Nenhum,Nenhum])],
                                              "Teste 15" ~: [(Estrada (-1),[Carro,Carro,Nenhum])] ~=? moverObstaculos [(Estrada (-1), [Nenhum,Carro,Carro])],
                                              "Teste 16" ~: [(Estrada 1,[Carro,Nenhum,Carro])] ~=? moverObstaculos [(Estrada 1, [Nenhum,Carro,Carro])],
                                              "Teste 17" ~: [(Rio 2,[Tronco,Tronco,Nenhum])] ~=? moverObstaculos [(Rio 2, [Nenhum,Tronco,Tronco])],
                                              "Teste 18" ~: [(Estrada (-1),[Carro,Carro,Nenhum]),(Rio 1,[Nenhum,Tronco,Nenhum]),(Relva,[Nenhum,Nenhum,Arvore])] ~=? moverObstaculos [(Estrada (-1), [Nenhum,Carro,Carro]), (Rio 1, [Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Nenhum,Arvore])],
                                              "Teste 19" ~: Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1,[Tronco,Tronco,Nenhum]),(Rio (-2),[Nenhum,Nenhum,Tronco])]) ~=? animaJogo 1 (Jogo (Jogador (0,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Rio (-2),[Nenhum,Nenhum,Tronco])])),
                                              "Teste 20" ~: Jogo (Jogador (0,1)) (Mapa 3 [(Rio 1,[Tronco,Tronco,Nenhum]),(Rio (-1),[Tronco,Nenhum,Tronco])]) ~=? animaJogo 1 (Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Rio (-1),[Tronco,Tronco,Nenhum])])),
                                              "Teste 21" ~: Jogo (Jogador (0,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Rio (-2),[Tronco,Nenhum,Tronco])]) ~=? animaJogo 2 (Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Rio (-2),[Tronco,Tronco,Nenhum])])),
                                              "Teste 22" ~: Jogo (Jogador (1,0)) (Mapa 3 [(Rio (-1),[Nenhum,Tronco,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])]) ~=? animaJogo 1 (Jogo (Jogador (2,0)) (Mapa 3 [(Rio (-1),[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])])),
                                              "Teste 23" ~: (Rio 1,[Tronco,Tronco,Nenhum,Nenhum,Tronco]) ~=? animaLinha 1 (Rio 1, [Tronco,Nenhum,Nenhum,Tronco,Tronco]),
                                              "Teste 24" ~: (Estrada (-1),[Carro,Nenhum,Nenhum,Carro,Nenhum,Carro]) ~=? animaLinha 1 (Estrada (-1), [Carro,Carro,Nenhum,Nenhum,Carro,Nenhum]),
                                              "Teste 25" ~: Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])]) ~=? animaJogador (Jogo (Jogador (0,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])) (Move Direita),
                                              "Teste 26" ~: Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])]) ~=? animaJogador (Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])) (Move Esquerda),
                                              "Teste 27" ~: Jogo (Jogador (2,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])]) ~=? animaJogador (Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])])) (Move Direita),
                                              "Teste 28" ~: Jogo (Jogador (2,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])]) ~=? animaJogador (Jogo (Jogador (2,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])])) (Move Baixo),
                                              "Teste 29" ~: Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])]) ~=? animaTartaruga (Jogo (Jogador (0,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])) (Move Direita),
                                              "Teste 30" ~: Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])]) ~=? animaTartaruga (Jogo (Jogador (2,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])) (Move Esquerda)
                                             ]
