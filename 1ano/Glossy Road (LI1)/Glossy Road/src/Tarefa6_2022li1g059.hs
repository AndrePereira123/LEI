{- |
Module      : Tarefa6_2022li1g059
Description : Aplicação gráfica completa
Copyright   : André Filipe Soares Pereira  <a104275@alunos.uminho.pt>
              Leonardo Gomes Alves <a104093@alunos.uminho.pt>

Módulo para a realização da Tarefa 6 do projeto de LI1 em 2022/23.
-}
module Tarefa6_2022li1g059 where 

import Graphics.Gloss 
import Graphics.Gloss.Interface.Pure.Game 
import LI12223
import Tarefa1_2022li1g059
import Tarefa2_2022li1g059
import Tarefa3_2022li1g059
import Tarefa4_2022li1g059
import Tarefa5_2022li1g059 
import Data.List
import Data.Maybe
{-| A Tarefa 6, corresponde à última tarefa do trabalho e, essencialmente trata-se da parte gráfica.
-}

{-| ==Data de Estado:

@
data Estado = Menu 
      |Play 
      |Pause
      deriving Show
@


-}
data Estado = Menu 
      |Play 
      |Pause
      deriving Show 

{-| ==Data das skins:
S corresponde à skin da galinha, T corresponde à skin da tartaruga e E ao elefante.
@
data Skin   = S Int
             |T Int
             |E Int
            deriving (Show,Read,Eq)
@

-}
data Skin   = S Int
             |T Int
             |E Int
            deriving (Show,Read,Eq)

{-| Tipo das skins:
A cada skin corresponde a sua picture.
@
type Skins = [(Skin,Picture)]
@
-}    
type Skins = [(Skin,Picture)]
{-|Tipo das texturas dos obstáculos:
A cada obstáculo corresponde a sua picture.
@
type Texturasobs = [(Obstaculo,Picture)]
@
-}
type Texturasobs = [(Obstaculo,Picture)]
{-|Tipo do GlossState:
O Estado corresponde ao estado do jogo, o parâmetro Int corresponde ao tempo, Texturasobs corresponde às texturas dos obstáculos, skins corresponde
às várias skins, skin corresponde à skin que será utilizada, e o Jogo corresponde ao mapa e o jogador.
@
type GlossState = (Estado,Int,Texturasobs,Skins,Skin,Jogo)
@
-}
type GlossState = (Estado,Int,Texturasobs,Skins,Skin,Jogo)

{-| A função intToFloat transforma uma valor inteiro num float, será utilizada mais tarde como função auxiliar.

@
intToFloat :: Int -> Float
intToFloat n = fromInteger (toInteger n)
@

-}
intToFloat :: Int -> Float
intToFloat n = fromInteger (toInteger n)

{-| Definimos uma função chamada mapainicial que, como o nome diz, corresponde precisamente ao mapa inicial de todos os jogos que se poderão jogar. Definimos também uma função chamada
estadoInicial, que corresponde à posição inicial do jogo e ao mapa inicial.

@
mapainicial = (Mapa 13 [(Estrada (-2), [Nenhum,Nenhum,Carro,Nenhum,Carro,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Carro,Carro,Carro]),
                        (Relva,[Arvore,Arvore,Nenhum,Arvore,Nenhum,Arvore,Nenhum,Nenhum,Arvore,Nenhum,Nenhum,Arvore,Nenhum]),
                        (Rio 2,[Tronco,Tronco,Nenhum,Nenhum,Tronco,Tronco,Nenhum,Tronco,Nenhum,Nenhum,Tronco,Nenhum,Nenhum]),
                        (Estrada 2, [Nenhum,Nenhum,Carro,Nenhum,Carro,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Carro,Carro,Carro]),
                        (Relva,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Relva,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Relva,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Relva,[Arvore,Arvore,Arvore,Nenhum,Nenhum,Nenhum,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore]),
                        (Relva,[Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore]),
                        (Rio 1,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Rio (-1),[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Rio 1,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum])])
@

@
estadoInicial = (Jogo (Jogador (6,6)) mapainicial)
@
-}
estadoInicial = (Jogo (Jogador (6,6)) mapainicial)

mapainicial = (Mapa 13 [
                        (Estrada (-2),[Nenhum,Nenhum,Carro,Nenhum,Carro,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Carro,Carro,Carro]),
                        (Relva,[Arvore,Arvore,Nenhum,Arvore,Nenhum,Arvore,Nenhum,Nenhum,Arvore,Nenhum,Nenhum,Arvore,Nenhum]),
                        (Rio 2,[Tronco,Tronco,Nenhum,Nenhum,Tronco,Tronco,Nenhum,Tronco,Nenhum,Nenhum,Tronco,Nenhum,Nenhum]),
                        (Estrada 2, [Nenhum,Nenhum,Carro,Nenhum,Carro,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Carro,Carro,Carro]),
                        (Relva,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Relva,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Relva,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Relva,[Arvore,Arvore,Arvore,Nenhum,Nenhum,Nenhum,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore]),
                        (Relva,[Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore,Arvore]),
                        (Rio 1,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Rio 1,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Rio 1,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum]),
                        (Rio 1,[Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum,Nenhum])
                        ])
                        

