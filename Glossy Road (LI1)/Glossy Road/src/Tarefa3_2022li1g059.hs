{- |
Module      : Tarefa3_2022li1g059
Description : Movimentação do personagem e obstáculos
Copyright   : André Filipe Soares Pereira  <a104275@alunos.uminho.pt>
              Leonardo Gomes Alves <a104093@alunos.uminho.pt>

Módulo para a realização da Tarefa 3 do projeto de LI1 em 2022/23.
-}
module Tarefa3_2022li1g059 where
import LI12223
import Tarefa4_2022li1g059

 


{-| A função principal "animaJogo" calcula todos os movimentos que serão realizados após um período de tempo.
Assim, para esta funcao, têm que ser considerados os limites do mapa e se o jogador está num tronco pois, em caso afirmativo, isso implica ajustar também 
as suas coordenadas.

No primeiro caso da funcao é comparado o "n" à "vel" , uma vez que a unica forma de estes valores alguma vez corresponderem é caso o jogador esteja 
num rio, pois a função seTronco devolve uma velocidade 0 caso esta condicoes não se verifiquem, neste caso será atualizado todo o mapa e também a 
posição do jogador.

Em qualquer outro caso apenas se atualiza todos os obstáculos sem necessidade de alterar a posição do jogador. 

==Função principal:
@
animaJogo :: Int -> Jogo -> Jogo
animaJogo n (Jogo m@(Jogador (x,y)) (Mapa l ((terr,(h:t)):t2)))                                                  
      | n == vel = (Jogo posinput (Mapa l movob ))                                                                                   
      | otherwise = (Jogo m (Mapa l movob ))                            
                                                                            
   where posinput   =      fst (seTronco m ((terr,(h:t)):t2) 0)
         vel        = abs (snd (seTronco m ((terr,(h:t)):t2) 0))                              
         movob = map (animaLinha n) mapaoriginal                        
         mapaoriginal = ((terr,(h:t)):t2)

@

====Exemplos de utilização da função:
>>>animaJogo 1 (Jogo (Jogador (0,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Rio (-2),[Nenhum,Nenhum,Tronco])])) 
Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1,[Tronco,Tronco,Nenhum]),(Rio (-2),[Nenhum,Nenhum,Tronco])])

>>>animaJogo 1 (Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Rio (-1),[Tronco,Tronco,Nenhum])]))
Jogo (Jogador (0,1)) (Mapa 3 [(Rio 1,[Tronco,Tronco,Nenhum]),(Rio (-1),[Tronco,Nenhum,Tronco])])

>>>animaJogo 2 (Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Rio (-2),[Tronco,Tronco,Nenhum])]))
Jogo (Jogador (0,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Rio (-2),[Tronco,Nenhum,Tronco])])

>>>animaJogo 1 (Jogo (Jogador (2,0)) (Mapa 3 [(Rio (-1),[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])]))
Jogo (Jogador (1,0)) (Mapa 3 [(Rio (-1),[Nenhum,Tronco,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])])

-}         


animaJogo :: Int -> Jogo -> Jogo
animaJogo n (Jogo m@(Jogador (x,y)) (Mapa l ((terr,(h:t)):t2)))                                                
      | n == vel = (Jogo posinput (Mapa l movob ))                                                                     --prevenir sair fora dos limites                                                                                    
      | otherwise = (Jogo m (Mapa l movob ))                                                                         --proceder sem problemas caso tudo o resto valido 

   where posinput   = fst (seTronco m ((terr,(h:t)):t2) 0)
         vel        = abs (snd (seTronco m ((terr,(h:t)):t2) 0))                              
         movob = map (animaLinha n) mapaoriginal                        
         mapaoriginal = ((terr,(h:t)):t2)

