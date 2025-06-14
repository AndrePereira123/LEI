##O propósito do programa chacha20_int_attck.py é ilustrar como pode ser manipulada a informação cifrada pelo programa anterior
## se soubermos um fragmento do conteúdo de uma dada posição do texto-limpo, podemos alterar essa informação. 
##O programa chacha20_int_attck.pydeve então receber os seguintes argumentos: <fctxt> <pos> <ptxtAtPos> <newPtxtAtPos>,
##sendo que <fctxt> é o nome do ficheiro contendo o criptograma; <pos> é a posição onde sabemos ter sido cifrado <ptxtAtPos>,
##e <newPtxtAtPos> é o que se pretende vir a obter quando se decifrar o ficheiro. O criptograma manipulado deve ser gravado no
##ficheiro com nome <fctxt>.attck.

## https://github.com/uminho-lei-ssi/2425-SSI/blob/main/guioes/S4.md



import sys
import os

## <fctxt> <pos> <ptxtAtPos> <newPtxtAtPos>

def chacha20_int_attck():
    

    
def main():
    chacha20_int_attck(sys.argv)

if __name__ == "__main__":
    main()