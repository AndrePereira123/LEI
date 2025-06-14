def attack(fcxt, pos, ptxtAtPos, newPtxtAtPos):
    with open(fcxt, "rb") as f_txt:
        ciphertext = bytearray(f_txt.read())

    ptxt_bytes = ptxtAtPos.encode()
    new_ptxt_bytes = newPtxtAtPos.encode()

    if len(ptxt_bytes) != len(new_ptxt_bytes):
        raise ValueError("O novo texto precisa ter o mesmo tamanho que o original para preservar a estrutura da cifra de fluxo.")

    for i in range(len(ptxt_bytes)):
        ciphertext[pos + i] ^= ptxt_bytes[i] ^ new_ptxt_bytes[i]

    output_filename = "teste.txt" + ".attck"
    
    with open(output_filename, "wb") as f_out:
        f_out.write(ciphertext)

attack("file.txt.txt.enc", 16, "MENSAGEMCIFRADA", "MENSAGEDCBA----")