{-|A função animaLinha tem o objetivo de fazer com que uma dada linha seja atualizada conforme a sua velocidade, basicamente faz com que os obstáculos de um rio ou estrada se movimentem conforme 
   se a sua velocidade é positiva (para a direita) ou se é negativa (para a esquerda).

   Realizamos esta funcao limitada a mover os obstaculos 1 unidade de cada vez de forma a poder usufruir dela recursivamente na "reageTempoGloss" e assim tornar o jogo mais fluido.


==Função principal:
@
animaLinha :: Int -> (Terreno,[Obstaculo]) -> (Terreno,[Obstaculo])
animaLinha x (Rio y,obs)      | x == abs y && y < 0 = (Rio y, move (-1) obs)
                              | x == abs y && y > 0 = (Rio y, move 1 obs)

animaLinha x (Estrada y,obs)  | x == abs y && y < 0 = (Estrada y, move (-1) obs)
                              | x == abs y && y > 0 = (Estrada y, move 1 obs)

animaLinha _ l = l
@

===Exemplos de utilização:
>>>animaLinha 1 (Rio 1, [Tronco,Nenhum,Nenhum,Tronco,Tronco])
(Rio 1,[Tronco,Tronco,Nenhum,Nenhum,Tronco])

>>>animaLinha 1 (Estrada (-1), [Carro,Carro,Nenhum,Nenhum,Carro,Nenhum])
(Estrada (-1),[Carro,Nenhum,Nenhum,Carro,Nenhum,Carro])

-}

animaLinha :: Int -> (Terreno,[Obstaculo]) -> (Terreno,[Obstaculo])
animaLinha x (Rio y,obs)      | x == abs y && y < 0 = (Rio y, move (-1) obs)
                              | x == abs y && y > 0 = (Rio y, move 1 obs)

animaLinha x (Estrada y,obs)  | x == abs y && y < 0 = (Estrada y, move (-1) obs)
                              | x == abs y && y > 0 = (Estrada y, move 1 obs)

animaLinha _ l = l

{-| A função animaJogador tem o objetivo de animar o jogador consoante vai jogando e, verifica:

   1- Se o jogador tentar se mover para dentro de uma árvore, a sua posição será a mesma;

   2- Caso o jogador esteja dentro do rio, a sua posição mantém-se e não pode ser mudada;

   3- Verifica ainda se o jogador é atropelado por um carro, caso for verdade, o jogador perde.

==Função principal:
@
animaJogador :: Jogo -> Jogada -> Jogo 
animaJogador i@(Jogo m@(Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))) jogada                                               
            | verLimites postesterio mapaoriginal == False = i
            | (cairRio m (Mapa l ((terr,(h:t)):t2))) = (Jogo m (Mapa l ((terr,(h:t)):t2)))                                          
            | atrop = jogoinicial                                                                                         
            | otherwise = (Jogo postesterio (Mapa l movob ))   

    where postesterio = (moverJogador (Jogador (x,y)) jogada)                                     
          movob = ((terr,(h:t)):t2)                             
          mapaoriginal = ((terr,(h:t)):t2)
          (atrop,jogoinicial) = (atropelamento (Jogo postesterio (Mapa l ((terr,(h:t)):t2)))) 0 
@
===Função auxiliar:

@
atropelamento :: Jogo -> Int -> (Bool,Jogo) 
atropelamento (Jogo (Jogador (x,y)) (Mapa l ((Estrada v,(h:t)):t2))) n |atropelado (Jogador (x,y)) (Mapa l ((Estrada v,(h:t)):t2)) = (True,jogosaved)
                                                                       |v > 1 = atropelamento (Jogo (Jogador (x,y)) (Mapa l ((Estrada (v-1),(last (h:t) : init (h:t))):t2))) (n+1)
                                                                       |v < 1 = atropelamento (Jogo (Jogador (x,y)) (Mapa l ((Estrada (v+1),(t ++ [h])):t2))) (n-1)
                                                                       |otherwise = (False,jogosaved)  
                                                                  where jogosaved =  (Jogo (Jogador (x,y)) (Mapa l ((Estrada (v+n),(h:t)):t2)))
atropelamento (Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))) n = (False,jogogenerico) 
                                                                  where jogogenerico = (Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2)))
@

====Exemplos de utilização:
>>>animaJogador (Jogo (Jogador (0,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])) (Move Direita)
Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])

>>>animaJogador (Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])) (Move Esquerda)
Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])

