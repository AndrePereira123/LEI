def preproc(str):
 l = []
 for c in str:
    if c.isalpha():
        l.append(c.upper())
 return "".join(l)

def cesar():
   d = None
   while (d != "enc" and d != "dec"):
    if (d != None): 
      print("Valor inserido é invalido\n")
    d = input("Insira o tipo de operação (enc ou dec)")
   
   chave = None
   while (chave == None or len(chave) != 1):
    if (chave != None):
      print("Valor inserido é invalido\n")
    chave = preproc(input("Insira a chave secreta - "))

   print(f"Chave: {chave}")

   input_frase = None
   while (input_frase == None):
     if (input_frase != None):
      print("Valor inserido é invalido\n")
     if (d == "enc"):
        input_frase = (input("Insira a frase a incriptar - "))
     else:
        input_frase = (input("Insira a frase a decriptar - "))

   frase = preproc(input_frase)
   resposta = ""

   if (d == "enc"):
    print(f"Frase Original:\"{frase}\"") 
    for c in frase:
     resposta += chr((((ord(c) + ord(chave))%26)) + ord('A'))
    print(f"Frase Incriptada:\"{resposta}\"")
   else:
    print(f"Frase Incriptada:\"{frase}\"")
    for c in frase:
     resposta += chr((((ord(c) - ord(chave))%26)) + ord('A'))
    print(f"Frase Original:\"{resposta}\"")
  
    
   
cesar()