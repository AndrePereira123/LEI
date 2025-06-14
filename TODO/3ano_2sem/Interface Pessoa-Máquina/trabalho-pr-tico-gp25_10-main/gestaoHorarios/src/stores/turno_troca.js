import { defineStore } from 'pinia'

export const useTurnoTrocaStore = defineStore('turnotroca', {
  state: () => ({
    Turno_Para_Troca: null,
    Id_Turno_Para_Troca: null,
  }),
  actions: {
    clear() {
      this.Turno_Para_Troca = null
      this.Id_Turno_Para_Troca = null
    },
    clearTurno() {
        this.Turno_Para_Troca = null
    },
    clearId() {
        this.Id_Turno_Para_Troca = null
    }
  }
})
