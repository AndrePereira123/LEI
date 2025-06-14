<script setup>
import { ref, onMounted, watch, computed } from 'vue';
import { useRouter } from 'vue-router';
import horarioComponent from '../components/Horario.vue'
import { useAlunoRemoverStore } from '../stores/aluno_remover'

const alunoRemoverStore = useAlunoRemoverStore();

const router = useRouter();
const selectedAluno = ref(alunoRemoverStore.Aluno_Para_Remover || ''); 
const idSelectedAluno = ref(alunoRemoverStore.Id_Aluno_Para_Remover || '');
const student = ref(null); 

// UC
const selectedUC = ref('');
const idUC = ref(null);
const ucs = ref([]);

// Turno
const selectedTurno = ref('');
const shifts = ref([]); 

const error = ref(null);

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

onMounted(async () => {
  const previousRoute = sessionStorage.getItem('previousRoute') || '';
  console.log('Previous route:', previousRoute); 
  const allowedPrefix = '/gestao-alunos/remover-alocacao';
  
  const hasNoQueryParams = Object.keys(router.currentRoute.value.query).length === 0;

  
    if ( hasNoQueryParams && !previousRoute.includes(allowedPrefix)) {
    selectedAluno.value = '';
    idSelectedAluno.value = '';
    alunoRemoverStore.clear();
  }
    

  if (idSelectedAluno.value) {
    await fetchStudentAndUCs();
  }
});

const fetchStudentAndUCs = async () => {  // Busca lista de UCs do aluno
  try {
    const studentsResponse = await fetch(`http://localhost:3000/students?id=${idSelectedAluno.value}`);
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
    
    // Reset turno e novo turno ao mudar de UC
    selectedTurno.value = '';
  } else {
    shifts.value = [];
    selectedTurno.value = '';
  }
});

const fetchShiftsForUC = async (courseId) => { // funcao que busca os turnos associados a uma UC com atencao ao aluno selecionado
  try {
    
    
    const shiftsResponse = await fetch(`http://localhost:3000/shifts?courseId=${courseId}`); //todos os turnos da UC
    if (!shiftsResponse.ok) {
      throw new Error('Failed to fetch shifts');
    }
    const fetchedShifts = await shiftsResponse.json();

    
    const allocationsResponse = await fetch(`http://localhost:3000/allocations?studentId=${idSelectedAluno.value}`); // todos os turnos do aluno
    if (!allocationsResponse.ok) {
      throw new Error('Failed to fetch student allocations');
    }
    const studentAllocations = await allocationsResponse.json();
    
    
    const studentShiftIds = studentAllocations.map(allocation => Number(allocation.shiftId)); // IDs dos turnos em que o aluno está alocado
    
    
    const studentShiftsForCourse = fetchedShifts.filter(shift =>  // filtra os turnos da UC em que o aluno nao está alocado
      studentShiftIds.includes(Number(shift.id)) && 
      Number(shift.courseId) === Number(courseId)
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
    

    const allShiftsWithDetails = await Promise.all( //detalhes de todos os turnos da UC
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

        return { 
          ...shift, 
          classroom: { ...classroom, building },
          displayText: `${shift.name} | ${shift.day} das ${shift.from}h às ${shift.to}h | Sala: ${building.abbreviation} ${classroom.name}`
        };
      })
    );
    
    novosTurnos_pre_compatibilidade.value = allShiftsWithDetails; 
    // guardamos os detalhes de todos os turnos da UC para depois filtrar os turnos compatíveis com o "turno atual" selecionado 
    
    console.log('Fetched shifts with classrooms and buildings:', shifts.value);
  } catch (err) {
    error.value = err.message;
    console.error('Error fetching shifts:', err);
  } 
};

const updateAllocation = async () => {
  try { 

    const allocationResponse = await fetch(`http://localhost:3000/allocations?studentId=${idSelectedAluno.value}&shiftId=${selectedTurno.value.id}`);
    if (!allocationResponse.ok) {
      throw new Error('Failed to fetch allocations');
    }
    const allocations = await allocationResponse.json();
    
    if (!allocations.length) {
      throw new Error('No allocation found for this student and shift');
    }
    
    const allocationId = allocations[0].id; 
    
    const response = await fetch(`http://localhost:3000/allocations/${allocationId}`, {
      method: 'DELETE',
      headers: {
        'Content-Type': 'application/json',
      }
    });

    if (!response.ok) {
      throw new Error('Failed to update allocation');
    }

    
    const shiftId = selectedTurno.value.id;
    const shiftResponse = await fetch(`http://localhost:3000/shifts/${shiftId}`);
    if (!shiftResponse.ok) {
      throw new Error('Failed to fetch shift');
    }
    const shift = await shiftResponse.json();
    shift.totalStudentsRegistered -= 1;
    
    const updateShiftResponse = await fetch(`http://localhost:3000/shifts/${shiftId}`, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(shift),
    });

    if (!updateShiftResponse.ok) {
      throw new Error('Failed to update new shift');
    }
    
    await updateColisions(idSelectedAluno.value);

    alert('Alocação atualizada com sucesso!');
    window.location.href = `/gestao-alunos/remover-alocacao?reload=true&studentId=${idSelectedAluno.value}}`;
  }
  catch (err) { 
    console.error('Error updating allocation:', err);
    alert('Erro ao atualizar alocação: ' + err.message);
  }
}