>>>animaJogador (Jogo (Jogador (1,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])])) (Move Direita)
Jogo (Jogador (2,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])])

>>>animaJogador (Jogo (Jogador (2,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])])) (Move Baixo)
Jogo (Jogador (2,1)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Nenhum])])

-}



animaJogador :: Jogo -> Jogada -> Jogo 
animaJogador i@(Jogo m@(Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))) jogada                                               
            | verLimites postesterio mapaoriginal == False = i
            | (cairRio m (Mapa l ((terr,(h:t)):t2))) = (Jogo m (Mapa l ((terr,(h:t)):t2)))                                     
            | atrop = jogoinicial                                                                                        
            | otherwise = (Jogo postesterio (Mapa l movob ))   

    where postesterio = (moverJogador (Jogador (x,y)) jogada)                                     
          movob = ((terr,(h:t)):t2)                             
          mapaoriginal = ((terr,(h:t)):t2)
          (atrop,jogoinicial) = (atropelamento (Jogo postesterio (Mapa l ((terr,(h:t)):t2)))) 0 

{-| Temos a função animaTartaruga, que foi basicamente uma mudança no nosso jogo, pois permitimos a tartaruga ser capaz de passar pelo rio sem precisar de saltar por cima dos troncos pois, como é uma tartaruga,
 é capaz de nadar.

    A função trabalha da mesma maneira que a função animaJogador porém a única coisa que retiramos foi a parte de verificar se caiu no rio.

==Função principal:
@
animaTartaruga :: Jogo -> Jogada -> Jogo 
animaTartaruga i@(Jogo m@(Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))) jogada                                                  
              | verLimites postesterio mapaoriginal == False = i
              | atrop = jogoinicial                                                                                        
              | otherwise = (Jogo postesterio (Mapa l movob ))   

       where postesterio = (moverJogador (Jogador (x,y)) jogada)                                     
             movob = ((terr,(h:t)):t2)                             
             mapaoriginal = ((terr,(h:t)):t2)
             (atrop,jogoinicial) = (atropelamento (Jogo postesterio (Mapa l ((terr,(h:t)):t2)))) 0 
@

===Exemplos de utilização:
>>>animaTartaruga (Jogo (Jogador (0,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])) (Move Direita)
Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])

>>>animaTartaruga (Jogo (Jogador (2,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])) (Move Esquerda)
Jogo (Jogador (1,0)) (Mapa 3 [(Rio 1,[Tronco,Nenhum,Tronco]),(Relva,[Arvore,Nenhum,Arvore])])


-}

animaTartaruga :: Jogo -> Jogada -> Jogo 
animaTartaruga i@(Jogo m@(Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))) jogada                                                  
              | verLimites postesterio mapaoriginal == False = i
              | atrop = jogoinicial                                                                                        
              | otherwise = (Jogo postesterio (Mapa l movob ))   

       where postesterio = (moverJogador (Jogador (x,y)) jogada)                                     
             movob = ((terr,(h:t)):t2)                             
             mapaoriginal = ((terr,(h:t)):t2)
             (atrop,jogoinicial) = (atropelamento (Jogo postesterio (Mapa l ((terr,(h:t)):t2)))) 0       

      


atropelamento :: Jogo -> Int -> (Bool,Jogo) 
atropelamento (Jogo (Jogador (x,y)) (Mapa l ((Estrada v,(h:t)):t2))) n |atropelado (Jogador (x,y)) (Mapa l ((Estrada v,(h:t)):t2)) = (True,jogosaved)
                                                                       |v > 1 = atropelamento (Jogo (Jogador (x,y)) (Mapa l ((Estrada (v-1),(last (h:t) : init (h:t))):t2))) (n+1)
                                                                       |v < 1 = atropelamento (Jogo (Jogador (x,y)) (Mapa l ((Estrada (v+1),(t ++ [h])):t2))) (n-1)
                                                                       |otherwise = (False,jogosaved)  
                                                                  where jogosaved =  (Jogo (Jogador (x,y)) (Mapa l ((Estrada (v+n),(h:t)):t2)))
atropelamento (Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))) n = (False,jogogenerico) 
                                                                  where jogogenerico = (Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2)))

