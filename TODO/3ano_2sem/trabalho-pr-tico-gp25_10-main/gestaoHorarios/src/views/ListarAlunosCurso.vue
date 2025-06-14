<script setup>
import { ref, computed, onMounted } from 'vue';
import { useRouter } from 'vue-router';

const router = useRouter();
const students = ref([]); 
const filteredStudents = ref([]);  
const searchQuery = ref('');
const error = ref(null);
const course = ref(null);
const currentPage = ref(1);
const itemsPerPage = ref(10);

const totalPages = computed(() => {
  return Math.ceil(filteredStudents.value.length / itemsPerPage.value);
});

const paginatedStudents = computed(() => {
  const start = (currentPage.value - 1) * itemsPerPage.value;
  const end = start + itemsPerPage.value;
  return filteredStudents.value.slice(start, end);
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

const filterStudents = () => {
  if (searchQuery.value.trim() === '') {
    filteredStudents.value = students.value;
  } else {
    filteredStudents.value = students.value.filter((student) =>
      student.name.toLowerCase().includes(searchQuery.value.toLowerCase()) ||
      student.id.toString().includes(searchQuery.value)
    );
  }
};

const selectAluno = (idAluno) => {
  router.push(`/gestao-alunos/listar-alunos-curso/${idAluno}`);
};

onMounted(async () => {
  try {
    const courseResponse = await fetch(`http://localhost:3000/courses/1`);
    if (!courseResponse.ok) {
      throw new Error('Failed to fetch course details');
    }
    course.value = await courseResponse.json();

    const studentsResponse = await fetch(`http://localhost:3000/students`);
    if (!studentsResponse.ok) {
      throw new Error('Failed to fetch students');
    }
    students.value = await studentsResponse.json();
    filteredStudents.value = students.value; 
  } catch (err) {
    error.value = err.message;
    console.error('Error:', err);
  }
});
</script>

<template>
  <main class="students-list">
    <div v-if="error" class="Caixa_branca">
      <p>{{ error }}</p>
    </div>
    <div v-else class="Caixa_branca">
      <h1>Lista de Alunos</h1>

      <!-- Barra de Pesquisa -->
      <div class="search-bar">
        <input
          type="text"
          v-model="searchQuery"
          @input="filterStudents"
          placeholder="Pesquisar"
        />
        <img src="/images/lupa.png" alt="Search" class="search-icon" />
      </div>

      <!-- Lista de Alunos -->
      <div class="caixa_2">
        <div class="students-grid">
          <div
            v-for="student in paginatedStudents"
            :key="student.id"
            class="student-item"
            @click="selectAluno(student.id)"
          >
            <p>{{ student.id }} | {{ student.name }}</p>
          </div>
        </div>

        <!-- Paginação -->
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
    </div>
  </main>
</template>

<style scoped>
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

.search-bar input::placeholder {
  text-decoration: underline; 
  color: #aaa; 
}

.Caixa_branca {
  display: flex;
  flex-direction: column;
  background-color: #d9d9d9;
  width: 90%;
  height: 80vh;
  align-items: center;
  padding: 20px;
  border-radius: 20px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.search-bar {
  display: flex;
  align-items: center;
  width: 85%;
  margin-bottom: 20px;
  height: 5vh;
}

.search-bar input {
  flex: 1;
  height:100%;
  padding: 10px;
  border: 1px solid #ccc;
  border-radius: 8px;
  font-size: 1rem;
  z-index: 0;
}

.search-bar .search-icon {
  position: flex; 
  top: 50%; 
  left: 10px;  
  transform: translateX(-150%); 
  width: 4%; 
  pointer-events: none; 
  z-index: 1;
  height: 100%;
}

.caixa_2 {
  display: flex;
  background-color: #333e4f;
  width: 60%;
  border-radius: 30px;
  flex-direction: column;
  align-items: center;
  height: 55vh;
  width: 80vh;
}

.students-grid {
  display: grid;
  grid-template-columns: repeat(2, 1fr); 
  gap: 1vh; 
  column-gap: 6vh; 
  padding: 1% 5%;
  background-color: #333e4f;
  color: white;
  border-radius: 20px;
  align-content: start; 
  position: relative; 
  height: 45vh;
  width: 70vh;
  margin-top: 5vh;
}



.student-item {
  font-size: 100%;
  padding: 3%;
  border-bottom: 2px solid white;
  cursor: pointer; /* Indica que o item é clicável */
  transition: background-color 0.3s ease;
}

.student-item:hover {
  background-color: #444;
  color: white;
}



</style>