</script>

<template>
  <main class="student-management">
    <div class="Caixa_branca">
      <div class="form-section">
        <h1>Remover Alocação de Turno</h1>
        <div class="form-group">
          <label for="aluno" style="color:black;">Aluno:</label>
          <div class="input-group">
            <div class="static-rectangle">{{ selectedAluno }}</div>
            <button
              class="btn blue"
              @click="router.push('/gestao-alunos/remover-alocacao/lista-alunos');"
            >
              Selecionar Aluno
            </button>
          </div>

          <div style="height: 5vh;"></div>
          <label for="uc" style="color:black;">UC:</label>
          <div class="input-group">
            <select 
              v-model="selectedUC" 
              class="uc-dropdown"
              :disabled="ucs.length === 0"
            >
              <option value="" disabled>{{!selectedAluno ? 'Aluno por selecionar' : 'Selecione a UC' }}</option>
              <option 
                v-for="uc in ucs" 
                :key="uc.id" 
                :value="uc"
              >
                {{ uc.name }} 
              </option>
            </select>
          </div>
          
          <div style="height: 5vh;"></div>
          <label for="turno" style="color:black;">Turno a remover:</label>
          <div class="input-group">
            <select 
              v-model="selectedTurno" 
              class="shift-dropdown"
              :disabled="shifts.length === 0"
            >
              <option value="" disabled>{{!selectedUC ? 'UC por selecionar' : 'Selecione um turno' }}</option>
              <option 
                v-for="shift in shifts" 
                :key="shift.id" 
                :value="shift"
              >
                {{ shift.name }} | {{ shift.day }} das {{ shift.from }}h às {{ shift.to }}h | Sala: {{ shift.classroom.building.abbreviation }} {{ shift.classroom.name }}
              </option>
            </select>
          </div>

          <div style="height: 5vh;"></div>
          
          
          <div class="action-container">
            <button class="btn remover" :disabled="!selectedAluno || !selectedTurno" @click="updateAllocation">
              Remover Alocação
            </button>
            <p v-if="!selectedAluno || !selectedTurno" class="warning">
              Por favor, preencha todos os campos.
            </p>
          </div>
        </div>  
      </div>

      <div class="horario-component" :hidden="!selectedAluno">
        <horarioComponent v-if="idSelectedAluno !== ''" :studentId="idSelectedAluno" :title="'Horário'" />
      </div>

    </div>
  </main>
</template>

<style scoped>
.horario-component {
  height: 74vh;
  width: 85vh;
}

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
}

h1 {
  font-size: 2.5rem;
  font-weight: bold;
  margin-bottom: 40px;
}

.options {
  display: flex;
  flex-direction: column;
  gap: 20px;
  width: 90%;
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
  height: 100%;
  max-height: 78vh;
  overflow-y: auto;
  flex: 1;
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
  height: 40px;
  background-color: white; 
  border: 1px solid #ccc; 
  border-radius: 5px; 
  display: flex;
  align-items: center; 
  padding-left: 10px; 
  font-size: 1rem;
  color: #333e4f; 
}

select {
  flex: 1;
  padding: 10px;
  border: 1px solid #ccc;
  border-radius: 5px;
  font-size: 1rem;
}

select option:first-child {
  color: #757575;
  font-style: italic;
}

.uc-dropdown, .shift-dropdown {
  flex: 1;
  height: 40px;
  background-color: white;
  border: 1px solid #ccc;
  border-radius: 5px;
  padding-left: 10px;
  font-size: 1rem;
  color: #333e4f;
}

.btn {
  padding: 10px 20px;
  border: none;
  border-radius: 5px;
  font-size: 1rem;
  cursor: pointer;
  color: white;
}

.btn.blue {
  background-color: #5a5ce6; 
  color: white;
  margin-left: 1%;
  padding: 1vh 0px;
  border: none;
  border-radius: 30px;
  font-size: 1rem;
  cursor: pointer;
  white-space: nowrap; 
  align-self: center;
  width: 18vh;
}

.btn.remover {
  margin-top: auto;
  font-size: 1rem;
  background-color: #1F996E;
  height: 6vh;
  width: 20vh;
  text-align: center;
  align-self: center;
  margin-top: 2vh;
  border-radius: 30px;
  white-space: nowrap;
  display: flex;
  justify-content: center;
  align-items: center;
  text-overflow: ellipsis;
}

.btn:disabled {
  background-color: #C83939;
  cursor: not-allowed;
}

.warning {
  margin-top: 10px;
  margin-bottom: 1%;
  color: black;
  font-weight: 700;
  align-self: center;
  font-size: 1.2rem;
}

.schedule-grid {
  display: grid;
  grid-template-columns: 50px repeat(5, 1fr);
  gap: 0.7%;
  margin-top: 20px;
  height: 100%;
  align-items: stretch;
}

.action-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  margin-top: auto;
  margin-bottom: 10px;
}

.form-section {
  display: flex;
  flex-direction: column;
  height: 100%;
}
</style>