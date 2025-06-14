<script setup>
import { useAuthStore } from '../stores/auth'
import horarioComponent from '../components/Horario.vue'


const authStore = useAuthStore()

const studentId = authStore.id 

</script>

<template>
  <main class="background-container">
  <horarioComponent 
    v-if="studentId !== ''" 
    :studentId="studentId" 
    :title="'Horário '"
    :path_horario_publicado="`publishedHorarios/${studentId.value}/`"
  />
</main>
</template>

<style scoped>
.schedule-view {
  margin-bottom: 10vh;
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #2c3e50;
  color: white;
  padding: 1.4%;
  height: 100vh;
  width: 100%;
  overflow-x: auto;
}

</style>
