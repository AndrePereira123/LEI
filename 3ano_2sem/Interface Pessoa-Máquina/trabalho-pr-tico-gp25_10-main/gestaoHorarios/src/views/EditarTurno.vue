<script setup>
import { ref, onMounted } from 'vue';
import { useRoute, useRouter } from 'vue-router';

const route = useRoute();
const router = useRouter();

const turno = ref(null); // Dados do turno
const error = ref(null);

const day = ref('');
const startTime = ref('');
const endTime = ref('');
const classroomId = ref('');

const classrooms = ref([]); 
const edificios = ref([]); 

onMounted(async () => {
  try {
    const shiftId = route.params.idTurno; // ID do turno vindo da rota
    console.log('Received shift ID from route:', shiftId);

    // Fetch turno details
    const turnoResponse = await fetch(`http://localhost:3000/shifts/${shiftId}`);
    if (!turnoResponse.ok) {
      throw new Error('Failed to fetch turno details');
    }
    turno.value = await turnoResponse.json();
    console.log('Fetched turno details:', turno.value);

    day.value = turno.value.day;
    startTime.value = turno.value.from;
    endTime.value = turno.value.to;
    classroomId.value = turno.value.classroomId;

    const classroomsResponse = await fetch(`http://localhost:3000/classrooms`);
    if (!classroomsResponse.ok) {
      throw new Error('Failed to fetch classrooms');
    }
    classrooms.value = await classroomsResponse.json();
    

    const edificiosResponse = await fetch(`http://localhost:3000/buildings`);
    if (!edificiosResponse.ok) {
      throw new Error('Failed to fetch buildings');
    }
    edificios.value = await edificiosResponse.json();
    
  } catch (err) {
    error.value = err.message;
    console.error('Error:', err);
  }
});

function hasTimeConflict(shift1, shift2) {
  if (shift1.day === shift2.day){
    return !(shift1.to <= shift2.from || shift2.to <= shift1.from)
  }
  return false
}

async function updateColisions(id){
    const shiftsResponse = await fetch(`http://localhost:3000/shifts`);
    if (!shiftsResponse.ok) {
      throw new Error('Failed to fetch shifts');
    }
    const shifts = await shiftsResponse.json();

    const allocationsResponse = await fetch(`http://localhost:3000/allocations?studentId=${id}`);
    if (!allocationsResponse.ok) {
      throw new Error('Failed to fetch allocations');
    }    
    const allocations = await allocationsResponse.json();

    const shifts_student = []
    allocations.forEach(allocation => {
      const shift = shifts.find(shift => shift.id == allocation.shiftId);
      console.log("SHIFT : " + shift)
      shifts_student.push(shift);
    })

    const conflicts = [];
    for (let i = 0; i < shifts_student.length; i++) {
        for (let j = i + 1; j < shifts_student.length; j++){
          if (hasTimeConflict(shifts_student[i], shifts_student[j])) {
              conflicts.push({
              shift1Id: shifts_student[i].id,
              shift2Id: shifts_student[j].id,
              day: shifts_student[i].day
          });
        }
        }
    }

    const studentResponse = await fetch(`http://localhost:3000/students/${id}`)
    const student = await studentResponse.json();
    student.conflicts = conflicts
    const updateResponse = await fetch(`http://localhost:3000/students/${id}`, {
    method: 'PUT', 
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(student),
  });

  if (!updateResponse.ok) {
    throw new Error('Failed to fetch student');
  }    
  
}

const submitForm = async () => {
  try {
    const shiftId = route.params.idTurno;

    const response = await fetch(`http://localhost:3000/shifts/${shiftId}`, {
      method: 'PUT',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        ...turno.value,
        day: day.value,
        from: startTime.value,
        to: endTime.value,
        classroomId: classroomId.value,
      }),
    });

    if (!response.ok) {
      throw new Error('Failed to update turno');
    }

    // update colisions
    const alunosResponse = await fetch(`http://localhost:3000/students`);
    if (!response.ok) {
      throw new Error('Failed to fetch students')
    }

    const alunos = await alunosResponse.json()

    await Promise.all(
      alunos.map(async (aluno) => {
        try {
          await updateColisions(aluno.id);
        } catch (err) {
          console.log(err);
        }
      })
    )

    alert('Turno atualizado com sucesso!');
    router.push(`/consultar-horario-UC/curso/${route.params.id}`);
  } catch (err) {
    error.value = err.message;
    console.error('Error:', err);
  }
};