{-| Nesta parte desenvolvemos o menu do jogo, e a partir daqui começamos a usar o Gloss. O que decidimos fazer foi, utilizando imagens, construir um simples menu em que tem a opção de jogar, 
escolher a skin, sair do jogo e um círculo em volta da skin selecionada. Utilizámos bastante o fromJust lookup que nos auxilia em ir buscar as imagens que são correspondentes a cada parte do menú. 

É de realçar que, o tipo GlossState é definido por 
(Estado,[Event],Int,Texturasobs,Skins,Skin,Jogo), onde o Estado corresponde ao estado do programa: Menu (corresponde ao menu), Play (corresponde a quando estámos a jogar), Pause (corresponde ao ecrã de quando perdemos).

Assim, utilizando as posições do jogador no Menu, somos capazes associar cada posição a cada imagem.

Definimos também o construtor do Play, o qual é feito pelo mapa, pelo jogador e a sua respetiva skin e pelo score, o qual mostra a pontuação do jogador.

Temos ainda o Pause, que corresponde ao ecrã que surge quando morremos e, mostra ainda a pontuação do jogador nessa jogada.

Definimos também o estadoGlossInicial, que corresponde precisamente ao início do nosso programa, e é de realçar que, por default, a skin a utilizar é a da galinha.

==Função do ecrã inicial:
@
estadoGlossInicial :: Int -> Texturasobs -> Skins -> GlossState
estadoGlossInicial n tex skins = (Menu,n,tex,skins,S 1, estadoInicial)
@

==Função que desenha o menu:
@
desenhaEstadoGloss :: GlossState -> Picture
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (6,6)) mapa))  = translate 0 0 $ fromJust (lookup Nenhum (drop 8 tex)) -- botao play
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (6,7)) mapa))  = translate 0 0 $ fromJust (lookup Nenhum (drop 9 tex)) -- botao menu
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (6,8)) mapa))  = translate 0 0 $ fromJust (lookup Nenhum (drop 10 tex)) -- botao quit
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (7,7)) mapa))  = if skin == (S 1) 
                                                                               then pictures [(translate 0 0 $ fromJust (lookup Nenhum (drop 11 tex))),  -- skin galinha
                                                                                              ( translate (-460) 20 $ ThickCircle 190 4)]
                                                                               else (translate 0 0 $ fromJust (lookup Nenhum (drop 11 tex)))
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (7,8)) mapa))  = if skin == (T 1) 
                                                                               then pictures [(translate 0 0 $ fromJust (lookup Nenhum (drop 12 tex))),  -- skin galinha
                                                                                              ( translate (-460) 20 $ ThickCircle 190 4)]
                                                                               else (translate 0 0 $ fromJust (lookup Nenhum (drop 12 tex)))
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (7,6)) mapa))  = if skin == (E 1) 
                                                                               then pictures [(translate 0 0 $ fromJust (lookup Nenhum (drop 14 tex))),  -- skin galinha
                                                                                              ( translate (-460) 20 $ ThickCircle 190 4)]
                                                                               else (translate 0 0 $ fromJust (lookup Nenhum (drop 14 tex)))
desenhaEstadoGloss (Pause,n,tex,skins,skin,(Jogo (Jogador (x,y)) mapa)) = pictures ([ (translate (0) 0 $ scale 1.5 1 $ (fromJust (lookup Nenhum $ drop 7 tex))),
                                                                                     (translate (500) 0 $ scale 1.5 1 $ (fromJust (lookup Nenhum $ drop 3 tex))),
                                                                                     (translate 300 (-300) $ scale 0.8 0.8 $ (fromJust (lookup Nenhum $ drop 13 tex)))] 
                                                                                      ++ [(translate (-250) 150 $  pictures $ score)])
                                                                                      where score = scoref n
desenhaEstadoGloss (Play,n,tex,skins,skin,(Jogo (Jogador (x,y)) mapa)) = Pictures ((layout)++[(jogador)]++[(translate (-50) 50 $  pictures $ score)]) 
                                                               where layout = desenhaMapa (-600) (450) mapa tex
                                                                     jogador = desenhaJogador (Jogador (x,y)) skin skins
                                                                     score = scoref n
@

-}

estadoGlossInicial :: Int -> Texturasobs -> Skins -> GlossState
estadoGlossInicial n tex skins = (Menu,n,tex,skins,S 1, estadoInicial)


