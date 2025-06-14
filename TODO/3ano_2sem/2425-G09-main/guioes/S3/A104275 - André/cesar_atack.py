def preproc(str):
 l = []
 for c in str:
    if c.isalpha():
        l.append(c.upper())
 return "".join(l)

def cesar_attack():
    criptograma = input("Insira o criptograma: ")
    print(f"Criptograma = {criptograma}")

    s = False
    palavras = []
    while (s != True):
        palavras.append(preproc(input("Insira uma palavra a ecnontrar no texto-limpo: ")))
        if (input("Pretende inserior mais palavras?(S/N) - ") == "N" or "n"):
            s = True

    print(f"Palavras a testar: {palavras}")

    for palavra in palavras:
        i = ord('A')
        while(i <= ord('Z')):
            resposta = ""
            for c in criptograma:
                resposta += chr(((((ord(c) - i))%26)) + ord('A'))
            if palavra in resposta:
                print(chr(i))
                print(resposta)
                i += 26
            i += 1

cesar_attack()