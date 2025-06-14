<script setup>
import { ref, onMounted, watch } from 'vue';
import { useRouter } from 'vue-router';
import { useAuthStore } from '../stores/auth'
import horarioComponent from '../components/Horario.vue'


const authStore = useAuthStore()

const studentId = authStore.id

const router = useRouter();
const error = ref(null);

const student = ref(null);
const selectedUC = ref('');
const idUC = ref(null);
const ucs = ref([]);

const currentShiftId = ref(null);
const selectedTurno = ref('');
const idTurno = ref(null);
const shifts = ref([]); 

const responseMessage = ref(''); // Mensagem de resposta do aluno

onMounted(async () => {
  const previousRoute = sessionStorage.getItem('previousRoute') || '';
  console.log('Previous route:', previousRoute);
  const prefix = '/aluno/pedidos/novopedido';
  

  fetchStudentAndUCs();
});

const fetchStudentAndUCs = async () => {  
  try {
    const studentsResponse = await fetch(`http://localhost:3000/students?id=${authStore.id}`);
    if (!studentsResponse.ok) {
      throw new Error('Falha ao buscar detalhes do aluno.');
    }
    const students = await studentsResponse.json();
    if (students.length === 0) {
      throw new Error('Aluno não encontrado.');
    }
    student.value = students[0]; 

    const enrolledCourseIds = student.value.enrolled || [];
    if (enrolledCourseIds.length === 0) {
      throw new Error('O aluno não está inscrito em nenhuma UC.');
    }

    const ucsPromises = enrolledCourseIds.map((id) =>
      fetch(`http://localhost:3000/courses/${id}`).then((response) => {
        if (!response.ok) {
          throw new Error(`Falha ao buscar UC com ID ${id}`);
        }
        return response.json();
      })
    );

    ucs.value = await Promise.all(ucsPromises);
    console.log('Fetched UCs:', ucs.value);
  } catch (err) {
    error.value = err.message;
    console.error('Error:', err);
  }
};

watch(selectedUC, async (newUC) => {  // Quando a UC é selecionada, busca-se os turnos associados
  if (newUC) {
    idUC.value = newUC.id;
    await fetchShiftsForUC(newUC.id);
    
  } else {
    shifts.value = [];
    selectedTurno.value = '';
  }

  console.log('Selected UC:', newUC);
});

watch(selectedTurno, (newShift) => { // Quando o turno é selecionado, guarda-se o ID do turno
  if (newShift) {
    idTurno.value = newShift.id;
    
  } else {
    shifts.value = [];
    selectedTurno.value = '';
  }

  console.log('Selected Shift:', newShift);
});


const fetchShiftsForUC = async (courseId) => { // funcao que busca os turnos associados a uma UC com atencao ao aluno selecionado
  try {
    
    const shiftsResponse = await fetch(`http://localhost:3000/shifts?courseId=${courseId}`); //todos os turnos da UC
    if (!shiftsResponse.ok) {
      throw new Error('Failed to fetch shifts');
    }
    const fetchedShifts = await shiftsResponse.json();

    
    const allocationsResponse = await fetch(`http://localhost:3000/allocations?studentId=${authStore.id}`); // todos os turnos do aluno
    if (!allocationsResponse.ok) {
      throw new Error('Failed to fetch student allocations');
    }
    const studentAllocations = await allocationsResponse.json();
    

    const shiftIds = fetchedShifts.map(shift => Number(shift.id)); 
    const studentShiftIds = studentAllocations.map(allocation => Number(allocation.shiftId)); // IDs dos turnos em que o aluno está alocado
    
    
    const studentShiftsForCourse = fetchedShifts.filter(shift => 
      !studentShiftIds.includes(Number(shift.id)) && 
      Number(shift.courseId) === Number(courseId)
    );
  
    currentShiftId.value = studentShiftIds.find(studentShiftId => 
      shiftIds.includes(studentShiftId)
    );
    

    if (studentShiftsForCourse.length === 0) {
      console.log('Student is not enrolled in any shifts for this course');
    }

    const shiftsWithDetails = await Promise.all(                                                  //detalhes do turno 
      studentShiftsForCourse.map(async (shift) => {
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

        return { 
          ...shift, 
          classroom: { ...classroom, building },
          displayText: `${shift.name} | ${shift.day} das ${shift.from}h às ${shift.to}h | Sala: ${building.abbreviation} ${classroom.name}`
        };
      })
    );

    shifts.value = shiftsWithDetails;  //valor é guardado no shifts para ser mostrado na seleção do turno atual
    

    console.log('Fetched shifts with classrooms and buildings:', shifts.value);
  } catch (err) {
    error.value = err.message;
    console.error('Error fetching shifts:', err);
  } 
};



const submitRequest = async () => {
  try {

    const requestResponse = await fetch('http://localhost:3000/shiftRequests');
    if (!requestResponse.ok) {
      throw new Error('Erro ao buscar pedidos de turno');
    }
    const requests = await requestResponse.json();
    const newId = requests.length + 1; 

    const newRequest = {
      id: String(newId),
      type: "shift",
      date: new Date().toISOString().split('T')[0],
      weekday: new Date().toLocaleDateString('pt-PT', { weekday: 'long' }).charAt(0).toUpperCase() + new Date().toLocaleDateString('pt-PT', { weekday: 'long' }).slice(1),
      hour: new Date().toLocaleTimeString('pt-PT', { hour: '2-digit', minute: '2-digit' }),
      studentId: Number(authStore.id),
      courseId: Number(idUC.value),
      current_shiftId: Number(currentShiftId.value),
      requested_shiftId: Number(idTurno.value),
      message: responseMessage.value,
      status: false,
      response: null
    }    

    // Atualizar o campo de resposta no JSON
    const response = await fetch('http://localhost:3000/shiftRequests', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(newRequest),
    });

    if (!response.ok) {
      throw new Error('Erro ao enviar o pedido');
    }

    alert('Pedido enviado com sucesso!');
    router.push('/aluno/pedidos'); 
  } catch (err) {
    console.error('Erro ao enviar a resposta:', err);
    alert('Erro ao enviar a resposta.');
  }
}
</script>