desenhaEstadoGloss :: GlossState -> Picture
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (6,6)) mapa))  = translate 0 0 $ fromJust (lookup Nenhum (drop 8 tex)) -- botao play
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (6,7)) mapa))  = translate 0 0 $ fromJust (lookup Nenhum (drop 9 tex)) -- botao menu
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (6,8)) mapa))  = translate 0 0 $ fromJust (lookup Nenhum (drop 10 tex)) -- botao quit
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (7,7)) mapa))  = if skin == (S 1) 
                                                                               then pictures [(translate 0 0 $ fromJust (lookup Nenhum (drop 11 tex))),  -- skin galinha
                                                                                              ( translate (-460) 20 $ ThickCircle 190 4)]
                                                                               else (translate 0 0 $ fromJust (lookup Nenhum (drop 11 tex)))
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (7,8)) mapa))  = if skin == (T 1) 
                                                                               then pictures [(translate 0 0 $ fromJust (lookup Nenhum (drop 12 tex))),  -- skin galinha
                                                                                              ( translate (-460) 20 $ ThickCircle 190 4)]
                                                                               else (translate 0 0 $ fromJust (lookup Nenhum (drop 12 tex)))
desenhaEstadoGloss (Menu,n,tex,skins,skin,(Jogo (Jogador (7,6)) mapa))  = if skin == (E 1) 
                                                                               then pictures [(translate 0 0 $ fromJust (lookup Nenhum (drop 14 tex))),  -- skin galinha
                                                                                              ( translate (-460) 20 $ ThickCircle 190 4)]
                                                                               else (translate 0 0 $ fromJust (lookup Nenhum (drop 14 tex)))
desenhaEstadoGloss (Pause,n,tex,skins,skin,(Jogo (Jogador (x,y)) mapa)) = pictures ([ (translate (0) 0 $ scale 1.5 1 $ (fromJust (lookup Nenhum $ drop 7 tex))),
                                                                                     (translate (500) 0 $ scale 1.5 1 $ (fromJust (lookup Nenhum $ drop 3 tex))),
                                                                                     (translate 300 (-300) $ scale 0.8 0.8 $ (fromJust (lookup Nenhum $ drop 13 tex)))] 
                                                                                      ++ [(translate (-300) 150 $  pictures $ score)])
                                                                                      where score = scoref n
desenhaEstadoGloss (Play,n,tex,skins,skin,(Jogo (Jogador (x,y)) mapa)) = Pictures ((layout)++[(jogador)]++[(translate (-150) 50 $  pictures $ score)]) 
                                                               where layout = desenhaMapa (-600) (450) mapa tex
                                                                     jogador = desenhaJogador (Jogador (x,y)) skin skins
                                                                     score = scoref n

{-| A função scoref tem o objetivo de mostrar a pontuação do jogador, a qual é baseada em conforme o tempo passa, a sua pontuação aumenta em 1 de 1 em 1 segundo.
    Para tal, definimos uma função que nos apresenta o valor no ecrã.
==Função principal:
@
scoref :: Int -> [Picture]
scoref n = [(translate 550 (-500) k),(translate 551 (-500) k),(translate 552 (-500) k),(translate 553 (-500) k),(translate 554 (-500) k),(translate 555 (-500) k),
            (translate 550 (-501) k),(translate 551 (-501) k),(translate 552 (-501) k),(translate 553 (-501) k),(translate 554 (-501) k),(translate 555 (-501) k),
            (translate 550 (-502) k),(translate 551 (-502) k),(translate 552 (-502) k),(translate 553 (-502) k),(translate 554 (-502) k),(translate 555 (-502) k)]
            where k = Text $ show $ div n 60
@
-}
scoref :: Int -> [Picture]
scoref n = [(translate 550 (-500) k),(translate 551 (-500) k),(translate 552 (-500) k),(translate 553 (-500) k),(translate 554 (-500) k),(translate 555 (-500) k),
            (translate 550 (-501) k),(translate 551 (-501) k),(translate 552 (-501) k),(translate 553 (-501) k),(translate 554 (-501) k),(translate 555 (-501) k),
            (translate 550 (-502) k),(translate 551 (-502) k),(translate 552 (-502) k),(translate 553 (-502) k),(translate 554 (-502) k),(translate 555 (-502) k)]
            where k = Text $ show $ div n 60 

{-| A função desenhaJogador tem como objetivo transformar o jogador numa Picture, desta forma transformámos as coordenadas do jogador em coordenadas no plano do Gloss e,
 associámos essas coordenadas à skin do jogador.

 ==Função principal:
 @
desenhaJogador :: Jogador -> Skin -> Skins -> Picture
desenhaJogador (Jogador (x,y)) skin skins = translate x2 y2 (fromJust (lookup skin skins)) 
                                                where x2 = (intToFloat x)    *100 -600
                                                      y2 = (intToFloat (-y)) *100 +450
 @
-}
desenhaJogador :: Jogador -> Skin -> Skins -> Picture
desenhaJogador (Jogador (x,y)) skin skins = translate x2 y2 (fromJust (lookup skin skins)) 
                                                where x2 = (intToFloat x)    *100 -600
                                                      y2 = (intToFloat (-y)) *100 +450

