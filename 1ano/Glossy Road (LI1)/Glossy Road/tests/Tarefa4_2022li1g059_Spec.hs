module Tarefa4_2022li1g059_Spec where

import LI12223
import Tarefa4_2022li1g059
import Test.HUnit

testsT4 :: Test
testsT4 = TestLabel "Testes Tarefa 4" $ test ["Teste 1" ~: False ~=? foraMapa (Jogador (1,0)) (Mapa 3 [(Rio 1, [Nenhum,Tronco,Tronco]), (Estrada (-1), [Carro,Nenhum,Nenhum]),(Relva, [Nenhum,Nenhum,Arvore])]),
                                              "Teste 2" ~: True ~=? foraMapa (Jogador ((-1),2)) (Mapa 3 [(Rio 1, [Nenhum,Tronco,Tronco]), (Estrada (-1), [Carro,Nenhum,Nenhum]),(Relva, [Nenhum,Nenhum,Arvore])]),
                                              "Teste 3" ~: False ~=? foraMapa (Jogador (1,3)) (Mapa 3 [(Estrada 1, [Nenhum,Tronco,Tronco]), (Estrada (-1), [Carro,Nenhum,Nenhum]),(Relva, [Nenhum,Nenhum,Arvore])]),
                                              "Teste 4" ~: False ~=? foraMapa (Jogador (2,4)) (Mapa 4 [(Relva,[Arvore,Arvore,Nenhum,Nenhum]), (Estrada 1, [Carro,Carro,Carro,Nenhum]), (Rio (-1), [Tronco,Nenhum,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum,Nenhum]), (Estrada (-1), [Carro,Nenhum,Nenhum,Carro])]),
                                              "Teste 5" ~: True ~=? cairRio (Jogador (2,0)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum])]),
                                              "Teste 6" ~: False ~=? cairRio (Jogador (0,1)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum]), (Rio (-2), [Tronco,Nenhum,Nenhum])]),
                                              "Teste 7" ~: True ~=? cairRio (Jogador (1,1)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum]), (Rio (-2), [Tronco,Nenhum,Nenhum])]),
                                              "Teste 8" ~: False ~=? cairRio (Jogador (2,2)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum]), (Rio (-1), [Nenhum,Tronco,Tronco]), (Estrada 1, [Carro,Carro,Nenhum])]),
                                              "Teste 9" ~: True ~=? atropelado (Jogador (1,3)) (Mapa 3 [(Rio 1, [Tronco,Tronco,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum]), (Rio (-1), [Nenhum,Tronco,Tronco]), (Estrada 1, [Carro,Carro,Nenhum])]),
                                              "Teste 10" ~: False ~=? atropelado (Jogador (0,0)) (Mapa 3 [(Estrada 1, [Nenhum,Carro,Nenhum])]),
                                              "Teste 11" ~: False ~=? atropelado (Jogador (2,3)) (Mapa 3 [(Estrada 1, [Nenhum,Carro,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum]), (Rio (-1), [Tronco,Nenhum,Tronco]), (Estrada 2, [Carro,Carro,Nenhum])]),
                                              "Teste 12" ~: True ~=? atropelado (Jogador (0,3)) (Mapa 3 [(Estrada 1, [Nenhum,Carro,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum]), (Rio (-1), [Tronco,Nenhum,Tronco]), (Estrada 2, [Carro,Carro,Nenhum])]),
                                              "Teste 13" ~: True ~=? jogoTerminou (Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1, [Tronco,Nenhum,Tronco])])),
                                              "Teste 14" ~: True ~=? jogoTerminou (Jogo (Jogador (0,0)) (Mapa 3 [(Estrada 1, [Carro,Nenhum,Nenhum])])),
                                              "Teste 15" ~: False ~=? jogoTerminou (Jogo (Jogador (2,0)) (Mapa 3 [(Rio 1, [Tronco,Nenhum,Tronco])])),
                                              "Teste 16" ~: False ~=? jogoTerminou (Jogo (Jogador (1,0)) (Mapa 3 [(Estrada 1, [Carro,Nenhum,Nenhum])])),
                                              "Teste 17" ~: True ~=? jogoTerminou (Jogo (Jogador (0,2)) (Mapa 3 [(Relva, [Arvore,Nenhum,Nenhum]), (Rio 1, [Tronco,Nenhum,Nenhum]), (Estrada (-2), [Carro,Nenhum,Nenhum])])),
                                              "Teste 18" ~: False ~=? jogoTerminou (Jogo (Jogador (1,3)) (Mapa 3 [(Relva, [Arvore,Nenhum,Nenhum]), (Rio 1, [Tronco,Nenhum,Nenhum]), (Estrada (-2), [Carro,Nenhum,Nenhum]), (Rio (-1), [Nenhum,Tronco,Nenhum])])),
                                              "Teste 19" ~: True ~=? jogoTerminou (Jogo (Jogador (0,4)) (Mapa 3 [(Relva, [Arvore,Nenhum,Nenhum]), (Rio 1, [Tronco,Nenhum,Nenhum]), (Estrada (-2), [Carro,Nenhum,Nenhum]), (Rio (-1), [Nenhum,Tronco,Nenhum]), (Estrada 1, [Carro,Nenhum,Nenhum])]))
                                            ]
