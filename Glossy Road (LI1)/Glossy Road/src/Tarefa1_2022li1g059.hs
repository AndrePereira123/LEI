{- |
Module      : Tarefa1_2022li1g059
Description : Validação de um mapa
Copyright   : André Filipe Soares Pereira  <a104275@alunos.uminho.pt>
              Leonardo Gomes Alves <a104093@alunos.uminho.pt>

Módulo para a realização da Tarefa 1 do projeto de LI1 em 2022/23.
-}
module Tarefa1_2022li1g059 where

import LI12223

{-| A função mapaValido recebe um mapa e, após aplicadas as funções auxiliares, diz-nos se o mapa é válido ou não.
    Têm ainda o seu caso de paragem, caso o mapa seja vazio.

==Função principal:
@
mapaValido (Mapa l []) = False
mapaValido (Mapa l t) 
  | compatibilidadeObstaculos (Mapa l t) && direcoesRios (Mapa l t) && troncos (Mapa l t) && carros (Mapa l t)
  && verificaObstaculos (Mapa l t) && verificaLargura (Mapa l t) && verificaContiguidade (Mapa l t) = True 
  |otherwise = False 
@

==== Exemplos de utilização da função: 

>>> mapaValido (Mapa 4 [(Rio 1, [Tronco,Tronco,Tronco,Nenhum]), (Rio (-2), [Nenhum,Tronco,Tronco,Tronco]), (Estrada 3, [Carro,Carro,Nenhum,Carro])])
True

>>> mapaValido (Mapa 2 [(Rio 1, [Tronco,Tronco,Tronco,Nenhum]), (Rio (-2), [Nenhum,Tronco,Tronco,Tronco]), (Estrada 3, [Carro,Carro,Nenhum,Carro])])
False

-}

mapaValido :: Mapa -> Bool
mapaValido (Mapa l []) = False 
mapaValido (Mapa l t) 
  | compatibilidadeObstaculos (Mapa l t) && direcoesRios (Mapa l t) && troncos (Mapa l t) && carros (Mapa l t)
  && verificaObstaculos (Mapa l t) && verificaLargura (Mapa l t) && verificaContiguidade (Mapa l t) = True 
  |otherwise = False 

{-| A função compatibilidadeObstaculos corresponde à função definida no ponto 1 e, verifica se um certo obstáculo dado numa lista de obstáculos, pertence ao seu terreno associado.
    
    Para tal utilizámos uma função auxiliar que verifica se um obstáculo pertence ao seu terreno, nesta função utilizámos a função "elem".

== Função principal:
@
compatibilidadeObstaculos :: Mapa -> Bool
compatibilidadeObstaculos (Mapa l ((tr, (h:t)):t2)) 
            |length ((tr, (h:t)):t2) == 1 = obscaux (tr,(h:t)) 
            |obscaux (tr,(h:t)) == False = False 
            |otherwise = compatibilidadeObstaculos (Mapa l t2)
@

=== Função auxiliar: 
@
obscaux :: (Terreno, [Obstaculo]) -> Bool
obscaux (tr,[]) = True 
obscaux ((Rio _),(h:t)) = not $ elem Carro (h:t)  || elem Arvore (h:t) 
obscaux ((Estrada _),(h:t)) = not $ elem Tronco (h:t) || elem Arvore (h:t) 
obscaux ((Relva),(h:t)) = not $ elem Tronco (h:t) || elem Carro (h:t) 
@

==== Exemplos de utilização da função:

>>>compatibilidadeObstaculos (Mapa 3 [(Rio 3, [Tronco,Tronco,Nenhum])])
True

>>>compatibilidadeObstaculos (Mapa 4 [(Rio 4, [Tronco,Tronco]), (Relva, [Arvore,Arvore,Nenhum,Nenhum]), (Estrada 2, [Carro,Nenhum,Carro,Carro])])
True

>>>compatibilidadeObstaculos (Mapa 3 [(Relva, [Arvore,Arvore,Nenhum]), (Rio (-3), [Carro,Arvore,Nenhum])])
False

-}

compatibilidadeObstaculos :: Mapa -> Bool
compatibilidadeObstaculos (Mapa l ((tr, (h:t)):t2)) 
            |length ((tr, (h:t)):t2) == 1 = obscaux (tr,(h:t)) 
            |obscaux (tr,(h:t)) == False = False 
            |otherwise = compatibilidadeObstaculos (Mapa l t2)


obscaux :: (Terreno, [Obstaculo]) -> Bool
obscaux (tr,[]) = True 
obscaux ((Rio _),(h:t)) = not $ elem Carro (h:t)  || elem Arvore (h:t) 
obscaux ((Estrada _),(h:t)) = not $ elem Tronco (h:t) || elem Arvore (h:t) 
obscaux ((Relva),(h:t)) = not $ elem Tronco (h:t) || elem Carro (h:t) 


