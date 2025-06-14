import { defineStore } from 'pinia'

export const useAlunoTrocaStore = defineStore('alunotroca', {
  state: () => ({
    Aluno_Para_Troca: null,
    Id_Aluno_Para_Troca: null,
  }),
  actions: {
    clear() {
      this.Aluno_Para_Troca = null
      this.Id_Aluno_Para_Troca = null
    },
    clearAluno() {
        this.Aluno_Para_Troca = null
    },
    clearId() {
        this.Id_Aluno_Para_Troca = null
    }
  }
})