{-| A função desenhaMapa tem como objetivo desenhar o mapa mas, primeiramente temos que ser capazes de definir o bloco, de cada linha, e a própria linha para posteriormente definirmos o mapa.

    Desta forma definimos a função desenhaBloco que desenha o bloco conforme o seu tipo, por exemplo quando é relva, vai buscar a imagem associada a esse tipo de terreno utilizando o drop nas texturas e 
    converte-a para Picture, fazemos o mesmo processo para os restantes terrenos e obstáculos, dando apenas rotate no carro por causa de aspetos gráficos.

    De seguida definimos a função desenhaLinha que desenha cada linha, juntando todos os blocos pertencentes às linhas e usámos assim a função desenhaBloco.

    Por fim, utilizámos a função desenhaMapa que o que faz é juntar as Pictures correspondentes de cada linha e forma assim um mapa.

==Função desenhaBloco:
@
desenhaBloco :: Terreno -> Obstaculo -> Texturasobs -> Picture
desenhaBloco Relva       h tex = fromJust (lookup h tex) 
desenhaBloco (Estrada v) h tex = if v > 0 then fromJust (lookup h (drop 1 tex)) 
                                        else rotate 180 (fromJust (lookup h (drop 1 tex))) 
desenhaBloco (Rio _)     h tex = fromJust (lookup h (drop 2 tex))
@

==Função desenhaLinha:
@
desenhaLinha :: Float -> Float -> (Terreno,[Obstaculo])  -> Texturasobs->  [Picture]
desenhaLinha x y (terr,[h])      tex  = [(translate x 0 (desenhaBloco terr h tex))]
desenhaLinha x y (terr,(h:h2:t)) tex  = (translate x 0 (desenhaBloco terr h tex)) : resto
                                      where resto = desenhaLinha (x+100) y (terr,(h2:t)) tex
@

==Função desenhaMapa: 
@
desenhaBloco :: Terreno -> Obstaculo -> Texturasobs -> Picture
desenhaMapa _ _ (Mapa l []) _  = []
desenhaMapa x y (Mapa l k) tex = (translate 0 y (Pictures linha)) : resto
                               where ((terr,(h:t)):t2) = k
                                     resto = desenhaMapa x (y-100) (Mapa l t2) tex
                                     linha = desenhaLinha x y (terr,(h:t)) tex
@

-}
desenhaMapa :: Float -> Float -> Mapa -> Texturasobs -> [Picture]
desenhaMapa _ _ (Mapa l []) _  = []
desenhaMapa x y (Mapa l k) tex = (translate 0 y (Pictures linha)) : resto
                               where ((terr,(h:t)):t2) = k
                                     resto = desenhaMapa x (y-100) (Mapa l t2) tex
                                     linha = desenhaLinha x y (terr,(h:t)) tex

desenhaLinha :: Float -> Float -> (Terreno,[Obstaculo])  -> Texturasobs->  [Picture]
desenhaLinha x y (terr,[h])      tex  = [(translate x 0 (desenhaBloco terr h tex))]
desenhaLinha x y (terr,(h:h2:t)) tex  = (translate x 0 (desenhaBloco terr h tex)) : resto
                                      where resto = desenhaLinha (x+100) y (terr,(h2:t)) tex

desenhaBloco :: Terreno -> Obstaculo -> Texturasobs -> Picture
desenhaBloco Relva       h tex = fromJust (lookup h tex) 
desenhaBloco (Estrada v) h tex = if v > 0 then fromJust (lookup h (drop 1 tex)) 
                                        else rotate 180 (fromJust (lookup h (drop 1 tex))) 
desenhaBloco (Rio _)     h tex = fromJust (lookup h (drop 2 tex)) 