{-| A função direcoesRios é definida com o objetivo de verificar se, quando existem dois ou mais rios consecutivos, eles têm direções opostas.
    
    Para tal, caso a multiplicação de ambas as velocidades dos rios for negativa, as direções serão opostas, pois um número negativo a multiplicar por um positivo dá um negativo.

    Desta forma definimos a seguinte função:

==Função principal:
@
direcoesRios (Mapa l [(tr,l1)]) = True 
direcoesRios (Mapa l   ((Rio a,l1):((Rio b,l2):t))) 
   |(a*b) < 0 = direcoesRios (Mapa l ((Rio b,l2):t))
direcoesRios (Mapa l (h:t)) = direcoesRios (Mapa l t) 
@

==== Exemplos de utilização da função:

>>>direcoesRios (Mapa 3 [(Rio 3, [Tronco,Tronco,Nenhum]), (Rio (-2), [Nenhum,Tronco,Tronco])])
True

>>>direcoesRios (Mapa 3 [(Rio (-3), [Tronco,Tronco,Nenhum]), (Rio 2, [Nenhum,Tronco,Tronco]), (Rio 4, [Tronco,Tronco,Nenhum])])
False

>>>direcoesRios (Mapa 3 [(Rio (-3), [Tronco,Tronco,Nenhum]), (Rio 2, [Nenhum,Tronco,Tronco]), (Rio (-4), [Tronco,Tronco,Nenhum]), (Rio 3, [Tronco,Nenhum,Tronco])])
True

-}

direcoesRios :: Mapa -> Bool
direcoesRios (Mapa l [(tr,l1)]) = True 
direcoesRios (Mapa l   ((Rio a,l1):((Rio b,l2):t))) 
   |(a*b) < 0 = direcoesRios (Mapa l ((Rio b,l2):t)) 
   |otherwise = False 
direcoesRios (Mapa l (h:t)) = direcoesRios (Mapa l t) 

{-| A função denomeada por troncos que definimos, tem como objetivo verificar se, os troncos, têm no máximo 5 unidades de comprimento e, para tal, utilizámos uma função
auxiliar a qual se chama check. 

Com esta função, somos capazes de verificar se, o comprimento da lista de obstáculos é maior do que 5.

==Função principal:
@
troncos :: Mapa -> Bool
troncos (Mapa l []) = True 
troncos (Mapa l ((Rio _,[]):t)) = troncos (Mapa l t)
troncos (Mapa l ((Rio _,(a:b)):t)) = if check (length (a:b)) (a:b) then troncos (Mapa l (t))
                                     else False 
troncos (Mapa l ((_,_):t)) = troncos (Mapa l t)
@


=== Função auxiliar: 
@
check :: Int -> [Obstaculo] -> Bool
check 0 (h:t) = True 
check a (h:t) | take 6 (h:t) == (replicate 6 Tronco) = False
              | take 4 (h:t) == (replicate 4 Carro) = False 
              |otherwise = check (a-1) (t ++ [h]) 
@

==== Exemplos da utilização da função:
>>>troncos (Mapa 10 [(Rio 1, [Tronco,Tronco,Tronco,Tronco,Nenhum,Tronco])])
True

>>>troncos (Mapa 10 [(Rio 1, [Tronco,Tronco,Tronco,Tronco,Nenhum,Tronco,Tronco])])
False

>>>troncos (Mapa 10 [(Rio 1, [Tronco,Tronco,Tronco,Tronco,Nenhum,Tronco]), (Rio (-3), [Tronco,Tronco,Tronco,Tronco,Tronco,Nenhum]), (Rio (-4), [Tronco,Tronco,Tronco,Tronco,Tronco,Nenhum,Tronco,Nenhum,Tronco,Nenhum])])
True

-} 

troncos :: Mapa -> Bool
troncos (Mapa l []) = True 
troncos (Mapa l ((Rio _,[]):t)) = troncos (Mapa l t)
troncos (Mapa l ((Rio _,(a:b)):t)) = if check (length (a:b)) (a:b) then troncos (Mapa l (t))
                                     else False 
troncos (Mapa l ((_,_):t)) = troncos (Mapa l t)

