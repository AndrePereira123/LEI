module Tarefa2_2022li1g059_Spec where

import LI12223
import Tarefa2_2022li1g059
import Test.HUnit

testsT2 :: Test
testsT2 = TestLabel "Testes Tarefa 2" $ test ["Teste 1" ~: [Relva,Rio 0,Estrada 0,Relva] ~=? proximosTerrenosValidos (Mapa 2 [(Rio 1, [Nenhum,Tronco]), (Rio (-1), [Nenhum,Tronco]), (Rio 2, [Tronco,Nenhum])]),
                                              "Teste 2" ~: [Relva,Estrada 0] ~=? proximosTerrenosValidos (Mapa 3 [(Rio 3, [Nenhum,Nenhum,Arvore]), (Rio (-1), [Nenhum,Tronco,Tronco]), (Rio 2, []), (Rio (-1), [Nenhum,Tronco,Tronco])]),
                                              "Teste 3" ~: [Relva,Rio 0,Estrada 0,Relva] ~=? proximosTerrenosValidos (Mapa 2 [(Estrada 2, [Carro,Nenhum]), (Estrada 3, [Nenhum,Nenhum]), (Estrada (-1), [Nenhum,Carro]), (Estrada 1, [Carro,Nenhum])]),
                                              "Teste 4" ~: [Relva,Rio 0,Relva] ~=? proximosTerrenosValidos (Mapa 3 [(Estrada 1, [Carro,Carro,Nenhum]), (Estrada 2, [Nenhum,Carro,Nenhum]), (Estrada (-2), [Carro,Carro,Nenhum]), (Estrada (-1), [Nenhum,Carro,Carro]), (Estrada 1, [Carro,Carro, Nenhum])]),
                                              "Teste 5" ~: [Relva,Rio 0,Estrada 0,Relva] ~=? proximosTerrenosValidos (Mapa 2 [(Relva, [Nenhum,Nenhum]), (Relva, [Nenhum,Arvore]), (Relva, [Arvore,Nenhum]), (Relva, [Nenhum,Nenhum])]),
                                              "Teste 6" ~: [Relva,Rio 0,Estrada 0] ~=? proximosTerrenosValidos (Mapa 3 [(Relva, [Nenhum,Nenhum,Arvore]), (Relva, [Nenhum,Arvore,Arvore]), (Relva, [Arvore,Nenhum,Arvore]), (Relva, [Nenhum,Nenhum,Nenhum]), (Relva, [Arvore,Arvore,Nenhum])]),
                                              "Teste 7" ~: [Relva,Rio 0,Estrada 0,Relva] ~=? proximosTerrenosValidos (Mapa 3 [(Estrada 2, [Carro,Carro,Nenhum]), (Relva, [Nenhum,Nenhum,Arvore]), (Rio (-1), [Nenhum,Nenhum,Tronco]), (Rio 2, [Nenhum,Tronco,Tronco]), (Rio 3, [Tronco,Tronco,Nenhum]), (Rio (-1), [Tronco,Nenhum,Nenhum])]),
                                              "Teste 8" ~: [Relva,Rio 0,Estrada 0,Relva] ~=? proximosTerrenosValidos (Mapa 3 [(Relva, [Nenhum,Nenhum,Arvore]), (Estrada 2, [Carro,Carro,Nenhum]), (Estrada (-1), [Nenhum,Carro,Nenhum]), (Rio 1, [Nenhum,Nenhum,Tronco]), (Estrada (-1), [Nenhum,Carro,Carro]), (Relva, [Arvore,Nenhum,Arvore]), (Relva, [Nenhum,Nenhum,Nenhum]), (Relva, [Arvore,Arvore,Nenhum])]),
                                              "Teste 9" ~: [Tronco,Nenhum] ~=? proximosObstaculosValidos 5 (Rio 2, []),
                                              "Teste 10" ~: [Carro,Nenhum] ~=? proximosObstaculosValidos 2 (Estrada 1, []),
                                              "Teste 11" ~: [Nenhum,Arvore] ~=? proximosObstaculosValidos 3 (Relva, []),
                                              "Teste 12" ~: [] ~=? proximosObstaculosValidos 5 (Rio 2, [Tronco,Tronco,Nenhum,Tronco,Tronco]),
                                              "Teste 13" ~: [] ~=? proximosObstaculosValidos 2 (Estrada 2, [Nenhum,Carro]),
                                              "Teste 14" ~: [] ~=? proximosObstaculosValidos 3 (Relva, [Nenhum,Arvore,Nenhum]),
                                              "Teste 15" ~: [Nenhum,Tronco,Tronco] ~=? proximosObstaculosValidos 5 (Rio 2, [Nenhum,Tronco,Nenhum,Nenhum]),
                                              "Teste 16" ~: [Nenhum,Carro,Nenhum] ~=? proximosObstaculosValidos 7 (Estrada 2, [Nenhum,Carro,Carro,Nenhum,Nenhum,Carro]),
                                              "Teste 17" ~: [Nenhum,Arvore,Nenhum] ~=? proximosObstaculosValidos 10 (Relva, [Nenhum,Arvore,Nenhum,Nenhum,Arvore,Arvore,Arvore,Nenhum]),
                                              "Teste 18" ~: [Nenhum,Nenhum] ~=? proximosObstaculosValidos 9 (Rio 2, [Nenhum,Tronco,Nenhum,Tronco,Tronco,Tronco,Tronco,Tronco]),
                                              "Teste 19" ~: [Nenhum,Nenhum] ~=? proximosObstaculosValidos 9 (Rio 2, [Tronco,Tronco,Tronco,Nenhum,Nenhum,Tronco,Tronco]),
                                              "Teste 20" ~: [Nenhum,Nenhum] ~=? proximosObstaculosValidos 7 (Estrada 2, [Carro,Carro,Nenhum,Nenhum,Carro]),
                                              "Teste 21" ~: [Nenhum,Nenhum] ~=? proximosObstaculosValidos 7 (Estrada 2, [Nenhum,Nenhum,Carro,Carro,Carro]),
                                              "Teste 22" ~: Mapa 3 [(Rio 2,[Tronco,Tronco,Nenhum]),(Relva,[Arvore,Nenhum,Nenhum])] ~=? estendeMapa (Mapa 3 [(Relva, [Arvore,Nenhum,Nenhum])]) 3,
                                              "Teste 23" ~: Mapa 4 [(Relva,[Arvore,Arvore,Nenhum,Arvore]),(Rio 2,[Tronco,Tronco,Nenhum,Tronco]),(Estrada (-1),[Carro,Carro,Nenhum,Nenhum])]~=? estendeMapa (Mapa 4 [(Rio 2, [Tronco,Tronco,Nenhum,Tronco]), (Estrada (-1), [Carro,Carro,Nenhum,Nenhum])]) 19,
                                              "Teste 24" ~: Mapa 3 [(Estrada (-1),[Nenhum,Nenhum,Carro]),(Relva,[Arvore,Arvore,Nenhum]),(Relva,[Nenhum,Arvore,Nenhum]),(Relva,[Nenhum,Arvore,Arvore]),(Relva,[Nenhum,Arvore,Nenhum]),(Relva,[Arvore,Arvore,Nenhum])]~=? estendeMapa (Mapa 3 [(Relva, [Arvore,Arvore,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum]), (Relva, [Nenhum,Arvore,Arvore]), (Relva, [Nenhum,Arvore,Nenhum]), (Relva, [Arvore,Arvore,Nenhum])]) 6,
                                              "Teste 25" ~: Mapa 3 [(Estrada 2,[Nenhum,Carro,Nenhum]),(Relva,[Arvore,Arvore,Nenhum]),(Relva,[Nenhum,Arvore,Nenhum]),(Relva,[Nenhum,Arvore,Arvore]),(Relva,[Nenhum,Arvore,Nenhum]),(Relva,[Arvore,Arvore,Nenhum])]~=? estendeMapa (Mapa 3 [(Relva, [Arvore,Arvore,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum]), (Relva, [Nenhum,Arvore,Arvore]), (Relva, [Nenhum,Arvore,Nenhum]), (Relva, [Arvore,Arvore,Nenhum])]) 100,
                                              "Teste 26" ~: Mapa 3 [(Estrada 1,[Carro,Nenhum,Carro]),(Rio (-2),[Tronco,Nenhum,Nenhum]),(Rio 1,[Nenhum,Tronco,Tronco]),(Rio (-3),[Tronco,Tronco,Nenhum])] ~=? estendeMapa (Mapa 3 [(Rio (-2), [Tronco,Nenhum,Nenhum]), (Rio 1, [Nenhum,Tronco,Tronco]), (Rio (-3), [Tronco,Tronco,Nenhum])]) 72,
                                              "Teste 27" ~: Mapa 5 [(Estrada (-1),[Carro,Carro,Nenhum,Nenhum,Carro]),(Rio (-1),[Tronco,Nenhum,Nenhum,Tronco,Tronco]),(Rio 2,[Tronco,Tronco,Nenhum,Tronco,Nenhum]),(Estrada (-1),[Carro,Nenhum,Carro,Carro,Nenhum]),(Relva,[Arvore,Nenhum,Nenhum,Arvore,Nenhum])] ~=? estendeMapa (Mapa 5 [(Rio (-1), [Tronco,Nenhum,Nenhum,Tronco,Tronco]), (Rio 2, [Tronco,Tronco,Nenhum,Tronco,Nenhum]), (Estrada (-1), [Carro,Nenhum,Carro,Carro,Nenhum]), (Relva, [Arvore,Nenhum,Nenhum,Arvore,Nenhum])]) 14           
                                             ]