{-| Nesta parte fizemos o movimento das teclas, o que é essencial para o nosso jogo funcionar.
    Para tal usámos as teclas: seta para cima, seta para baixo, seta para a esquerda, seta para a direita e W,S,A,D associadas respetivamente às anteriores. Estas teclas servem para nos movimentar-mos
    no programa em si.

    Definimos também a tecla Enter, que serve para entrar por exemplo no Menu e no Jogo e a tecla "R" que serve para voltar ao menu inicial.

    Tivemos também que definir casos especiais, por exemplo quando clicámos na tecla para baixo no menu inicial ele volta para a primeira opção e assim vice-versa.

    É de realçar também que, tivémos que por os casos para cada skin, por exemplo quando o personagem anda para a esquerda a skin vira também para a esquerda e associámos isso a cada skin que o nosso jogo tem.

==Função principal:
@
aplicarKey :: Event -> GlossState -> GlossState

CASO EM QUE CLICÁMOS NA SETA PARA BAIXO NA ULTIMA OPÇÃO E VOLTA À PRIMEIRA OPÇÃO:

aplicarKey (EventKey (SpecialKey KeyDown) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = if  y == 8  then  (Menu,1,b,c,d,(Jogo (Jogador (x,6)) mapaks)) else (Menu,1,b,c,d,(Jogo (Jogador (x,y+1)) mapaks))

cASO EM QUE CLICÁMOS NA SETA PARA CIMA  NA PRIMEIRA OPÇÃO E VOLTA À PRIMEIRA OPÇÃO:

aplicarKey (EventKey (SpecialKey KeyUp) Down _ _) j@(Menu,keys,a,b,c,d,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = if  y == 6  then  (Menu,1,b,c,d,(Jogo (Jogador (x,8)) mapaks)) else (Menu,[],1,b,c,d,(Jogo (Jogador (x,y-1)) mapaks))

CASOS EM QUE CLICAMOS PARA ENTRAR NO MENU DE ESCOLHA DA SKIN:

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (6,7)) mapaks)) = (Menu,1,b,c,d,(Jogo (Jogador (7,7)) mapaks))
aplicarKey (EventKey (SpecialKey KeyRight) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (6,7)) mapaks)) = (Menu,1,b,c,d,(Jogo (Jogador (7,7)) mapaks))

CASO PARA VOLTAR ATRÁS NO MENU DE SELEÇÃO DE SKIN:

aplicarKey (EventKey (SpecialKey KeyLeft) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo(Jogador (7,y)) mapaks)) = (Menu,1,b,c,d,(Jogo (Jogador(6,7)) mapaks))

CASO PARA FECHAR O JOGO:

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) (Menu,a,b,c,d,(Jogo(Jogador (6,8)) mapaks)) = error "" 

CASO EM QUE SELECIONAMOS A SKIN E FICA ASSIM DEFINIDA PARA O RESTO DO JOGO.

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) (Menu,a,b,c,d,(Jogo(Jogador (7,y)) mapaks)) | y == 6 = (Menu,1,b,c,ef,(Jogo (Jogador(7,6)) mapaks))
                                                                                                 | y == 7 = (Menu,1,b,c,ga,(Jogo (Jogador(7,7)) mapaks))
                                                                                                 | otherwise = (Menu,1,b,c,ta,(Jogo (Jogador(7,8)) mapaks))
                                                                                                      where ga = S 1
                                                                                                            ta = T 1
                                                                                                            ef = E 1


OS SEGUINTES CASOS SÃO REFERENTES À PARTE DA MOVIMENTAÇÃO DO JOGO EM QUE UTILIZAMOS O CASE PARA ASSOCIAR AS SKINS AO MOVIMENTO DAS TECLAS:

aplicarKey (EventKey (SpecialKey KeyUp) Down _ _) j@(Play,a,b,c,skin,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = case skin of
                         S _ -> (Play,a,b,c,S 1,animaJogador j2 (Move Cima)) 
                         T _ -> (Play,a,b,c,T 1,animaTartaruga j2 (Move Cima))
                         E _ -> (Play,a,b,c,E 1,animaJogador j2 (Move Cima))

aplicarKey (EventKey (SpecialKey KeyDown) Down _ _) j@(Play,a,b,c,skin,j2@(Jogo (Jogador (x,y)) mapaks))
                  = case skin of
                        S  _ -> (Play,a,b,c,S 2,animaJogador j2 (Move Baixo))
                        T  _ -> (Play,a,b,c,T 2,animaTartaruga j2 (Move Baixo))
                        E  _ -> (Play,a,b,c,E 2,animaJogador j2 (Move Baixo))s 
                       
aplicarKey (EventKey (SpecialKey KeyLeft) Down _ _)  j@(Play,a,b,c,skin,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = case skin of
                        S _ -> (Play,a,b,c,S 3,animaJogador j2 (Move Esquerda))
                        T _ -> (Play,a,b,c,T 3,animaTartaruga j2 (Move Esquerda))
                        E _ -> (Play,a,b,c,E 3,animaJogador j2 (Move Esquerda))
                    
aplicarKey (EventKey (SpecialKey KeyRight) Down _ _) j@(Play,a,b,c,skin,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = case skin of 
                        S _ -> (Play,a,b,c,S 4,animaJogador j2 (Move Direita))
                        T _ -> (Play,a,b,c,T 4,animaTartaruga j2 (Move Direita))
                        E _ -> (Play,a,b,c,E 4,animaJogador j2 (Move Direita)) 
                       
CASO PARA ENTRAR NA PARTE DE JOGAR:                       

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (6,6)) mapaks)) 
                  = (Play,1,b,c,d,estadoInicial)
            
