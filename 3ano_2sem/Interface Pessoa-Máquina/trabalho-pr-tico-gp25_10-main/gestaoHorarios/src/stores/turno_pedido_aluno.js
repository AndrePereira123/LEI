import { defineStore } from 'pinia'

export const useTurnoPedidoAlunoStore = defineStore('turnopedidoaluno', {
  state: () => ({
    Turno_Para_Pedido_Aluno: null,
    Id_Turno_Para_Pedido_Aluno: null,
  }),
  actions: {
    clear() {
      this.Turno_Para_Pedido_Aluno = null
      this.Id_Turno_Para_Pedido_Aluno = null
    },
    clearTurno() {
        this.Turno_Para_Pedido_Aluno = null
    },
    clearId() {
        this.Id_Turno_Para_Pedido_Aluno = null
    }
  }
})
