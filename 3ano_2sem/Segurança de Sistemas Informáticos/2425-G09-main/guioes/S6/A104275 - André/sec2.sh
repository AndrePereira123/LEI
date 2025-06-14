## r 4
## w 2 
## x 1


 > cd /etc
 > cat passwd
 > cat group


 > sudo adduser andre
    -pass:Andre
 > sudo adduser miguel 
    -pass:Miguel
 > sudo adduser marco
    -pass:Marco
 > sudo groupadd grupo_ssi
 > sudo groupadd par_ssi
 > sudo usermod -aG grupo_ssi andre
 > sudo usermod -aG grupo_ssi marco
 > sudo usermod -aG grupo_ssi miguel
 > sudo usermod -aG par_ssi marco
 > sudo usermod -aG par_ssi andre
    ## - O ficheiro passwd tem, agora, no final 3 novas entradas com os detalhes 
    ## dos 3 utilizadores criados (andre/marco/miguel)
    ## - O fichiero group contem 2 novos registos "grupo_ssi" e "par_ssi", cada um
    ## com a lista de nomes dos users que pertencem aos grupos.
 
 > cd /home/ubuntu
 > sudo chown andre braga.txt
 > cat braga.txt 
 ## permission denied
 
 > su andre

 ##como estava na pasta de ubuntu tive de alterar
 ##para a pasta andre:
 > cd ..
 > cd andre
 > nano braga.txt 
 > chown andre braga.txt  
 
 > id
    - uid=1001(andre) gid=1001(andre) groups=1001(andre),100(users),1005(grupo_ssi),1006(par_ssi)
    ##podemos ver os grupos etc.
 > groups
    - andre users grupo_ssi par_ssi
    ##podemos ver o nome dos grupos a que pertence
 > cat braga.txt
    ##desta vez podemos abrir o ficheiro
 
 > mkdir dir2
 > exit
 > chmod 766 dir2
 > su andre
 > cd dir2
 ## a permissao foi negada