CASO PARA VOLTAR AO MENU INICIAL QUANDO MORREMOS:

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) j@(Pause,a,b,c,d,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = (Menu,1,b,c,d,estadoInicial)

CASO PARA VOLTAR AO MENU INICIAL EM QUALQUER PARTE DO PROGRAMA UTILIZANDO A TECLA "R":

aplicarKey (EventKey (Char 'r') Down _ _) (est,a,b,c,d,(Jogo (Jogador(x,y)) mapaks)) 
                  = case d of 
                        S _  -> (Menu,1,b,c,S 1,(Jogo (Jogador (6,6)) mapaks)) 
                        T _ -> (Menu,1,b,c,T 1,(Jogo (Jogador (6,6)) mapaks))
                        E _ -> (Menu,1,b,c,T 1,(Jogo (Jogador (6,6)) mapaks))

CASOS QUE ASSOCIAM AS TECLAS WASD ÀS SETAS:

aplicarKey (EventKey (Char 'w') Down a b) j = aplicarKey (EventKey (SpecialKey KeyUp) Down a b) j
aplicarKey (EventKey (Char 's') Down a b) j = aplicarKey (EventKey (SpecialKey KeyDown) Down a b) j
aplicarKey (EventKey (Char 'a') Down a b) j = aplicarKey (EventKey (SpecialKey KeyLeft) Down a b) j
aplicarKey (EventKey (Char 'd') Down a b) j = aplicarKey (EventKey (SpecialKey KeyRight) Down a b) j
aplicarKey _ s = s
@


-}
aplicarKey :: Event -> GlossState -> GlossState
aplicarKey (EventKey (SpecialKey KeyDown) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = if  y == 8  then  (Menu,1,b,c,d,(Jogo (Jogador (x,6)) mapaks)) else (Menu,1,b,c,d,(Jogo (Jogador (x,y+1)) mapaks))

aplicarKey (EventKey (SpecialKey KeyUp) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = if  y == 6  then  (Menu,1,b,c,d,(Jogo (Jogador (x,8)) mapaks)) else (Menu,1,b,c,d,(Jogo (Jogador (x,y-1)) mapaks))

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (6,7)) mapaks)) = (Menu,1,b,c,d,(Jogo (Jogador (7,7)) mapaks))
aplicarKey (EventKey (SpecialKey KeyRight) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (6,7)) mapaks)) = (Menu,1,b,c,d,(Jogo (Jogador (7,7)) mapaks))

aplicarKey (EventKey (SpecialKey KeyLeft) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo(Jogador (7,y)) mapaks)) = (Menu,1,b,c,d,(Jogo (Jogador(6,7)) mapaks))

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) (Menu,a,b,c,d,(Jogo(Jogador (6,8)) mapaks)) = error "" 

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) (Menu,a,b,c,d,(Jogo(Jogador (7,y)) mapaks)) | y == 6 = (Menu,1,b,c,ef,(Jogo (Jogador(7,6)) mapaks))
                                                                                                 | y == 7 = (Menu,1,b,c,ga,(Jogo (Jogador(7,7)) mapaks))
                                                                                                 | otherwise = (Menu,1,b,c,ta,(Jogo (Jogador(7,8)) mapaks))
                                                                                                      where ga = S 1
                                                                                                            ta = T 1
                                                                                                            ef = E 1

