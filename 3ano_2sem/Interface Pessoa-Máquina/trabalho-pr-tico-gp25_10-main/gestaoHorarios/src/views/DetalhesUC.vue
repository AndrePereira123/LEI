<script setup>
import { ref, onMounted } from 'vue';
import { useRoute } from 'vue-router';

const route = useRoute();
const course = ref(null);
const shifts = ref([]);
const error = ref(null);

onMounted(async () => {
  try {
    const courseId = route.params.id; // Get the course ID from the route
    console.log('Received course ID from route:', courseId);

    const courseResponse = await fetch(`http://localhost:3000/courses/${courseId}`);
    if (!courseResponse.ok) {
      throw new Error('Failed to fetch course details');
    }
    course.value = await courseResponse.json();
    console.log('Fetched course details:', course.value);

    const shiftsResponse = await fetch(`http://localhost:3000/shifts?courseId=${courseId}`);
    if (!shiftsResponse.ok) {
      throw new Error('Failed to fetch shifts');
    }
    const fetchedShifts = await shiftsResponse.json();

    const shiftsWithDetails = await Promise.all(
      fetchedShifts.map(async (shift) => {
        const classroomResponse = await fetch(`http://localhost:3000/classrooms/${shift.classroomId}`);
        if (!classroomResponse.ok) {
          throw new Error(`Failed to fetch classroom with ID ${shift.classroomId}`);
        }
        const classroom = await classroomResponse.json();

        const buildingResponse = await fetch(`http://localhost:3000/buildings/${classroom.buildingId}`);
        if (!buildingResponse.ok) {
          throw new Error(`Failed to fetch building with ID ${classroom.buildingId}`);
        }
        const building = await buildingResponse.json();

        return { ...shift, classroom: { ...classroom, building } };
      })
    );

    shifts.value = shiftsWithDetails;
    console.log('Fetched shifts with classrooms and buildings:', shifts.value);
  } catch (err) {
    error.value = err.message;
    console.error('Error:', err);
  }
});
</script>

<template>
    <main class="course-details">  
      <div class = "Caixa_branca" >
        <div v-if="error" class="error">
        <p>{{ error }}</p>
        </div>
        <div v-else-if="course" class="course-info">
            <h1 style="font-weight: bold;">Detalhes de {{ course.name }}</h1>
        </div>
        <div v-else class="loading">
        <p>Carregando detalhes...</p>
        </div>
        

        <div v-if="shifts.length > 0" class="shifts" >
          <ul>
              <li v-for="shift in shifts" :key="shift.id" style="display: flex; flex-direction: row;">
                <div class="shift-info">
                  <p>
                    {{ shift.name }} &nbsp;|&nbsp;
                    <strong>Horário:</strong> {{ shift.day }} das {{ shift.from }}h às {{ shift.to }}h &nbsp;|&nbsp;
                    <strong>Capacidade:</strong> {{ shift.totalStudentsRegistered }}/{{ shift.classroom.capacity }} &nbsp;|&nbsp;
                    <strong>Sala:</strong> {{ shift.classroom.building.abbreviation }} {{ shift.classroom.name }}
                  </p>
                </div>
                <router-link
                  :to="`/consultar-horario-UC/curso/${route.params.id}/listar-alunos/${shift.id}`"
                  class="consultar-alunos-link"
                >
                  Consultar<br>Alunos
                </router-link>
                <router-link
                  :to="`/consultar-horario-UC/curso/${route.params.id}/editar-turno/${shift.id}`"
                  class="editar-turno-link"
                >
                  Editar<br>Turno
                </router-link>
              </li>
          </ul>
        </div>
        <div v-else-if="course && shifts.length === 0" class="no-shifts">
        <p>Não há turnos disponíveis para esta UC.</p>
        </div>
     </div>
    </main>
</template>

<style scoped>

.Caixa_branca h1 {
  font-size: 5vh;
}



.course-details {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100vh; 
  align-items: center;
  background-color: #2c3e50;
  color: white;
  padding: 20px;
  
}

.error {
  color: red;
}

.course-info {
  background-color: #D9D9D9;
  padding: 20px;
  width: 90%;
  text-align: center;
  font-size: 2.5em;
  color: #333E4F;
}

.shifts {
  font-size: large;
  background-color: #D9D9D9;
  width: 100%;
  overflow: auto;
}

.shifts ul {
  list-style: none;
  padding: 0;
}


.shift-info {
  flex: 1; 
  margin-bottom: 20px;
  padding: 20px;
  background-color: #333E4F;
  color: #ffffff;
  border-radius: 40px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  display: flex; 
  justify-content: space-between; 
  align-items: center; 
}

.no-shifts {
  margin-top: 20px;
  font-size: 16px;
  color: #ffffff;
}

.consultar-alunos-link {
  margin-left: 20px;
  margin-bottom: 20px;
  padding: 10px 20px;
  min-width: 13%;
  background-color: #6C63FF;
  color: white;
  text-align: center;
  text-decoration: none;
  border-radius: 40px; 
  font-size: 1rem; 
  transition: background-color 0.3s ease;
}

.consultar-alunos-link:hover {
  background-color: #5146c6;
}

.editar-turno-link {
  margin-left: 20px;
  margin-bottom: 20px;
  min-width: 10%;
  padding: 10px 20px;
  background-color: #1F996E;
  color: white;
  text-align: center;
  text-decoration: none;
  border-radius: 40px; 
  font-size: 1rem; 
  transition: background-color 0.3s ease; 
}

.editar-turno-link:hover {
  background-color: #127d57;
}
</style>