{-| A função moverObstaculos tem o objetivo de mover os obstáculos de um dado terreno, conforme as suas velocidades.

Desta forma criámos uma função auxiliar denomeada por move que faz exatamente isso. Por exemplo caso a velocidade for 1, 
o objeto desloca-se uma unidade para a direita e, caso for -1, desloca-se uma unidade para a esquerda, e da mesma forma funciona com 
velocidades maiores.

==Função principal:
@
moverObstaculos :: [(Terreno,[Obstaculo])] -> [(Terreno,[Obstaculo])]
moverObstaculos [] = []
moverObstaculos ((Relva,(h:t)):t2) =  (Relva,(h:t)) : moverObstaculos t2

moverObstaculos ((Estrada v,(h:t)):t2) =  
     (Estrada v,(move v (h:t))) : moverObstaculos t2

moverObstaculos ((Rio v,(h:t)):t2) = 
    (Rio v, (move v (h:t))) : moverObstaculos t2
@

===Função auxiliar:
@
move :: Int -> [Obstaculo] -> [Obstaculo]
move 0 (h:t) = (h:t)
move v (h:t) |v >= 1 = move (v-1) (last (h:t) : init (h:t))
             |otherwise = move (v+1) ( t ++ [h])
@

====Exemplos de utilização da função:
>>>moverObstaculos [(Estrada (-1), [Nenhum,Carro,Carro])]
[(Estrada (-1),[Carro,Carro,Nenhum])]

>>>moverObstaculos [(Estrada 1, [Nenhum,Carro,Carro])]
[(Estrada 1,[Carro,Nenhum,Carro])]

>>>moverObstaculos [(Rio 2, [Nenhum,Tronco,Tronco])]
[(Estrada 2,[Tronco,Tronco,Nenhum])]


>>>moverObstaculos [(Estrada (-1), [Nenhum,Carro,Carro]), (Rio 1, [Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Nenhum,Arvore])]
[(Estrada (-1),[Carro,Carro,Nenhum]),(Rio 1,[Nenhum,Tronco,Nenhum]),(Relva,[Nenhum,Nenhum,Arvore])]


-}         

    
--1.-- 5.-- 
moverObstaculos :: [(Terreno,[Obstaculo])] -> [(Terreno,[Obstaculo])]
moverObstaculos [] = []
moverObstaculos ((Relva,(h:t)):t2) =  (Relva,(h:t)) : moverObstaculos t2
moverObstaculos ((Estrada v,(h:t)):t2) =  
     (Estrada v,(move v (h:t))) : moverObstaculos t2
moverObstaculos ((Rio v,(h:t)):t2) = 
    (Rio v, (move v (h:t))) : moverObstaculos t2


move :: Int -> [Obstaculo] -> [Obstaculo]
move 0 (h:t) = (h:t)
move v (h:t) |v >= 1 = move (v-1) (last (h:t) : init (h:t))
             |otherwise = move (v+1) ( t ++ [h])

{-| A função denomeada por moverJogador tem como objetivo mover um jogador, dado a sua posição e a jogada que irá efetuar.

Desta forma, caso o jogador se deslocar para cima, a sua posição irá mudar, e como os próximos terrenos são sempre adicionados à
cabeça da lista, assume valor -1, e para baixo assume valor 1.

==Função principal:
@
moverJogador (Jogador (i1,i2)) Parado = Jogador (i1,i2)
moverJogador (Jogador (i1,i2)) (Move dir) 
      |dir == Cima  = Jogador (i1,i2-1)
      |dir == Baixo  = Jogador (i1,i2+1)
      |dir == Direita  = Jogador (i1+1,i2)
      |dir == Esquerda = Jogador (i1-1,i2)
@

====Exemplos de utilização da função:

>>>moverJogador (Jogador (0,0)) (Move Cima)
Jogador (0,-1)

>>>moverJogador (Jogador (0,0)) (Move Baixo)
Jogador (0,1)

>>>moverJogador (Jogador (0,0)) (Move Direita)
Jogador (1,0)

>>>moverJogador (Jogador (0,0)) (Move Esquerda)
Jogador (-1,0)

-}
--2.-- 
moverJogador :: Jogador -> Jogada -> Jogador
moverJogador (Jogador (i1,i2)) Parado = Jogador (i1,i2)
moverJogador (Jogador (i1,i2)) (Move dir) 
       |dir == Cima  = Jogador (i1,i2-1)
       |dir == Baixo  = Jogador (i1,i2+1)
       |dir == Direita  = Jogador (i1+1,i2)
       |dir == Esquerda = Jogador (i1-1,i2)

