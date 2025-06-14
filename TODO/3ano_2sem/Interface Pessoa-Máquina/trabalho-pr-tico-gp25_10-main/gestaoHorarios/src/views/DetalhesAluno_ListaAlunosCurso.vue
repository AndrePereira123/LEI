<script setup>
import { ref, onMounted } from 'vue';
import { useRoute, useRouter } from 'vue-router';

const route = useRoute();
const router = useRouter();
const aluno = ref(null);
const error = ref(null);
const id = route.params.idAluno;

const navigateToVerHorario = () => {
  router.push(`/gestao-alunos/ver-horario/${id}`);
};

onMounted(async () => {
  try {
    const response = await fetch(`http://localhost:3000/profiles/${id}`);
    if (!response.ok) {
      throw new Error('Erro ao buscar os detalhes do aluno');
    }
    aluno.value = await response.json();
  } catch (err) {
    error.value = err.message;
    console.error('Erro ao carregar os detalhes do aluno:', err);
  }
});
</script>

<template>
  <main class="pagina-inicial">
    <div class="Caixa_branca">
      <h1>Detalhes de Aluno</h1>
      <div v-if="error" class="error">
        <p>{{ error }}</p>
      </div>
      <div v-else-if="aluno" class="details-card">
        <p><span class="label">Nome:</span> {{ aluno.name }}</p>
        <p><span class="label">Número:</span> {{ aluno.id }}</p>
        <p><span class="label">Idade:</span> {{ aluno.age }} anos</p>
        <p><span class="label">Profissão:</span> {{ aluno.profession }}</p>
        <p><span class="label">Localização:</span> {{ aluno.location }}</p>
        <p><span class="label">Educação:</span> {{ aluno.education }}</p>
        <p><span class="label">Interesses:</span> {{ aluno.interests }}</p>
        <p><span class="label">Objetivos:</span> {{ aluno.objectives }}</p>
        <p><span class="label">Desafios:</span> {{ aluno.challenges }}</p>
        <p><span class="label">Soluções:</span> {{ aluno.solutions }}</p>
        <p><span class="label">Citação:</span> "{{ aluno.quote }}"</p>
        
      </div>
      <div v-else class="loading">
        <p>A carregar detalhes do aluno...</p>
      </div>


      <button
        class="btn green"
        @click="navigateToVerHorario"
        style="margin-top: 2vh;"
      >
        Ver horário
      </button>

    </div>
  </main>
</template>

<style scoped>
.pagina-inicial {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100vh;
  align-items: center;
  background-color: #2c3e50;
  color: white;
  padding: 20px;
}

h1 {
  font-size: 3.5em;
  font-weight: bold;
  margin-bottom: 40px;
  color: #333e4f;
}

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

.details-card {
  background-color: #343E4E;
  padding: 20px;
  border-radius: 10px;
  color: white;
  font-size: 1.1rem;
  width: 80%;
  overflow: auto;
}

.details-card .label {
  font-weight: bold;
  color: #00ff00;
}

.error {
  color: red;
  font-size: 1.2rem;
}

.loading {
  font-size: 1.2rem;
  color: #333E4F;
}

.btn {
  padding: 10px 20px;
  border: none;
  border-radius: 5px;
  font-size: 1rem;
  cursor: pointer;
  color: white;
}

.btn.green {
  background-color: #1F996E; 
  color: white;
  margin-left: 3%;
  padding: 1% 6%;
  border: none;
  border-radius: 30px;
  font-size: 1rem;
  cursor: pointer;
  white-space: nowrap; 
  display: flex;
  justify-content: center;
  align-items: center;
  margin: 0 auto;
}

</style>