export class Aluno {
    id = null
    email = ""
    password = ""
    nome = ""
    specialstatus = false
    enrolled = []


    constructor() {
      
    }

    async login(email, password) {
      const query = `http://localhost:3000/students?email=${email}`

      try {
        const response = await fetch(query)
        const lista = await response.json();
        const aluno = lista[0]

        if(aluno.password == password){
            this.carregaAluno(aluno)
            return true
        }
      } 
      catch (error) {
        console.error("Erro ao aceder à data:",error);
      }
      return false
  }

  carregaAluno(aluno) {
    this.id = aluno.id
    this.email = aluno.email
    this.password = aluno.password
    this.nome = aluno.nome
    this.specialstatus = aluno.specialstatus
  }

}