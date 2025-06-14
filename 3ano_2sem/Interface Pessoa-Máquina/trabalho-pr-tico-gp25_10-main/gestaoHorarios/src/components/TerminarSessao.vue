<template>
    <nav class="terminar-sessao">
        <img src="/images/TERMINAR-SESSAO.png" class="logo-porta" style="height:70%;padding-right: 2.5%;"@click="TerminarSessao"/>
        <img src="/images/user.png" class="logo-user" style="height:80%;"@click="User"/>
    </nav>
  </template>

<script>
import { useAuthStore } from '../stores/auth'

export default {
    data() {
      return {
        auth: useAuthStore()
      }
    },
    methods: {
        TerminarSessao() {
            this.auth.logout() 
            this.$router.push('/');
        },
        User() {
          if (sessionStorage.getItem('role') === 'Aluno') {
            this.$router.push('/aluno/pagina_inicial');
          } else {
            this.$router.push('/pagina_inicial'); 
        }
      }
    }
}
</script>

  <style scoped>

.terminar-sessao {
  width: 100%;
  height: 10%; 
  background-color: #2c3e50;
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: flex-end;
  padding: 1%;
  padding-right: 8%;
  margin-bottom: -1%;
}

a.router-link-active {
    background-color: var(--color-accent);
    color: var(--color-light);
}

.logo-porta {
  cursor: pointer;
}

.logo-user {
  cursor: pointer
}

.content {
  flex: 1; 
  display: flex;
  flex-direction: column;
  padding: 20px;
}
  </style>