{-| Esta função, denomeada por carros, consiste em verificar que os carros tem uma largura máxima de 3 unidades de comprimento, para tal, 
assim como na alínea anterior, utilizámos a função check e, o procedimento da função principal é o mesmo, apenas fazemos alterações mínimas ao código.

== Função principal:
@
carros (Mapa l []) = True 
carros (Mapa l ((Estrada _,[]):t)) = carros (Mapa l t)
carros (Mapa l ((Estrada _,(a:b)):t)) = if check (length (a:b)) (a:b) then carros (Mapa l (t))
                                        else False 
carros (Mapa l ((_,_):t)) = carros (Mapa l t)
@

=== Função auxiliar:
@
check :: Int -> [Obstaculo] -> Bool
check 0 (h:t) = True 
check a (h:t) | take 6 (h:t) == (replicate 6 Tronco) = False
              | take 4 (h:t) == (replicate 4 Carro) = False 
              |otherwise = check (a-1) (t ++ [h])
@

==== Exemplos da utilização da função:

>>>carros (Mapa 6 [(Estrada 4, [Carro,Carro,Nenhum,Carro,Carro,Nenhum])])
True

>>>carros (Mapa 6 [(Estrada 3, [Carro,Carro,Carro,Nenhum,Nenhum,Nenhum]), (Estrada (-4), [Carro,Carro,Carro,Nenhum,Nenhum,Carro])])
False

>>>carros (Mapa 6 [(Estrada 4, [Carro,Carro,Carro,Nenhum,Nenhum,Nenhum]), (Estrada (-4), [Carro,Carro,Carro,Nenhum,Nenhum,Nenhum]), (Estrada 2, [Carro,Carro,Nenhum,Carro,Carro,Nenhum])])
True

-}

carros :: Mapa -> Bool
carros (Mapa l []) = True 
carros (Mapa l ((Estrada _,[]):t)) = carros (Mapa l t)
carros (Mapa l ((Estrada _,(a:b)):t)) = if check (length (a:b)) (a:b) then carros (Mapa l (t))
                                  else False 
carros (Mapa l ((_,_):t)) = carros (Mapa l t)

check :: Int -> [Obstaculo] -> Bool
check 0 (h:t) = True 
check a (h:t) | take 6 (h:t) == (replicate 6 Tronco) = False
              | take 4 (h:t) == (replicate 4 Carro) = False 
              |otherwise = check (a-1) (t ++ [h])                                  

{-| Esta função denomeada por verificaObstaculos tem o objetivo de verificar se num certo mapa, os terrenos têm pelo menos um objecto
que se "Nenhum", isto é, não é composta por exclusivamente obstáculos válidos (Tronco,Carro,Arvore). 

Para tal fizémos uma simples função
que verifica se o "Nenhum" pertence às listas de obstáculos.

==Função principal:
@
verificaObstaculos :: Mapa -> Bool
verificaObstaculos (Mapa l []) = True
verificaObstaculos (Mapa l [(Rio v,obs)]) = if Nenhum `elem` obs && Tronco `elem` obs then True else False 
verificaObstaculos (Mapa l [(tr,l1)]) = if Nenhum `elem` l1 then True else False
verificaObstaculos (Mapa l ((Rio v,obs):t)) | Nenhum `elem` obs && Tronco `elem` obs = verificaObstaculos (Mapa l t)
                                            | otherwise = False
verificaObstaculos (Mapa l ((_,obs):t)) | Nenhum `elem` obs = verificaObstaculos (Mapa l t)
                                        | otherwise = False
@

====Exemplos de utilização da função:

>>>verificaObstaculos (Mapa 4 [(Rio 3, [Tronco,Tronco,Tronco,Nenhum])])
True

>>>verificaObstaculos (Mapa 4 [(Rio 3, [Tronco,Tronco,Tronco,Nenhum]), (Relva, [Nenhum,Arvore,Arvore,Arvore]), (Estrada (-3), [Carro,Carro,Carro,Carro])])
False

>>>verificaObstaculos (Mapa 4 [(Rio 3, [Tronco,Tronco,Tronco,Nenhum]), (Relva, [Nenhum,Arvore,Arvore,Arvore]), (Estrada 2, [Nenhum,Nenhum,Carro,Carro])])
True

-}
verificaObstaculos :: Mapa -> Bool
verificaObstaculos (Mapa l []) = True
verificaObstaculos (Mapa l [(Rio v,obs)]) = if Nenhum `elem` obs && Tronco `elem` obs then True else False 
verificaObstaculos (Mapa l [(tr,l1)]) = if Nenhum `elem` l1 then True else False
verificaObstaculos (Mapa l ((Rio v,obs):t)) | Nenhum `elem` obs && Tronco `elem` obs = verificaObstaculos (Mapa l t)
                                            | otherwise = False
verificaObstaculos (Mapa l ((_,obs):t)) | Nenhum `elem` obs = verificaObstaculos (Mapa l t)
                                        | otherwise = False

