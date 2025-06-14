import sys

def preproc(str):
 l = []
 for c in str:
    if c.isalpha():
        l.append(c.upper())
 return "".join(l)


def vigenere(argv):
    tipo = argv[1]
    chave = preproc(argv[2])
    print(f"Chave a ser usada: \"{chave}\"")
    frase = preproc(argv[3])

    if(tipo == "enc"):
       print(f"Frase a ser incriptada: \"{frase}\"")
       resposta = ""
       index = 0
       for c in frase:
        resposta += chr((((ord(c) + ord(chave[index%(len(chave))]))%26)) + ord('A')) 
        index += 1          
       print(f"Resposta incpritada: {resposta}")
    else:
       print(f"Frase a ser desincriptada: \"{frase}\"")
       resposta = ""
       index = 0
       for c in frase:
        resposta += chr((((ord(c) - ord(chave[index%(len(chave))]))%26)) + ord('A')) 
        index += 1          
       print(f"Resposta desincriptada: {resposta}")


def main():
    print("Argumentos:", sys.argv)
    vigenere(sys.argv)

if __name__ == "__main__":
    main()
