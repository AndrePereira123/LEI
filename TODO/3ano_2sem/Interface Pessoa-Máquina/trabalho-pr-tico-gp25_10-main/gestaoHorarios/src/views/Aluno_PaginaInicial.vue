<script setup>
  import { ref, onMounted } from 'vue'
  import { useAuthStore } from '../stores/auth'

  const authStore = useAuthStore()

  const perfil = ref(null)
  const role = ref('')	
  const id = ref(null)

  onMounted(async () => {
    try {
      role.value = authStore.role
      id.value = authStore.id
      const response = await fetch(`http://localhost:3000/profiles/${id.value}`)
      perfil.value = await response.json()
    } catch (error) {
      console.error('Error fetching perfil:', error)
    }
  })

</script>


<template>
  <main class="pagina-inicial">
    <div class="Caixa_branca">
      <div v-if="error" class="error">
        <p>{{ error }}</p>
        </div>
        <div v-else-if="role" class="role-info">
            <h1 style="font-weight: bold;">Detalhes de {{ role }}</h1>
        </div>
        <div v-else class="loading">
        <p>Carregando detalhes...</p>
        </div>
          <section class="details-card" v-if="perfil">
            <p><span class="label">Nome:</span> {{ perfil.name }}</p>
            <p><span class="label">Idade:</span> {{ perfil.age }} anos</p>
            <p><span class="label">Profissão:</span> {{ perfil.profession }}</p>
            <p><span class="label">Localização:</span> {{ perfil.location }}</p>
            <p><span class="label">Educação:</span> {{ perfil.education }}</p>
            <p><span class="label">Interesses:</span> {{ perfil.interests.join(', ') }}</p>
            <p><span class="label">Objetivos:</span> {{ perfil.objectives }}</p>
            <p><span class="label">Desafios:</span> {{ perfil.challenges }}</p>
            <p><span class="label">Soluções:</span> {{ perfil.solutions }}</p>
            <p><span class="label">Citação:</span> "{{ perfil.quote }}"</p>
        </section>
      <section v-else>
        <p>A carregar perfil...</p>
      </section>
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



.role-info {
  background-color: #D9D9D9;
  padding: 20px;
  width: 90%;
  text-align: center;
  font-size: 2.5rem;
  color: #333E4F;
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

.Caixa_branca h1 {
  font-size: 50px;
}

.content {
  flex: 1;
  display: flex;
  flex-direction: column;
  padding: 20px;
}


.user-actions {
  display: flex;
  align-items: center;
}

.user-actions button {
  padding: 10px;
  background-color: #007bff;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
}

.user-actions button:hover {
  background-color: #0056b3;
}

.user-actions .user-icon {
  margin-left: 10px;
  font-size: 24px;
  color: white;
}



.details-card {
  background-color: #343E4E;
  padding: 20px;
  border-radius: 10px;
  color: white;
  font-size: 1.1rem;
  width: 80%;
  height: 80vh;
  overflow: auto;
  margin-top: -8vh;
}

.details-card .label {
  font-weight: bold;
  color: #00ff00;
}
</style>