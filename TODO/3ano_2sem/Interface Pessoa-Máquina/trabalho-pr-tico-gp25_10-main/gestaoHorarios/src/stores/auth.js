import { defineStore } from 'pinia'
import { Aluno } from '../models/Aluno'
import { Diretor } from '../models/Diretor'

export const useAuthStore = defineStore('auth', {
  state: () => ({
    id: null,
    role: null,
    isAuthenticated: false
  }),
  actions: {
    async login(email, password) {
      const aluno = new Aluno()
      const diretor = new Diretor()

      let isDirectorLoggedIn = await diretor.login(email, password)
      if (isDirectorLoggedIn) {
        this.id = diretor.id
        this.role = 'Diretor'
        this.isAuthenticated = true
        return 'diretor'
      }

      let isStudentLoggedIn = await aluno.login(email, password)
      if (isStudentLoggedIn) {
        this.id = aluno.id
        this.role = 'Aluno'
        this.isAuthenticated = true
        return 'aluno'
      }

      return null
    },
    checkAuth() {
        return this.isAuthenticated;
    },
    logout() {
      this.id = null
      this.role = null
      this.isAuthenticated = false
    },
  },
  persist: { // persistência no ficheiro
    enabled: true,
    strategies: [
      {
        key: 'auth',
        storage: sessionStorage,
      },
    ],
  },
})
