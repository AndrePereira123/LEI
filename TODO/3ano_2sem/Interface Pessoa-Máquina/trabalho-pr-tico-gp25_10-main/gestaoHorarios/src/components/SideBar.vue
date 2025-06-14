<template>
  <nav class="sidebar">
    <img src="/images/UM_LOGO.png" class="logo" style="margin-bottom: 0%;" />
    <div class="nav-items" style="align-items: center; display: flex; flex-direction: column; justify-content: center; font-size: 1rem;">
      <router-link
        v-if="role !== 'Aluno'"
        to="/consultar-horario-UC"
        class="nav-link"
        :class="{ 'active-link': isActive('/consultar-horario-UC') }"
      >
        Consultar horário de UC
      </router-link>
      <router-link
        v-if="role !== 'Aluno'"
        to="/gestao-alunos"
        class="nav-link"
        :class="{ 'active-link': isActive('/gestao-alunos') }"
      >
        Gestão de Alunos
      </router-link>
      <router-link
        v-if="role !== 'Aluno'"
        to="/publicar-horarios"
        class="nav-link"
        :class="{ 'active-link': isActive('/publicar-horarios') }"
      >
        Publicar Horários
      </router-link>
      <router-link
        v-if="role !== 'Aluno'"
        to="/atendimento-pedidos"
        class="nav-link"
        :class="{ 'active-link': isActive('/atendimento-pedidos') }"
      >
        Atendimento de Pedidos
      </router-link>
      <router-link
        v-if="role == 'Aluno'"
        to="/aluno/horario"
        class="nav-link"
        :class="{ 'active-link': isActive('/aluno/horario') }"
      >
        Consultar horário
      </router-link>
      <router-link
        v-if="role == 'Aluno'"
        to="/aluno/pedidos"
        class="nav-link"
        :class="{ 'active-link': isActive('/aluno/pedidos')}"
      >
        Pedidos
      </router-link>

      <button
        v-if="role != 'Aluno'"
        class="nav-link voltar"
        :class="{ 'active-link': isActive('voltar') }"
        @click="goBack"
      >
        Voltar
      </button>
      <button
        v-if="role == 'Aluno'"
        class="nav-link voltar" 
        :class="{ 'active-link': isActive('voltar') }"
        @click="goBack"
      >
        Voltar
      </button>
    </div>
  </nav>
</template>

<script setup>
import { useRoute } from 'vue-router';
import { useAuthStore } from '../stores/auth'

const authStore = useAuthStore()

const route = useRoute();

const role = authStore.role;

const isActive = (basePath) => {
  return route.path.startsWith(basePath);
};

const goBack = () => {
  window.history.back();
};
</script>

<style scoped>
.sidebar {
  width: 20%; 
  height: 100vh;
  background-color: white;
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 1%;
  box-shadow: 2px 0 5px rgba(0, 0, 0, 0.1);
}

.sidebar .logo {
  width: 100%;
  margin-bottom: 20px;
}

.nav-items {
  display: flex;
  flex-direction: column;
  align-items: center;
  width: 100%;
  height: 100vh;
  overflow: auto;
}

.nav-link {
  display: block;
  width: 80%;
  padding: 10px;
  margin: 10px 0; 
  text-align: center;
  text-decoration: none; 
  color: black; 
  background-color: #e0e0e0; 
  border-radius: 30px; 
  transition: background-color 0.3s ease; 
}

.nav-link:hover {
  background-color: #b8b8b8;
}

.nav-link.voltar {
  margin-top: auto; 
  width: 50%; 
  color: #000000; 
  background-color: #8D8D8D;
}

.nav-link.voltar:hover {
  background-color: #e0e0e0; 
}

.sidebar button:hover {
  background-color: #d6d6d6;
}
  
a.active-link {
  background-color: #333E4F; /* Highlight active link */
  color: white; /* Change text color */
}

.content {
  flex: 1; 
  display: flex;
  flex-direction: column;
  padding: 20px;
}
</style>