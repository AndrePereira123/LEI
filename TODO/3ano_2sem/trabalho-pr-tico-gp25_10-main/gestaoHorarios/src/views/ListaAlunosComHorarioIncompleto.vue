<script setup>
import { ref, computed, onMounted } from 'vue';
import { useRoute } from 'vue-router';
import { useAlunoTrocaManualStore } from '../stores/aluno_troca_manual'

const alunoTrocaManualStore = useAlunoTrocaManualStore();

const route = useRoute();
const students = ref([]);
const studentsWithIncompleteSchedule = ref([]);
const courses = ref([]);
const allocations = ref([]);
const error = ref(null);
const currentPage = ref(1);
const itemsPerPage = ref(3);

const totalPages = computed(() => {
  return Math.ceil(studentsWithIncompleteSchedule.value.length / itemsPerPage.value);
});

const paginatedStudents = computed(() => {
  const start = (currentPage.value - 1) * itemsPerPage.value;
  const end = start + itemsPerPage.value;
  return studentsWithIncompleteSchedule.value.slice(start, end);
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
      throw new Error('Failed to fetch students');
    }
    students.value = await studentsResponse.json();

    const coursesResponse = await fetch(`http://localhost:3000/courses`);
    if (!coursesResponse.ok) {
      throw new Error('Failed to fetch courses');
    }
    courses.value = await coursesResponse.json();

    const allocationsResponse = await fetch(`http://localhost:3000/allocations`);
    if (!allocationsResponse.ok) {
      throw new Error('Failed to fetch allocations');
    }
    allocations.value = await allocationsResponse.json();

    for (const student of students.value) {
      let requiredShifts = {};
      let allocatedShifts = {};

      for (const enrolledCourseId of student.enrolled) {
        const course = courses.value.find((course) => String(course.id) === String(enrolledCourseId));
        if (course && course.requiredShifts) {
          for (const shiftType of course.requiredShifts) {
            if (!requiredShifts[shiftType]) {
              requiredShifts[shiftType] = { name: course.name, count: 0 };
            }
            requiredShifts[shiftType].count += 1;
          }
        }
      }

      for (const allocation of allocations.value) {
        if (String(allocation.studentId) === String(student.id)) {
          const shiftType = allocation.shiftType; 
          allocatedShifts[shiftType] = (allocatedShifts[shiftType] || 0) + 1;
        }
      }

      let missingShifts = [];
      for (const [shiftType, tuple] of Object.entries(requiredShifts)) {
        const allocatedCount = allocatedShifts[shiftType] || 0;
        if (allocatedCount < tuple.count) {
          missingShifts.push({
            type: shiftType,
            missing: tuple.count - allocatedCount,
            courses: [tuple.name], 
          });
        }
      }

      if (missingShifts.length > 0) {
        studentsWithIncompleteSchedule.value.push({
          student,
          missingShifts,
        });
      }
    }

    if (studentsWithIncompleteSchedule.value.length === 0) {
      console.log("Todos os alunos têm horários completos.");
    }
  } catch (err) {
    error.value = err.message;
    console.error('Error:', err);
  }
});

const selectAluno = (name,id) => {
  alunoTrocaManualStore.Aluno_Para_Troca_Manual = name;
  alunoTrocaManualStore.Id_Aluno_Para_Troca_Manual = id;
};

</script>

<template>
  <main class="students-list">
    <div class="Caixa_branca">
      <h1>Gestão de Alunos</h1>
      <div v-if="error" class="error">
        <p>{{ error }}</p>
      </div>
      <div v-else-if="studentsWithIncompleteSchedule.length > 0" class="students-grid" >
          <h2 >Lista de Alunos com horário incompleto</h2>
          <div
            v-for="entry in paginatedStudents"
            :key="entry.student.id"
            class="student-item"
          >
            <p>
              {{ entry.student.id }} | {{ entry.student.name }} 
                <span v-if="entry.student.specialStatus" style="color: #FF0000;">*</span>
            </p>
            <div class="actions">
                <div class="missing-shifts">
                <ul> 
                  <p>Turnos em falta:</p>
                  <li v-for="missing in entry.missingShifts" :key="missing.type">
                    <strong >{{ missing.type }}</strong> - {{ missing.courses.join(', ') }}
                  </li>
                </ul>
              </div>
              <router-link
                @click="selectAluno(entry.student.name, entry.student.id)"
                :to="`/gestao-alunos/alocacao-manual-turno`"
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
        <p style="color: #0e8b29;font-size: 3rem;font-weight: 700;">Todos os alunos têm horários completos.</p>
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

.students-grid {
  display: flex;
  flex-direction: column;
  width: 100%;
  background-color: #2c3e50;
  color: white;
  margin-top: 0px;
}

h2 {
  font-size: 2.2em;
  font-weight: bold;
  color: white;
}


.student-item {
  display: flex; 
  flex-direction: row; 
  justify-content: space-between; 
  align-items: center; 
  font-size: 1.2em;
  padding: 20px;
  background-color: #333E4F;
  border-radius: 10px;
  color: white;
}


.missing-shifts {
  min-width: 20%;
  width: 45%;
  justify-content: center;
  text-align: center;
  font-size: 0.8em;
  color: white;
}


.missing-shifts ul {
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
  display: flex;
  justify-content: center;
  align-items: center;
  background-color: #1F996E;
  color: white;
  width: 30%;
  font-size: 2vh;
  border-radius: 15px;
  cursor: pointer;
  transition: background-color 0.3s ease;
}

.allocate-button:hover {
  background-color: #45a049;
}



.view-schedule-button {
  display: flex;
  justify-content: center;
  align-items: center;
  background-color: #D9D9D9;
  color: #2c3e50;
  width: 30%;
  font-size: 2vh;
  border-radius: 15px;
  cursor: pointer;
  transition: background-color 0.3s ease;
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