aplicarKey (EventKey (SpecialKey KeyUp) Down _ _) j@(Play,a,b,c,skin,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = case skin of
                         S _ -> (Play,a,b,c,S 1,animaJogador j2 (Move Cima)) 
                         T _ -> (Play,a,b,c,T 1,animaTartaruga j2 (Move Cima))
                         E _ -> (Play,a,b,c,E 1,animaJogador j2 (Move Cima))

aplicarKey (EventKey (SpecialKey KeyDown) Down _ _) j@(Play,a,b,c,skin,j2@(Jogo (Jogador (x,y)) mapaks))
                  = case skin of
                        S  _ -> (Play,a,b,c,S 2,animaJogador j2 (Move Baixo))
                        T  _ -> (Play,a,b,c,T 2,animaTartaruga j2 (Move Baixo))
                        E  _ -> (Play,a,b,c,E 2,animaJogador j2 (Move Baixo))  

aplicarKey (EventKey (SpecialKey KeyLeft) Down _ _)  j@(Play,a,b,c,skin,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = case skin of
                        S _ -> (Play,a,b,c,S 3,animaJogador j2 (Move Esquerda))
                        T _ -> (Play,a,b,c,T 3,animaTartaruga j2 (Move Esquerda))
                        E _ -> (Play,a,b,c,E 3,animaJogador j2 (Move Esquerda))
                    
aplicarKey (EventKey (SpecialKey KeyRight) Down _ _) j@(Play,a,b,c,skin,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = case skin of 
                        S _ -> (Play,a,b,c,S 4,animaJogador j2 (Move Direita))
                        T _ -> (Play,a,b,c,T 4,animaTartaruga j2 (Move Direita))
                        E _ -> (Play,a,b,c,E 4,animaJogador j2 (Move Direita))

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) j@(Menu,a,b,c,d,j2@(Jogo (Jogador (6,6)) mapaks)) 
                  = (Play,1,b,c,d,estadoInicial)
            

aplicarKey (EventKey (SpecialKey KeyEnter) Down _ _) j@(Pause,a,b,c,d,j2@(Jogo (Jogador (x,y)) mapaks)) 
                  = (Menu,1,b,c,d,estadoInicial)

aplicarKey (EventKey (Char 'r') Down _ _) (est,a,b,c,d,(Jogo (Jogador(x,y)) mapaks)) 
                  = case d of 
                        S _  -> (Menu,1,b,c,S 1,(Jogo (Jogador (6,6)) mapaks)) 
                        T _ -> (Menu,1,b,c,T 1,(Jogo (Jogador (6,6)) mapaks))
                        E _ -> (Menu,1,b,c,T 1,(Jogo (Jogador (6,6)) mapaks))


aplicarKey (EventKey (Char 'w') Down a b) j = aplicarKey (EventKey (SpecialKey KeyUp) Down a b) j
aplicarKey (EventKey (Char 's') Down a b) j = aplicarKey (EventKey (SpecialKey KeyDown) Down a b) j
aplicarKey (EventKey (Char 'a') Down a b) j = aplicarKey (EventKey (SpecialKey KeyLeft) Down a b) j
aplicarKey (EventKey (Char 'd') Down a b) j = aplicarKey (EventKey (SpecialKey KeyRight) Down a b) j
aplicarKey _ s = s



{-| A função reageEventoGloss o que faz é basicamente aplicar um Event, no caso pressionar uma tecla, e aplica a tecla ao jogo, fazendo as funções de aplicarKey funcionarem.

==Função principal:
@
reageEventoGloss :: Event -> GlossState -> GlossState
reageEventoGloss ev (e,n,tex,skins,skin,(Jogo (Jogador (x,y)) mapaks)) = aplicarKey ev (e,n,tex,skins,skin,(Jogo (Jogador (x,y)) mapaks))
@

-}

reageEventoGloss :: Event -> GlossState -> GlossState
reageEventoGloss ev (e,n,tex,skins,skin,(Jogo (Jogador (x,y)) mapaks)) = aplicarKey ev (e,n,tex,skins,skin,(Jogo (Jogador (x,y)) mapaks))
                                                                           

{-| A função reageTempoGloss tem como objetivo fazer com que a parte gráfica atualize consoante o tempo vai passando, e desta forma, fizemos com que uma linha nova apareça aproximadamente de 2 em 2 segundos (corresponde ao mod do n 119).

    De forma a randomizar o mapa, adicionámos novamente vários mod ao n que corresponde ao tempo que vai aumentando conforme vamos jogando e, vai assim randomizando usando a função animaJogo que foi previamente
    definida na Tarefa 3.

    Temos ainda definidos os casos de quando o jogador vai para fora do mapa ou é atropelado, o jogador perde e vai para o menu de Pause, ecrâ de morte.

==Função principal:
@
reageTempoGloss :: Float -> GlossState -> GlossState
reageTempoGloss temp (Play,n,tex,skins,o@(T _),j@(Jogo tar@(Jogador (x,y)) ma@(Mapa l ((terr,(h:t)):t2)))) | foraMapa   tar ma || atropelado tar ma   = (Pause,n,tex,skins,o,j)
                                                                                                           | (mod n 119) == 0 = (e,n+1,tex,skins,o, jogdes)
                                                                                                           | (mod n 60)  == 0 = (e,n+1,tex,skins,o, animaJogo 1 (animaJogo 3 (animaJogo 2 (quatro))))
                                                                                                           | (mod n 30)  == 0 = (e,n+1,tex,skins,o, animaJogo 2 (um) )
                                                                                                           | (mod n 20)  == 0 = (e,n+1,tex,skins,o, animaJogo 3 j )
                                                                                                           | (mod n 15)  == 0 = (e,n+1,tex,skins,o, animaJogo 4 j )
                                                                                                           | otherwise = (e,n+1,tex,skins,o,j)
                                                                                                                   where jogdes = deslizaJogo n j
                                                                                                                         e = Play 
                                                                                                                         um = animaJogo 1 j        
                                                                                                                         quatro = animaJogo 4 j 

reageTempoGloss temp (Play,n,tex,skins,skin,j@(Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))))| jogoTerminou j   = (Pause,n,tex,skins,skin,j)
                                                                                                | (mod n 119) == 0 = (e,n+1,tex,skins,skin, jogdes)
                                                                                                | (mod n 60)  == 0 = (e,n+1,tex,skins,skin, animaJogo 1 (animaJogo 3 (animaJogo 2 (quatro))))
                                                                                                | (mod n 30)  == 0 = (e,n+1,tex,skins,skin, animaJogo 2 (um) )
                                                                                                | (mod n 20)  == 0 = (e,n+1,tex,skins,skin, animaJogo 3 j )
                                                                                                | (mod n 15)  == 0 = (e,n+1,tex,skins,skin, animaJogo 4 j )
                                                                                                | otherwise = (e,n+1,tex,skins,skin,j)
                                                                                                      where jogdes = deslizaJogo n j
                                                                                                            e = Play 
                                                                                                            um = animaJogo 1 j        
                                                                                                            quatro = animaJogo 4 j       

reageTempoGloss temp f@(e,n,tex,skins,skin,j@(Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2)))) = f 
@

