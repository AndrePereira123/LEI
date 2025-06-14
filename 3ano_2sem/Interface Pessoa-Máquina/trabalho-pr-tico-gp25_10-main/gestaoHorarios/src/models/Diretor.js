export class Diretor {
    id = null
    email = ""
    password = ""
    nome = ""

    constructor() {
      
    }

    async login(email, password) {
      const query = `http://localhost:3000/directors?email=${email}`

      try {
        const response = await fetch(query)
        const lista = await response.json();
        const diretor = lista[0]

        if(diretor.password == password){
            this.carregaDiretor(diretor)
            return true
        } 
      } 
      catch (error) {
        console.error("Erro ao aceder à data:",error);
      }
      return false
  }

  carregaDiretor(diretor) {
    this.id = diretor.id
    this.email = diretor.email
    this.password = diretor.password
    this.nome = diretor.nome
  }

}