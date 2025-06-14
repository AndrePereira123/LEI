<script setup>
  import { ref, onMounted } from 'vue'

    const firstYearFirstSemester = ref([]);
    const firstYearSecondSemester = ref([]);
    const secondYearFirstSemester = ref([]);
    const secondYearSecondSemester = ref([]);
    const thirdYearFirstSemester = ref([]);
    const thirdYearSecondSemester = ref([]);

    onMounted(async () => {
    try {
        const fetchCourses = async (year, semester) => {
        const response = await fetch(`http://localhost:3000/courses?year=${year}&semester=${semester}`);
        return await response.json();
        };

        firstYearFirstSemester.value = await fetchCourses(1, 1);
        firstYearSecondSemester.value = await fetchCourses(1, 2);
        secondYearFirstSemester.value = await fetchCourses(2, 1);
        secondYearSecondSemester.value = await fetchCourses(2, 2);
        thirdYearFirstSemester.value = await fetchCourses(3, 1);
        thirdYearSecondSemester.value = await fetchCourses(3, 2);
    } catch (error) {
        console.error('Error fetching courses:', error);
    }
    });
</script>


<template>
  <main class="pagina-inicial">
    <div class="Caixa_branca">
    <h1>Lista de UC's</h1>
      <div class="semester-container">
        <div class="semester">
          <h2>1º ano - 1º semestre</h2>
          <ul>
            <li v-for="course in firstYearFirstSemester" :key="course.id">
              <router-link :to="`/consultar-horario-UC/curso/${course.id}`" style = "color: #000000;">
                {{ course.name }}
              </router-link>
            </li>
          </ul>
        </div>

        

        <div class="semester">
          <h2>2º ano - 1º semestre</h2>
          <ul>
            <li v-for="course in secondYearFirstSemester" :key="course.id">
              <router-link :to="`/consultar-horario-UC/curso/${course.id}`" style = "color: #000000;">{{ course.name }}</router-link>
            </li>
          </ul>
        </div>

        

        <div class="semester">
          <h2>3º ano - 1º semestre</h2>
          <ul>
            <li v-for="course in thirdYearFirstSemester" :key="course.id">
              <router-link :to="`/consultar-horario-UC/curso/${course.id}`" style = "color: #000000;">{{ course.name }}</router-link>
            </li>
          </ul>
        </div>

        <div class="semester">
          <h2>1º ano - 2º semestre</h2>
          <ul>
            <li v-for="course in firstYearSecondSemester" :key="course.id">
              <router-link :to="`/consultar-horario-UC/curso/${course.id}`" style = "color: #000000;">{{ course.name }}</router-link>
            </li>
          </ul>
        </div>

        <div class="semester">
          <h2>2º ano - 2º semestre</h2>
          <ul>
            <li v-for="course in secondYearSecondSemester" :key="course.id">
              <router-link :to="`/consultar-horario-UC/curso/${course.id}`" style = "color: #000000;">{{ course.name }}</router-link>
            </li>
          </ul>
        </div>

        <div class="semester">
          <h2>3º ano - 2º semestre</h2>
          <ul>
            <li v-for="course in thirdYearSecondSemester" :key="course.id">
              <router-link :to="`/consultar-horario-UC/curso/${course.id}`" style = "color: #000000;">{{ course.name }}</router-link>
            </li>
          </ul>
        </div>

      </div>
    </div>
  </main>
</template>

<style scoped>



.pagina-inicial {
  display: flex;
  height: 100vh; 
  flex-direction: column;
  align-items: center;
  background-color: #2c3e50; 
  color: white;
  padding: 20px;
}

h1 {
  font-size: 4.5rem;
  font-weight: bold;
  margin-bottom: 40px;
  color: #333e4f;
  margin-bottom: -1%;
}


.semester-container {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 20px 5%;
  height: 70vh;
  overflow: auto;
}

.semester {
  background-color: #34495e; 
  padding: 20px;
  border-radius: 10px;
  width: 25%; 
  text-align: center;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.2); 
}

.semester h2 {
  color: #00aaff; 
  margin-bottom: 10px;
  font-size: 18px;
}

.semester ul {
  list-style: none;
  padding: 0;
}

.semester li {
  background-color: #ffffff; 
  color: #000000; 
  margin: 5px 0;
  padding: 10px;
  border-radius: 5px;
  font-size: 14px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); 
}

.Caixa_branca {
  display: flex;
  flex-direction: column;
  background-color: #D9D9D9;
  width: 90%;
  height: 85vh;
  align-items: center;
  padding: 20px;
  border-radius: 20px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}
</style>