{-| Esta função chamada de verificaLargura tem o objetivo de verificar se, o comprimento da lista de obstáculos de cada linha, corresponde
com a largura do mapa dada no início. Desta forma fizémos uma simples função que compara a largura do mapa e o comprimento da lista de
obstáculos.

==Função principal:
@
verificaLargura :: Mapa -> Bool
verificaLargura (Mapa l []) = True
verificaLargura (Mapa l [(tr,l1)]) = if l == length l1 then True else False
verificaLargura (Mapa l ((_,obs):t)) | l == length obs = verificaLargura (Mapa l t)
                                     | otherwise = False
@

====Exemplos de utilização da função:

>>>verificaLargura (Mapa 4 [(Rio 3, [Tronco,Tronco,Tronco,Nenhum])])
True

>>>verificaLargura (Mapa 3 [(Relva, [Arvore,Arvore,Arvore]), (Rio 3, [Tronco, Nenhum, Nenhum]), (Estrada (-2), [Nenhum,Carro])])
False

>>>verificaLargura (Mapa 4 [(Relva, [Arvore,Nenhum,Arvore,Arvore]), (Rio (-1), [Tronco,Tronco,Tronco,Nenhum]), (Estrada 4, [Carro,Nenhum,Nenhum,Nenhum])])
True


-}
verificaLargura :: Mapa -> Bool
verificaLargura (Mapa l []) = True
verificaLargura (Mapa l [(tr,l1)]) = if l == length l1 then True else False
verificaLargura (Mapa l ((_,obs):t)) | l == length obs = verificaLargura (Mapa l t)
                                     | otherwise = False

{-| Esta função denomeada por verificaContiguidade tem o objetivo de verificar que, não existem mais do que 4 rios, nem 5 estradas ou relvas
consecutivas, para tal, criámos uma função auxiliar que, verifica o tipo de terreno, e cria uma lista com os números correspondentes
ao tipo de terreno (Rio = 1; Estrada = 2; Relva = 3). 

De seguida comparámos essa lista criada e, caso for constituída por por exemplo "cinco uns", dá falso, pois não podem existem 5 rios
consecutivos; caso for verdadeiro, a função analisa as restantes linhas.

==Função principal:

@
verificaContiguidade :: Mapa -> Bool
verificaContiguidade (Mapa l []) = True
verificaContiguidade (Mapa l j@((a,b):t)) | length j <= 4 = True
                                          | isTerreno (take 5 j) == (replicate 5 1) = False
                                          | isTerreno (take 6 j) == (replicate 6 2) = False
                                          | isTerreno (take 6 j) == (replicate 6 3) = False
                                          | otherwise = verificaContiguidade (Mapa l t)
@

===Função auxiliar:

@
isTerreno :: [(Terreno,[Obstaculo])] -> [Int] 
isTerreno ((Rio _,b):t) = 1 : isTerreno t
isTerreno ((Estrada _,b):t) = 2 : isTerreno t
isTerreno ((Relva,b):t) = 3 : isTerreno t
isTerreno [] = []
@

====Exemplos de utilização da função:

>>>verificaContiguidade (Mapa 3 [(Rio 2, [Tronco,Tronco,Nenhum]),(Rio 3, [Tronco,Nenhum,Tronco]), (Rio (-1), [Nenhum,Nenhum,Tronco]), (Rio 1, [Tronco,Tronco,Nenhum])])
True

>>>verificaContiguidade (Mapa 3 [(Rio 2, [Tronco,Tronco,Nenhum]),(Rio 3, [Tronco,Nenhum,Tronco]), (Rio (-1), [Nenhum,Nenhum,Tronco]), (Rio 1, [Tronco,Tronco,Nenhum]), (Rio (-2), [Nenhum,Tronco,Tronco])])
False

>>>verificaContiguidade (Mapa 2 [(Rio 1, [Tronco,Nenhum]), (Rio (-1), [Nenhum,Tronco]), (Rio 3, [Tronco,Nenhum]), (Estrada 3, [Carro,Nenhum])])
True

-}
verificaContiguidade :: Mapa -> Bool
verificaContiguidade (Mapa l []) = True
verificaContiguidade (Mapa l j@((a,b):t)) | length j <= 4 = True
                                          | isTerreno (take 5 j) == (replicate 5 1) = False
                                          | isTerreno (take 6 j) == (replicate 6 2) = False
                                          | isTerreno (take 6 j) == (replicate 6 3) = False
                                          | otherwise = verificaContiguidade (Mapa l t)


isTerreno :: [(Terreno,[Obstaculo])] -> [Int] 
isTerreno ((Rio _,b):t) = 1 : isTerreno t
isTerreno ((Estrada _,b):t) = 2 : isTerreno t
isTerreno ((Relva,b):t) = 3 : isTerreno t
isTerreno [] = []

