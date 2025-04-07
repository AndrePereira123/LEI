module Main where 

import Graphics.Gloss 
import Graphics.Gloss.Interface.Pure.Game 
import LI12223
import Tarefa1_2022li1g059
import Tarefa2_2022li1g059
import Tarefa3_2022li1g059
import Tarefa4_2022li1g059
import Tarefa5_2022li1g059 
import Tarefa6_2022li1g059
import Data.List
import Data.Maybe

fr :: Int 
fr = 60

dm = (InWindow "GlossyRoad" ((13*100),(1000)) (300,0))


main :: IO ()
main = do
      jogarb <- loadBMP "../imagens/jogar.bmp"
      menub <- loadBMP "../imagens/menu.bmp"
      sairb <- loadBMP "../imagens/sair.bmp"
      bugati <- loadBMP "../imagens/bugatifixed.bmp"
      tronco <- loadBMP "../imagens/troncofixed.bmp"
      arvore <- loadBMP "../imagens/arvorefixed.bmp"
      blank <- loadBMP "../imagens/relva.bmp"
      blank2 <- loadBMP "../imagens/estrada.bmp"
      blank3 <- loadBMP "../imagens/agua.bmp"
      textomorte <- loadBMP "../imagens/morreste.bmp"
      deathscreen <- loadBMP "../imagens/death.bmp"
      galinha1 <- loadBMP "../imagens/galinha1.bmp"
      galinha2 <- loadBMP "../imagens/galinha2.bmp"
      galinha3 <- loadBMP "../imagens/galinha3.bmp"
      galinha4 <- loadBMP "../imagens/galinha4.bmp"
      skingalinha <- loadBMP "../imagens/skin1.bmp"
      skintarta <- loadBMP "../imagens/skin2.bmp"
      turtle1 <- loadBMP "../imagens/turtle1.bmp"
      turtle2 <- loadBMP "../imagens/turtle2.bmp"
      turtle3 <- loadBMP "../imagens/turtle3.bmp"
      turtle4 <- loadBMP "../imagens/turtle4.bmp"
      nuvem   <- loadBMP  "../imagens/nuvem.bmp"
      elefante1 <- loadBMP "../imagens/elefante1.bmp"
      elefante2 <- loadBMP "../imagens/elefante2.bmp"
      elefante3 <- loadBMP "../imagens/elefante3.bmp"
      elefante4 <- loadBMP "../imagens/elefante4.bmp"
      skinelef <- loadBMP "../imagens/skin3.bmp"

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
             (Nenhum, scale 1 1 skintarta),
             (Nenhum, scale 1 1 nuvem),
             (Nenhum, scale 1 1 skinelef)])                            
             (
              [((S 1),scale 1 1 galinha1),
               ((S 2),scale 1 1 galinha2),
               ((S 3),scale 1 1 galinha3),
               ((S 4),scale 1 1 galinha4),
               ((T 1),scale 1 1 turtle1),
               ((T 2),scale 1 1 turtle2),
               ((T 3),scale 1 1 turtle3),
               ((T 4),scale 1 1 turtle4),
               ((E 1),scale 1 1 elefante1),
               ((E 2),scale 1 1 elefante2),
               ((E 3),scale 1 1 elefante3),
               ((E 4),scale 1 1 elefante4)]    
             )      
             )                                
             desenhaEstadoGloss   
             reageEventoGloss    
             reageTempoGloss    


