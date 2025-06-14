<script setup>
import { ref, onMounted } from 'vue';
import { useRoute } from 'vue-router';
import horarioComponent from '../components/Horario.vue'

const route = useRoute();
const studentId = route.params.id; 
const schedule = ref({});

</script>

<template>
  <main class="schedule-view">
    <horarioComponent v-if="studentId !== ''" :studentId="studentId" :title="'Horário do aluno ' + studentId" />
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
