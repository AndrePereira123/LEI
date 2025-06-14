module Tarefa1_2022li1g059_Spec where

import LI12223
import Tarefa1_2022li1g059
import Test.HUnit

testsT1 :: Test
testsT1 = TestLabel "Testes Tarefa 1" $ test ["Teste 1" ~: True ~=? mapaValido (Mapa 4 [(Rio 1, [Tronco,Tronco,Tronco,Nenhum]), (Rio (-2), [Nenhum,Tronco,Tronco,Tronco]), (Estrada 3, [Carro,Carro,Nenhum,Carro])]),
                                              "Teste 2" ~: False ~=? mapaValido (Mapa 2 [(Rio 1, [Tronco,Tronco,Tronco,Nenhum]), (Rio (-2), [Nenhum,Tronco,Tronco,Tronco]), (Estrada 3, [Carro,Carro,Nenhum,Carro])]),
                                              "Teste 3" ~: True ~=? compatibilidadeObstaculos (Mapa 3 [(Rio 3, [Tronco,Tronco,Nenhum])]),
                                              "Teste 4" ~: True ~=? compatibilidadeObstaculos (Mapa 4 [(Rio 4, [Tronco,Tronco]), (Relva, [Arvore,Arvore,Nenhum,Nenhum]), (Estrada 2, [Carro,Nenhum,Carro,Carro])]),
                                              "Teste 5" ~: False ~=? compatibilidadeObstaculos (Mapa 3 [(Relva, [Arvore,Arvore,Nenhum]), (Rio (-3), [Carro,Arvore,Nenhum])]),
                                              "Teste 6" ~: True ~=? direcoesRios (Mapa 3 [(Rio 3, [Tronco,Tronco,Nenhum]), (Rio (-2), [Nenhum,Tronco,Tronco])]),
                                              "Teste 7" ~: False ~=? direcoesRios (Mapa 3 [(Rio (-3), [Tronco,Tronco,Nenhum]), (Rio 2, [Nenhum,Tronco,Tronco]), (Rio 4, [Tronco,Tronco,Nenhum])]),
                                              "Teste 8" ~: True ~=? direcoesRios (Mapa 3 [(Rio (-3), [Tronco,Tronco,Nenhum]), (Rio 2, [Nenhum,Tronco,Tronco]), (Rio (-4), [Tronco,Tronco,Nenhum]), (Rio 3, [Tronco,Nenhum,Tronco])]),
                                              "Teste 9" ~: True ~=? troncos (Mapa 10 [(Rio 1, [Tronco,Tronco,Tronco,Tronco,Nenhum,Tronco])]),
                                              "Teste 10" ~: False ~=? troncos (Mapa 10 [(Rio 1, [Tronco,Tronco,Tronco,Tronco,Nenhum,Tronco,Tronco])]),
                                              "Teste 11" ~: True ~=? troncos (Mapa 10 [(Rio 1, [Tronco,Tronco,Tronco,Tronco,Nenhum,Tronco]), (Rio (-3), [Tronco,Tronco,Tronco,Tronco,Tronco,Nenhum]), (Rio (-4), [Tronco,Tronco,Tronco,Tronco,Tronco,Nenhum,Tronco,Nenhum,Tronco,Nenhum])]),
                                              "Teste 12" ~: True ~=? carros (Mapa 6 [(Estrada 4, [Carro,Carro,Nenhum,Carro,Carro,Nenhum])]),
                                              "Teste 13" ~: False ~=? carros (Mapa 6 [(Estrada 3, [Carro,Carro,Carro,Nenhum,Nenhum,Nenhum]), (Estrada (-4), [Carro,Carro,Carro,Nenhum,Nenhum,Carro])]),
                                              "Teste 14" ~: True ~=? carros (Mapa 6 [(Estrada 4, [Carro,Carro,Carro,Nenhum,Nenhum,Nenhum]), (Estrada (-4), [Carro,Carro,Carro,Nenhum,Nenhum,Nenhum]), (Estrada 2, [Carro,Carro,Nenhum,Carro,Carro,Nenhum])]),
                                              "Teste 15" ~: True ~=? verificaObstaculos (Mapa 4 [(Rio 3, [Tronco,Tronco,Tronco,Nenhum])]),
                                              "Teste 16" ~: False ~=? verificaObstaculos (Mapa 4 [(Rio 3, [Tronco,Tronco,Tronco,Nenhum]), (Relva, [Nenhum,Arvore,Arvore,Arvore]), (Estrada (-3), [Carro,Carro,Carro,Carro])]),
                                              "Teste 17" ~: True ~=? verificaObstaculos (Mapa 4 [(Rio 3, [Tronco,Tronco,Tronco,Nenhum]), (Relva, [Nenhum,Arvore,Arvore,Arvore]), (Estrada 2, [Nenhum,Nenhum,Carro,Carro])]),
                                              "Teste 18" ~: True ~=? verificaLargura (Mapa 4 [(Rio 3, [Tronco,Tronco,Tronco,Nenhum])]),
                                              "Teste 19" ~: False ~=? verificaLargura (Mapa 3 [(Relva, [Arvore,Arvore,Arvore]), (Rio 3, [Tronco, Nenhum, Nenhum]), (Estrada (-2), [Nenhum,Carro])]),
                                              "Teste 20" ~: True ~=? verificaLargura (Mapa 4 [(Relva, [Arvore,Nenhum,Arvore,Arvore]), (Rio (-1), [Tronco,Tronco,Tronco,Nenhum]), (Estrada 4, [Carro,Nenhum,Nenhum,Nenhum])]),
                                              "Teste 21" ~: True ~=? verificaContiguidade (Mapa 3 [(Rio 2, [Tronco,Tronco,Nenhum]),(Rio 3, [Tronco,Nenhum,Tronco]), (Rio (-1), [Nenhum,Nenhum,Tronco]), (Rio 1, [Tronco,Tronco,Nenhum])]),
                                              "Teste 22" ~: False ~=? verificaContiguidade (Mapa 3 [(Rio 2, [Tronco,Tronco,Nenhum]),(Rio 3, [Tronco,Nenhum,Tronco]), (Rio (-1), [Nenhum,Nenhum,Tronco]), (Rio 1, [Tronco,Tronco,Nenhum]), (Rio (-2), [Nenhum,Tronco,Tronco])]),
                                              "Teste 23" ~: True ~=? verificaContiguidade (Mapa 2 [(Rio 1, [Tronco,Nenhum]), (Rio (-1), [Nenhum,Tronco]), (Rio 3, [Tronco,Nenhum]), (Estrada 3, [Carro,Nenhum])])                
                                            ]