{-| A função seTronco, verifica se um jogador está num tronco e, caso seja afirmativo, desloca o jogador uma unidade na direção desse tronco, ou seja conforme a direção do rio.

Para tal criámos uma função auxiliar chamada rioJogador que verifica se o jogador está no rio em cima de um tronco.

Esta função tenta verificar na linha em que se encontra o jogador; para encontrar esta linha recorremos a uma forma de recursividade, verificando linha a linha até alcançar aquela 
em que está o jogador, ou seja, excluíndo "linhas" e ajustando o valor de y até este deter o valor de 0 onde a cabeça da lista de "linhas" do mapa (lista[Terreno,[Obstaculo]])
corresponderá à linha correta.

Para conservar a variável y original bem como poder extrai-la do resultado da função, tornámos o resultado da aplicação desta num tuplo e recorremos 
a uma variável do tipo "Int" de nome n que conserva esta coordenada.  


==Função principal:
@
seTronco :: Jogador -> [(Terreno,[Obstaculo])] -> Int -> (Jogador,Int)
seTronco   (Jogador (x,0)) (m@(Rio v,(h:t)):t2)  n   |rioJogador x m && v < 0  = ((Jogador (x - 1 ,n)),v) 
                                                     |rioJogador x m           = ((Jogador (x + 1 ,n)),v) 
                                                     |otherwise                = (Jogador (x,n),0)
seTronco j@(Jogador (x,y)) ((terreno1,(h:t)):t2) n   |y > 0                    = seTronco (Jogador (x,y-1)) t2 (n+1)
                                                     |otherwise                = (Jogador (x,n),0)
@

===Função auxiliar:
@
rioJogador :: Int -> (Terreno,[Obstaculo]) -> Bool                                  
rioJogador x (_,obs) | x < 0 || x > 12 = True
                     | otherwise = ((!!) (obs) x == Tronco)      
@

====Exemplos de utilização da função:

>>>seTronco (Jogador (1,0)) [(Rio (-1), [Nenhum,Tronco,Tronco])] (0)
(Jogador (0,0),-1)

>>>seTronco (Jogador (1,0)) [(Rio 1, [Tronco,Tronco,Nenhum])] (0)
(Jogador (2,0),1)

>>>seTronco (Jogador (1,2)) [(Rio (-1), [Tronco,Nenhum,Tronco])] (0)
(Jogador (1,2),-1)

>>>seTronco (Jogador (1,2)) [(Rio (-1), [Nenhum,Tronco,Nenhum])] (0)
(Jogador (0,2),-1)

-}       

--3.--
seTronco :: Jogador -> [(Terreno,[Obstaculo])] -> Int -> (Jogador,Int)
seTronco   (Jogador (x,0)) (m@(Rio v,(h:t)):t2)  n   |rioJogador x m && v < 0  = ((Jogador (x - 1 ,n)),v) 
                                                     |rioJogador x m           = ((Jogador (x + 1 ,n)),v) 
                                                     |otherwise                = (Jogador (x,n),0)
seTronco j@(Jogador (x,y)) ((terreno1,(h:t)):t2) n   |y > 0                    = seTronco (Jogador (x,y-1)) t2 (n+1)
                                                     |otherwise                = (Jogador (x,n),0)                                                              

rioJogador :: Int -> (Terreno,[Obstaculo]) -> Bool                                  
rioJogador x (_,obs) | x < 0 || x > 12 = True
                     | otherwise = ((!!) (obs) x == Tronco)