const cancelEdit = () => {
  router.push(`/consultar-horario-UC/curso/${route.params.id}`);
};

const getEdificioName = (buildingId) => {
  const edificio = edificios.value.find((edificio) => edificio.id == buildingId);
  return edificio ? edificio.abbreviation : 'Edifício Desconhecido';
}
</script>



<template>
  <main class="edit-turno">
    <div class="Caixa_branca">
      <h1>Editar Turno</h1>
      <div v-if="error" class="error">
        <p>{{ error }}</p>
      </div>
      <form v-else @submit.prevent="submitForm" class="form-container">
        <div class="form-group">
          <label for="day">Dia da Semana:</label>
          <select id="day" v-model="day">
            <option value="Segunda-feira">Segunda-feira</option>
            <option value="Terça-feira">Terça-feira</option>
            <option value="Quarta-feira">Quarta-feira</option>
            <option value="Quinta-feira">Quinta-feira</option>
            <option value="Sexta-feira">Sexta-feira</option>
          </select>
        </div>
        <div class="form-group-inline">
          <div class="form-group">
            <label for="startTime">Hora de Início:</label>
            <input id="startTime" type="time" v-model="startTime" />
          </div>
          <div class="form-group">
            <label for="endTime">Hora de Fim:</label>
            <input id="endTime" type="time" v-model="endTime" />
          </div>
        </div>
        <div class="form-group">
          <label for="classroom">Sala:</label>
          <select id="classroom" v-model="classroomId">
            <option v-for="classroom in classrooms" :key="classroom.id" :value="classroom.id">
              {{ getEdificioName(classroom.buildingId) }} - {{ classroom.name }} | Cap: {{ classroom.capacity }}
            </option>
          </select>
        </div>
        <div class="form-actions">
          <button type="submit" class="submit-button">Submeter</button>
          <button type="button" class="cancel-button" @click="cancelEdit">Cancelar</button>
        </div>
      </form>
    </div>
  </main>
</template>


<style scoped>
.edit-turno {
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #313E4D;
  height: 100vh;
  padding: 20px;
  color: #333e4f;
}


h1 {
  font-size: 7vh;
  color: #333E4F;
  margin-bottom: 20px;
}

.form-container {
  display: flex;
  flex-direction: column;
  width: 80%;
  height: 80%;
  align-items: center;
  background-color: #333E4F;
  color: white;
  border-radius: 20px;
  padding: 20px;
}

.form-group {
  display: flex;
  flex-direction: column;
  align-items: flex-start;
  width: 100%;
  margin-bottom: 20px;
}

.form-group-inline {
  display: flex;
  justify-content: space-between;
  gap: 20px;
  width: 100%;
}

label {
  font-size: 1rem;
  margin-bottom: 5px;
  color: white; /* Alterado para branco */
}

input,
select {
  width: 100%;
  padding: 10px;
  border: 1px solid #ccc;
  border-radius: 5px;
  font-size: 1rem;
}

.form-actions {
  display: flex;
  margin-top: auto;
  justify-content: center;
  gap: 20vh;
  width: 100%;
  margin-bottom: 4vh;
}

.submit-button {
  background-color: #1F996E;
  color: white;
  border: none;
  padding: 2vh 10vh;
  border-radius: 35px;
  cursor: pointer;
  font-size: 1rem;
}

.submit-button:hover {
  background-color: #17a974;
}

.cancel-button {
  background-color: #E74C3C;
  color: white;
  border: none;
  padding: 2vh 10vh;
  border-radius: 35px;
  cursor: pointer;
  font-size: 1rem;
}

.cancel-button:hover {
  background-color: #c0392b;
}
</style>