module Tarefa5_2022li1g059_Spec where

import LI12223
import Tarefa5_2022li1g059
import Test.HUnit

testsT5 :: Test
testsT5 = TestLabel "Testes Tarefa 5" $ test [ "Teste 1" ~: Jogo (Jogador (2,1)) (Mapa 4 [(Rio 2,[Nenhum,Tronco,Nenhum,Nenhum]),(Relva,[Arvore,Arvore,Nenhum,Nenhum])]) ~=? deslizaJogo 1 (Jogo (Jogador (2,0)) (Mapa 4 [(Relva, [Arvore,Arvore,Nenhum,Nenhum]), (Estrada 1, [Nenhum,Carro,Carro,Nenhum])])),
                                               "Teste 2" ~: Jogo (Jogador (2,1)) (Mapa 4 [(Relva,[Arvore,Arvore,Nenhum,Arvore]),(Relva,[Arvore,Arvore,Nenhum,Nenhum])]) ~=? deslizaJogo 2 (Jogo (Jogador (2,0)) (Mapa 4 [(Relva, [Arvore,Arvore,Nenhum,Nenhum]), (Estrada 1, [Nenhum,Carro,Carro,Nenhum])])),
                                               "Teste 3" ~: Jogo (Jogador (2,1)) (Mapa 4 [(Rio 2,[Nenhum,Tronco,Nenhum,Nenhum]),(Relva,[Arvore,Arvore,Nenhum,Nenhum])]) ~=? deslizaJogo 3 (Jogo (Jogador (2,0)) (Mapa 4 [(Relva, [Arvore,Arvore,Nenhum,Nenhum]), (Estrada 1, [Nenhum,Carro,Carro,Nenhum])])),
                                               "Teste 4" ~: Jogo (Jogador (1,4)) (Mapa 4 [(Relva,[Arvore,Arvore,Nenhum,Arvore]),(Estrada 1,[Nenhum,Carro,Carro,Nenhum]),(Relva,[Nenhum,Nenhum,Nenhum,Arvore]),(Rio 2,[Tronco,Tronco,Tronco,Nenhum])]) ~=? deslizaJogo 56 (Jogo (Jogador (1,3)) (Mapa 4 [(Estrada 1, [Nenhum,Carro,Carro,Nenhum]), (Relva, [Nenhum,Nenhum,Nenhum,Arvore]), (Rio 2, [Tronco,Tronco,Tronco,Nenhum]), (Estrada (-2), [Carro,Carro,Nenhum,Nenhum])])),
                                               "Teste 5" ~: Jogo (Jogador (1,5)) (Mapa 4 [(Relva,[Nenhum,Arvore,Nenhum,Arvore]),(Relva,[Arvore,Arvore,Nenhum,Arvore]),(Estrada 1,[Nenhum,Carro,Carro,Nenhum]),(Relva,[Nenhum,Nenhum,Nenhum,Arvore])]) ~=? deslizaJogo 987 (Jogo (Jogador (1,4)) (Mapa 4 [(Relva,[Arvore,Arvore,Nenhum,Arvore]),(Estrada 1,[Nenhum,Carro,Carro,Nenhum]),(Relva,[Nenhum,Nenhum,Nenhum,Arvore]),(Rio 2,[Tronco,Tronco,Tronco,Nenhum])])),
                                               "Teste 6" ~: Jogo (Jogador (2,3)) (Mapa 4 [(Rio 2,[Tronco,Tronco,Tronco,Nenhum]),(Relva,[Nenhum,Arvore,Nenhum,Arvore]),(Relva,[Arvore,Arvore,Nenhum,Arvore]),(Estrada 1,[Nenhum,Carro,Carro,Nenhum])]) ~=? deslizaJogo 44 (Jogo (Jogador (2,2)) (Mapa 4 [(Relva,[Nenhum,Arvore,Nenhum,Arvore]),(Relva,[Arvore,Arvore,Nenhum,Arvore]),(Estrada 1,[Nenhum,Carro,Carro,Nenhum]),(Relva,[Nenhum,Nenhum,Nenhum,Arvore])]))                                             
                                             ]