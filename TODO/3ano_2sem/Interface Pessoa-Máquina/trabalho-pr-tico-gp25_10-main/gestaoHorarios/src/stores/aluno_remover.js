import { defineStore } from 'pinia'

export const useAlunoRemoverStore = defineStore('alunoremover', {
  state: () => ({
    Aluno_Para_Remover: null,
    Id_Aluno_Para_Remover: null,
  }),
  actions: {
    clear() {
      this.Aluno_Para_Remover = null
      this.Id_Aluno_Para_Remover = null
    },
    clearAluno() {
        this.Aluno_Para_Remover = null
    },
    clearId() {
        this.Id_Aluno_Para_Remover = null
    }
  }
})