-}

reageTempoGloss :: Float -> GlossState -> GlossState
reageTempoGloss temp (Play,n,tex,skins,o@(T _),j@(Jogo tar@(Jogador (x,y)) ma@(Mapa l ((terr,(h:t)):t2)))) | foraMapa   tar ma || 
                                                                                                             atropelado tar ma   = (Pause,n,tex,skins,o,j)
                                                                                                           | (mod n 119) == 0 = (e,n+1,tex,skins,o, jogdes)
                                                                                                           | (mod n 60)  == 0 = (e,n+1,tex,skins,o, animaJogo 1 (animaJogo 3 (animaJogo 2 (quatro))))
                                                                                                           | (mod n 30)  == 0 = (e,n+1,tex,skins,o, animaJogo 2 (um) )
                                                                                                           | (mod n 20)  == 0 = (e,n+1,tex,skins,o, animaJogo 3 j )
                                                                                                           | (mod n 15)  == 0 = (e,n+1,tex,skins,o, animaJogo 4 j )
                                                                                                           | otherwise = (e,n+1,tex,skins,o,j)
                                                                                                                   where jogdes = deslizaJogo n j
                                                                                                                         e = Play 
                                                                                                                         um = animaJogo 1 j        
                                                                                                                         quatro = animaJogo 4 j 

reageTempoGloss temp (Play,n,tex,skins,skin,j@(Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2))))| jogoTerminou j   = (Pause,n,tex,skins,skin,j)
                                                                                                | (mod n 119) == 0 = (e,n+1,tex,skins,skin, jogdes)
                                                                                                | (mod n 60)  == 0 = (e,n+1,tex,skins,skin, animaJogo 1 (animaJogo 3 (animaJogo 2 (quatro))))
                                                                                                | (mod n 30)  == 0 = (e,n+1,tex,skins,skin, animaJogo 2 (um) )
                                                                                                | (mod n 20)  == 0 = (e,n+1,tex,skins,skin, animaJogo 3 j )
                                                                                                | (mod n 15)  == 0 = (e,n+1,tex,skins,skin, animaJogo 4 j )
                                                                                                | otherwise = (e,n+1,tex,skins,skin,j)
                                                                                                      where jogdes = deslizaJogo n j
                                                                                                            e = Play 
                                                                                                            um = animaJogo 1 j        
                                                                                                            quatro = animaJogo 4 j       

reageTempoGloss temp f@(e,n,tex,skins,skin,j@(Jogo (Jogador (x,y)) (Mapa l ((terr,(h:t)):t2)))) = f         

{-
      play  dm (greyN 0.5) fr            
             (estadoGlossInicial (1)
             ([(Nenhum, scale 1 1 blank), 
             (Nenhum, scale 1 1 blank2),
             (Nenhum, scale 1 1 blank3),
             (Arvore, scale 1 1 arvore),
             (Tronco, scale 1 1 tronco),
             (Carro, scale 1 1 bugati),
             (Nenhum, scale (0.5) (0.5) textomorte),
             (Nenhum, scale (0.5)  (0.7) deathscreen),
             (Nenhum, scale 1 1 jogarb),
             (Nenhum, scale 1 1 menub),
             (Nenhum, scale 1 1 sairb),
             (Nenhum, scale 1 1 skingalinha),
             (Nenhum, scale 1 1 skintarta)])                            
             (
              [((S 1),scale 1 1 galinha1),
               ((S 2),scale 1 1 galinha2),
               ((S 3),scale 1 1 galinha3),
               ((S 4),scale 1 1 galinha4),
               ((T 1),scale 1 1 turtle1),
               ((T 2),scale 1 1 turtle2),
               ((T 3),scale 1 1 turtle3),
               ((T 4),scale 1 1 turtle4)]    
             )      
             )                                
             desenhaEstadoGloss   
             reageEventoGloss    
             reageTempoGloss    


-}