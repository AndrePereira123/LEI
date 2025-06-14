<script setup>
import { ref, onMounted } from 'vue';
import { useRoute } from 'vue-router';


</script>

<template>
  <main class="student-management">
    <div class="Caixa_branca">
        <h1>Gestão de Alunos</h1>
        <div class="options">
          <router-link
            to="/gestao-alunos/listar-alunos-horario-incompleto"
            class="option red"
            style="color: white; text-decoration: none;"
          >
             Alunos com horário incompleto
          </router-link>
          <router-link
            to="/gestao-alunos/listar-alunos-colisoes"
            class="option dark"
            style="color: white; text-decoration: none;"
          >
            Alunos com colisões 
          </router-link>
          <router-link
            to="/gestao-alunos/alocacao-manual-turno"
            class="option green"
            style="color: white; text-decoration: none;"
          >
            Alocação manual de alunos
          </router-link>
          <router-link
            to="/gestao-alunos/listar-alunos-curso"
            class="option blue"
            style="color: white; text-decoration: none;"
          >
            Alunos inscritos no curso
          </router-link>
          <router-link
            to="/gestao-alunos/troca-turnos"
            class="option orange"
            style="color: white; text-decoration: none;"
          >
            Troca de Turnos
          </router-link>

          <router-link
            to="/gestao-alunos/remover-alocacao"
            class="option purple"
            style="color: white; text-decoration: none;"
          >
            Remover alocação de aluno
          </router-link>
        </div>
    </div>
  </main>
</template>

<style scoped>

.Caixa_branca {
  display: flex;
  flex-direction: column;
  background-color: #D9D9D9;
  width: 90%;
  height: 80vh;
  align-items: center;
  padding: 20px;
  border-radius: 20px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.student-management {
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #313E4D;
  height: 100vh;
  padding: 20px;
  color: #333e4f;
}

.options {
  display: flex;
  flex-direction: column;
  gap: 2vh;
  width: 110vh;
  overflow: auto;
}

.option {
  display: flex;
  height: 9vh;
  align-items: center;
  justify-content: center;
  padding: 20px;
  border-radius: 20px;
  font-size: 1.6em;
  font-weight: 500;
  color: white;
  text-align: center;
}

.red {
  background-color: #C94D3F; 
}

.dark {
  background-color: #2F3B4A; 
}

.green {
  background-color: #4CAF50; 
}

.blue {
  background-color: #5A5CE6; 
}

.orange {
  background-color: #E68A2E; 
}

.purple {
  background-color: purple;
}
</style>