<script setup>
import { ref, computed, onMounted } from 'vue';
import { useRouter } from 'vue-router';
import { useAlunoTrocaStore } from '../stores/aluno_troca'

const alunoTrocaStore = useAlunoTrocaStore();

const router = useRouter();
const studentsWithConflicts = ref([]);
const error = ref(null);
const currentPage = ref(1);
const itemsPerPage = ref(2);
const shifts = ref([])
const courses = ref([])

const totalPages = computed(() => {
  return Math.ceil(studentsWithConflicts.value.length / itemsPerPage.value);
});

const paginatedStudents = computed(() => {
  const start = (currentPage.value - 1) * itemsPerPage.value;
  const end = start + itemsPerPage.value;
  return studentsWithConflicts.value.slice(start, end);
});

const nextPage = () => {
  if (currentPage.value < totalPages.value) {
    currentPage.value++;
  }
};

const prevPage = () => {
  if (currentPage.value > 1) {
    currentPage.value--;
  }
};


onMounted(async () => {
  try {

    const studentsResponse = await fetch(`http://localhost:3000/students`);
    if (!studentsResponse.ok) {
      throw new Error('Failed to fetch student');
    }
    const students = await studentsResponse.json();

      students.forEach(student => {
        if (student.conflicts.length > 0){
          studentsWithConflicts.value.push({student})
        }
      });
  
    const coursesResponse = await fetch(`http://localhost:3000/courses`);
    if (!coursesResponse.ok) {
      throw new Error('Failed to fetch courses');
    }
    courses.value = await coursesResponse.json()

    const shiftsResponse = await fetch(`http://localhost:3000/shifts`);
    if (!shiftsResponse.ok) {
      throw new Error('Failed to fetch shifts');
    }
    shifts.value = await shiftsResponse.json()

  } catch (err) {
    error.value = err.message;
    console.error('Error:', err);
  }
});


const getShiftInfo = (shiftId) => {
  // Convert string IDs to numbers if needed
  const shift = shifts.value.find(s => String(s.id) === String(shiftId));
  const course = courses.value.find(c => String(c.id) === String(shift.courseId));
  const courseAbrv = course.abbreviation;
  
  return `${shift.name} - ${courseAbrv}`;
};

const selectAluno = (name,id) => {
  alunoTrocaStore.Aluno_Para_Troca = name;
  alunoTrocaStore.Id_Aluno_Para_Troca = id;

  router.push('/gestao-alunos/troca-turnos');
};

</script>

<template>
  <main class="students-list">
    <div class="Caixa_branca">
      <h1>Gestão de Alunos</h1>
      <div v-if="error" class="error">
        <p>{{ error }}</p>
      </div>
      <div v-else-if="studentsWithConflicts.length > 0" class="students-grid" >
          <h2 >Lista de Alunos com Colisões</h2>
          <div
            v-for="entry in paginatedStudents"
            :key="entry.student.id"
            class="student-item"
          >
            <p>{{ entry.student.id }} | {{ entry.student.name }}
              <span v-if="entry.student.specialStatus" style="color: #FF0000;">*</span>
            </p>
            <div class="actions">
                <div class="conflicting-shifts">
                <ul> 
                  <p>Turnos em Conflito:</p>
                  <li v-for="(conflict, index) in entry.student.conflicts" :key="index" style="overflow: hidden;">
                    <strong>
                      {{ getShiftInfo(conflict.shift1Id) }} ⚠️ 
                      {{ getShiftInfo(conflict.shift2Id) }}
                    </strong>
                  </li>
                </ul>
              </div>
              <router-link
                @click="selectAluno(entry.student.name, entry.student.id)"
                :to="`/gestao-alunos/troca-turnos`"
                class="allocate-button"
              >
                Alocar Turno
              </router-link>
              <router-link
                :to="`/gestao-alunos/ver-horario/${entry.student.id}`"
                class="view-schedule-button"
              >
                Ver horário
              </router-link>
            </div>
          </div>
          <p class="status-explanation">* - Alunos com estatuto</p>  
        <div class="pagination">
          <button
            class="prev-page"
            v-if="currentPage > 1"
            @click="prevPage"
          >
            ◀
          </button>
          <p>{{ currentPage }}/{{ totalPages }}</p>
          <button
            class="next-page"
            v-if="currentPage < totalPages"
            @click="nextPage"
          >
            ▶
          </button>
        </div>
      </div>
      <div v-else class="no-students">
        <p style="color: #0e8b29;font-size: 3rem;font-weight: 700;">Nenhum aluno têm colisões de horário.</p>
      </div>
    </div>
  </main>
</template>

<style scoped>
.status-explanation {
  font-size: 0.9em;
  color: #ffffff;
  text-align: center;
  margin-top: 10px;
  font-style: italic;
} 

.students-list {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100vh;
  align-items: center;
  background-color: #2c3e50;
  color: white;
  padding: 20px;
}


h2 {
  font-size: 2.2em;
  font-weight: bold;
  color: white;
}

.students-grid {
  display: flex;
  flex-direction: column;
  width: 100%;
  align-items: center;
  margin-top: 0px;
  background-color: #2c3e50;
  color: white;
}


.student-item {
  width: 90%;
  height: auto;
  display: flex; 
  flex-direction: row; 
  justify-content: space-between; 
  align-items: center; 
  font-size: 2vh;
  padding: 20px;
  background-color: #333E4F;
  border-radius: 10px;
  color: white;
  
}

.student-item li {
  font-size: 1.5vh;
  overflow: auto;
  height: 3vh;
}

.conflicting-shifts {
  min-width: 20%;
  width: 45%;
  justify-content: center;
  text-align: center;
  font-size: 1.5vh;
  color: white;
}


.conflicting-shifts ul {
  padding : 0;
  text-align: center;
  justify-content: center;
  list-style: none;
  background-color: #C83939;
  color: white;
  width: 100%;
  border-radius: 15px;
  padding-bottom: 2%;
}

.actions {
  display: flex;
  justify-content: space-between;
  width: 80%;
  gap: 2%;
}

.allocate-button {
  background-color: #1F996E;
  color: white;
  border: none;
  border-radius: 15px;
  width: 30%;
  cursor: pointer;
  transition: background-color 0.3s ease;
  font-size: 2vh; 
  width: 30%;
  display: flex;
  justify-content: center;
  align-items: center;

}


.allocate-button:hover {
  background-color: #45a049;
}

.view-schedule-button {
  background-color: #D9D9D9;
  color: #2c3e50;
  border: none;
  border-radius: 15px;
  width: 30%;
  cursor: pointer;
  transition: background-color 0.3s ease;
  font-size: 2vh; 
  display: flex;
  justify-content: center;
  align-items: center;
}

.view-schedule-button:hover {
  background-color: #c0c0c0;
}

.pagination {
  display: flex;
  justify-content: center;
  align-items: center;
  margin-top: auto; 
  width: 100%; 
  padding-bottom: 5%;
  font-family: "josefin-sans", sans-serif;
  font-size: 1.3em;
}

.pagination p {
  font-size: 1rem;
}

.prev-page,
.next-page {
  background-color: #6C63FF;
  color: white;
  border: none;
  border-radius: 50%;
  width: 40px;
  height: 40px;
  font-size: 1.2rem;
  cursor: pointer;
  transition: background-color 0.3s ease;
}

.prev-page:disabled,
.next-page:disabled {
  background-color: #a0a0a0;
  cursor: not-allowed;
}

.prev-page:hover:not(:disabled),
.next-page:hover:not(:disabled) {
  background-color: #5146c6;
}
</style>