export class Professor {
    id = null
    email = ""
    password = ""
    nome = ""

    constructor() {
      
    }

    async login(email, password) {
      const query = `http://localhost:3000/teachers?email=${email}`

      try {
        const response = await fetch(query)
        const lista = await response.json();
        const teacher = lista[0]

        if(teacher.password == password){
            this.carregaProfessor(teacher)
            return true
        }
      } 
      catch (error) {
        console.error("Erro ao aceder à data:",error);
      }
      return false
  }

  carregaProfessor(professor) {
    this.id = professor.id
    this.email = professor.email
    this.password = professor.password
    this.nome = professor.nome
  }

}