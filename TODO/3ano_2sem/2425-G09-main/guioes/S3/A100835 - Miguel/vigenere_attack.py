
import sys

def preproc(str):
 l = []
 for c in str:
    if c.isalpha():
        l.append(c.upper())
 return "".join(l)



def vigenere_attack(argv):
    tamanho_chave = argv[1]
    frase = preproc(argv[2])
    palavras_limpas = []
    i = 3
    while (argv[i] != None):
       palavras_limpas.append(argv[i])

    print(f"{tamanho_chave},Frase:\"{frase}\",Palavras_limpas: {palavras_limpas}")

    alfabeto = 'abcdefghijklmnopqrstuvwxyz'
    chaves = ['']
    
    for _ in range(tamanho_chave):
        novas_chaves = []
        for chave in chaves:
            for letra in alfabeto:
                novas_chaves.append(chave + letra)
        chaves = novas_chaves
    
    for chave in chaves:
        print(chave)


def main():
    tamanho_chave = 3  # Altere conforme necessário
    vigenere_attack(sys.argv)

if __name__ == "__main__":
    main()