{-| A função verLimites verifica se o jogador está dentro dos limites do jogo e, para tal criámos também uma função auxiliar
que verifica se a posição do jogador é válida, isto é, se o jogador estiver dentro de um carro, a posição é inválida, e assim como se estiver dentro de uma árvore.
    Definimos também as funçãos isTree e isCar que, dada uma coordenada x, verifica se existe algum obstáculo nessa posição

A nossa função verifica a altura e a largura do mapa, comparando-as entre o número de terrenos e o comprimento da lista de obstáculos, respetivamente.

==Função principal:
@
verLimites :: Jogador -> [(Terreno,[Obstaculo])] -> Bool                     
verLimites posmov@(Jogador (x,y)) layout@((terr,(h:t)):t2)
     | y <= (-1) || y >= numerolinhas = False
     | x+1 > comprimento  || x < 0 = False
     | not(verificarArvore posmov layout) = False
     | otherwise = True
     where numerolinhas = length layout
           comprimento = length (h:t)
@

===Função auxiliar:
@
verificarArvore :: Jogador -> [(Terreno,[Obstaculo])] -> Bool
verificarArvore (Jogador (x,0)) (m@(Relva,obs):t2) = not (isTree x m)
verificarArvore (Jogador (x,0)) (m@(Estrada _,obs):t2) = not (isCar x m)
verificarArvore (Jogador (x,y)) ((terreno1,(h:t)):t2) | y > 0 = verificarArvore (Jogador (x,y-1)) t2
                                                      |otherwise = True
@

====Funções auxiliares:

@
isTree :: Int -> (Terreno,[Obstaculo]) -> Bool
isTree x (Relva,obs) = ((!!) obs x == Arvore) 
isTree _ _ = False 
@

@
isCar :: Int -> (Terreno,[Obstaculo]) -> Bool
isCar x (Estrada _ ,obs) = ((!!) obs x == Carro) 
isCar _ _ = False
@

====Exemplos de utilização da função:

>>>verLimites (Jogador (1,2)) [(Rio 1, [Tronco,Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum])]
False

>>>verLimites (Jogador (1,3)) [(Rio 1, [Tronco,Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum])]
False

>>>verLimites (Jogador (4,3)) [(Rio 1, [Tronco,Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum])]
False

>>>verLimites (Jogador ((-1),1)) [(Rio 1, [Tronco,Tronco,Nenhum,Nenhum]), (Relva, [Nenhum,Arvore,Nenhum])]
False

>>>verLimites (Jogador (1,2)) [(Rio 1, [Tronco,Tronco,Nenhum]),(Relva, [Arvore,Nenhum,Nenhum]), (Estrada 2, [Carro,Nenhum,Nenhum])]
True

>>>verLimites (Jogador (1,0)) [(Relva, [Arvore,Arvore,Nenhum]), (Estrada 1, [Carro,Nenhum,Nenhum])]
False

-}
--4.
verLimites :: Jogador -> [(Terreno,[Obstaculo])] -> Bool                     
verLimites posmov@(Jogador (x,y)) layout@((terr,(h:t)):t2)
     | y <= (-1) || y >= numerolinhas = False
     | x+1 > comprimento  || x < 0 = False
     | not(verificarArvore posmov layout) = False
     | otherwise = True
     where numerolinhas = length layout
           comprimento = length (h:t)
           



verificarArvore :: Jogador -> [(Terreno,[Obstaculo])] -> Bool
verificarArvore (Jogador (x,0)) (m@(Relva,obs):t2) = not (isTree x m)                                   
verificarArvore (Jogador (x,0)) (m@(Estrada _,obs):t2) = not (isCar x m)
verificarArvore (Jogador (x,y)) ((terreno1,(h:t)):t2) | y > 0 = verificarArvore (Jogador (x,y-1)) t2
                                                      |otherwise = True 

isTree :: Int -> (Terreno,[Obstaculo]) -> Bool
isTree x (Relva,obs) = ((!!) obs x == Arvore) 
isTree _ _ = False 

isCar :: Int -> (Terreno,[Obstaculo]) -> Bool
isCar x (Estrada _ ,obs) = ((!!) obs x == Carro) 
isCar _ _ = False