<template>
  <main class="student-management">
    <div class="Caixa_branca">
      <div class="form-section">
        <h1 style="text-align: center;">Novo Pedido</h1>
        <div class="form-group">
          <div style="height: 3vh;"></div>
            <label for="uc" style="color:black;">UC:</label>
            <div class="input-group">
              <select 
                v-model="selectedUC" 
                class="uc-dropdown"
                :disabled="ucs.length === 0"
              >
                Selecione a UC
                <option 
                  v-for="uc in ucs" 
                  :key="uc.id" 
                  :value="uc"
                >
                  {{ uc.name }} 
                </option>
              </select>
            </div>

            <div style="height: 3vh;"></div>
            <label for="turno" style="color:black;">Turno:</label>
            <div class="input-group">
              <select 
                v-model="selectedTurno" 
                class="shift-dropdown"
                :disabled="shifts.length === 0" 
              >
                <option value="" disabled>{{ !selectedUC ? 'UC por selecionar' : shifts.length === 0 ? "Não há outros turnos disponíveis" : "Selecione um Turno"}}</option>
                <option 
                  v-for="shift in shifts" 
                  :key="shift.id" 
                  :value="shift"
                >
                  {{ shift.name }} | {{ shift.day }} das {{ shift.from }}h às {{ shift.to }}h 
                </option>
              </select>
            </div>

          <div style="height: 3vh;"></div>
          <div v-if="selectedUC !== '' && selectedTurno !== ''" class="response-field">
          <label for="response">Escreva sua mensagem:</label>
          <textarea
            id="response"
            v-model="responseMessage"
            placeholder="Digite sua mensagem aqui..."
          ></textarea>
        </div>
          
          
        </div>

        <button class="btn enviar" :disabled=" !selectedTurno" @click="submitRequest">
          Enviar
        </button>
        <p v-if=" !selectedTurno" class="warning">
          Por favor, preencha todos os campos.
        </p>
      </div>

      <horarioComponent 
          v-if="studentId !== ''" 
          :studentId="studentId" 
          :title="'Horário'"
          :path_horario_publicado="`publishedHorarios/${studentId.value}/`"
        />
    </div>
  </main>
</template>

<style scoped>

.Caixa_branca {
  display: flex;
  flex-direction: row; 
  justify-content: space-between; 
  background-color: #D9D9D9;
  width: 90%;
  height: 80vh;
  align-items: flex-start;
  padding: 20px;
  border-radius: 20px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  overflow: auto;
}

.horario-box {
  height: 70vh;
}

h1 {
  font-size: 3.5rem;
  font-weight: bold;
  margin-bottom: 40px;
}

.options {
  display: flex;
  flex-direction: column;
  gap: 20px;
  width: 90%;
}

.uc-dropdown, .shift-dropdown {
  width: 400px; 
  height: 40px; 
  height: 5vh;
  background-color: white;
  border: 1px solid #ccc;
  border-radius: 5px;
  padding-left: 10px;
  font-size: 1rem;
  color: #333e4f;
}

.option {
  display: flex;
  height: 9vh;
  align-items: center;
  justify-content: center;
  padding: 20px;
  border-radius: 20px;
  font-size: 1.4rem;
  font-weight: 500;
  color: white;
  text-align: center;
}


.form-group {
  display: flex;
  flex-direction: column;
  width: 90%;
}

.input-group {
  display: flex;
  flex-direction: row;
  align-items: flex-start;
  gap: 10px;
  width: 100%;
}

.input-row {
  display: flex;
  align-items: center;
  gap: 10px;
  width: 100%;
}

label {
  font-size: 1.2rem;
  font-weight: bold;
  color: #333e4f;
}

.static-rectangle {
  flex: 1; 
  height: 10%; 
  min-height: 40px;
  min-width: 300px;
  background-color: white; 
  border: 1px solid #ccc; 
  border-radius: 5px; 
  display: flex;
  align-items: center; 
  padding-left: 10px; 
  font-size: 1rem;
  color: black; 
}

.static-rectangle.cor {
  background-color: #333E4F; 
  color: white;
  font-weight: 700;
}

.response-field {
  margin-bottom: 20px;
}

.response-field label {
  display: block;
  font-size: 1rem;
  color: #2c3e50;
  margin-bottom: 5px;
}

.response-field textarea {
  width: 100%;
  height: 100px;
  padding: 10px;
  border-radius: 5px;
  border: 1px solid #ccc;
  font-size: 1rem;
}

select {
  flex: 1;
  padding: 10px;
  border: 1px solid #ccc;
  border-radius: 5px;
  font-size: 1rem;
}

.btn {
  padding: 10px 20px;
  align-self: flex-end;
  border: none;
  border-radius: 35px;
  font-size: 1rem;
  cursor: pointer;
  background-color: #1F996E;
  color: white;
}





.btn.enviar {
  margin-top: auto;
  align-self: center;
  background-color: #1F996E;
  font-size: 1.5rem;
  font-weight: 700;
  width: 40%;
  border-radius: 30px;
  height: 7vh;
}

.btn:disabled {
  background-color: #C83939;
  cursor: not-allowed;
}

.warning {
  text-align: center;
  margin-top: 10px;
  margin-bottom: 1%;
  color: black;
  font-weight: 700;
  font-size: 1.2rem;
}
</style>