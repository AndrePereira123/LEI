import { defineStore } from 'pinia'

export const useAlunoTrocaManualStore = defineStore('alunotrocamanual', {
  state: () => ({
    Aluno_Para_Troca_Manual: null,
    Id_Aluno_Para_Troca_Manual: null,
  }),
  actions: {
    clear() {
      this.Aluno_Para_Troca_Manual = null
      this.Id_Aluno_Para_Troca_Manual = null
    },
    clearAluno() {
        this.Aluno_Para_Troca_Manual = null
    },
    clearId() {
        this.Id_Aluno_Para_Troca_Manual = null
    }
  }
})
