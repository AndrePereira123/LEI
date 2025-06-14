<script setup>
import { ref, onMounted } from 'vue';
import { useRoute, useRouter } from 'vue-router';

const route = useRoute();
const router = useRouter();

const request = ref(null);
const error = ref(null);
const course = ref(null);
const shift1 = ref(null);
const shift2 = ref(null);
const classroom = ref(null);
const teacher = ref(null);
const building = ref(null);
const student = ref(null);

onMounted(async () => {
  try {
    const { type, id } = route.params;

    // Buscar o pedido com base no tipo e no ID
    const response = await fetch(`http://localhost:3000/${type}Requests/${id}`);
    if (!response.ok) {
      throw new Error('Erro ao buscar o pedido');
    }
    request.value = await response.json();

    const courseResponse = await fetch(`http://localhost:3000/courses/${request.value.courseId}`);
    if (!courseResponse.ok) {
      throw new Error('Erro ao buscar a UC');
    }
    course.value = await courseResponse.json();

    if (request.value.type === 'shift') {
      const shift1Response = await fetch(`http://localhost:3000/shifts/${request.value.current_shiftId}`);
      if (!shift1Response.ok) {
        throw new Error('Erro ao buscar o turno atual');
      }
      shift1.value = await shift1Response.json();

      const shift2Response = await fetch(`http://localhost:3000/shifts/${request.value.requested_shiftId}`);
      if (!shift2Response.ok) {
        throw new Error('Erro ao buscar o turno pretendido');
      }
      shift2.value = await shift2Response.json();

      const studentResponse = await fetch(`http://localhost:3000/students/${request.value.studentId}`);
      if (!studentResponse.ok) {
        throw new Error('Erro ao buscar o estudante');
      }
      student.value = await studentResponse.json();
    }
    else if (request.value.type === 'classroom') {
      const shift1Response = await fetch(`http://localhost:3000/shifts/${request.value.shiftId}`);
      if (!shift1Response.ok) {
        throw new Error('Erro ao buscar o turno atual');
      }
      shift1.value = await shift1Response.json();

      const classroomResponse = await fetch(`http://localhost:3000/classrooms/${request.value.classroomId}`);
      if (!classroomResponse.ok) {
        throw new Error('Erro ao buscar a sala');
      }
      classroom.value = await classroomResponse.json();

      const buildingResponse = await fetch(`http://localhost:3000/buildings/${classroom.value.buildingId}`);
      if (!buildingResponse.ok) {
        throw new Error('Erro ao buscar o edifício');
      }
      building.value = await buildingResponse.json();

      const teacherResponse = await fetch(`http://localhost:3000/teachers/${request.value.teacherId}`);
      if (!teacherResponse.ok) {
        throw new Error('Erro ao buscar o professor');
      }
      teacher.value = await teacherResponse.json();

      console.log('Professor:', teacher.value);
    }

    console.log('Pedido:', request.value);
  } catch (err) {
    error.value = err.message;
    console.error('Erro:', err);
  }
});


const goToResponderPedido = () => {
  const { type, id } = route.params;
  router.push(`/atendimento-pedidos/${type}/resposta/${id}`);
};
</script>

<template>
  <main class="view-request">
    <div class="Caixa_branca" style="font-size: 2vh;font-weight: bold;">
      <h1 v-if="request && request.response !== null" style="color: #0e8b29;">Pedido</h1>
      <h1 v-else-if="request" style="color: #8B0000;">Pedido</h1>
      <div v-if="error" class="error">
        <p>{{ error }}</p>
      </div>
      <div v-else-if="request" class="request-details">
        <div class="request-info">
          <p>
            Pedido realizado no dia
            <strong>{{ request.date }}</strong>, {{ request.weekday }} às
            <strong>{{ request.hour }}</strong>.
          </p>
          <p>
            <strong style="color: #5EF530;font-weight: 700;">UC:</strong> {{ course.name }}
          </p>
          <template v-if="request.type === 'shift'">
            <p>
              <strong style="color: #5EF530;font-weight: 700;">Turno Atual:</strong> {{ shift1.name }}
            </p>
            <p>
              <strong style="color: #5EF530;font-weight: 700;">Turno Pretendido:</strong> {{ shift2.name }}
            </p>
          </template>
          <template v-else-if="request.type === 'classroom'">
            <p>
              <strong style="color: #5EF530;font-weight: 700;">Turno :</strong> {{ shift1.name }}
            </p>
            <p>
              <strong style="color: #5EF530;font-weight: 700;">Sala :</strong> {{ building.name }} - {{ classroom.name }}
            </p>
          </template>
        </div>
        <div v-if="request.response === null" class="request-message">
          <p>
            <strong style="color: #5EF530;font-weight: 700;">Mensagem:</strong>
          </p>
          <p>
            {{ request.message }}
          </p>
        </div>
        <div v-else class="request-message" style="height: 15vh;">
          <p>
            <strong style="color: #5EF530;font-weight: 700;">Mensagem do Pedido:</strong>
            {{ request.message }}
          </p>
        </div>
        <div v-if="request.response !== null" class="response-message">
          <p>
            <strong style="color: yellow;font-size: large;font-weight: 700;">Resposta:</strong>
            {{ request.response }}
          </p>
        </div>
        <button v-if="request.response === null" class="responder-button" @click="goToResponderPedido">
          Responder
        </button>
        <div v-if="request.type === 'shift'" class="request-footer">
          <a :href="`/gestao-alunos/listar-alunos-curso/${student.id}`" style="color: white;">{{ student.name }} - A{{ student.id }}</a>
        </div>
        <div v-else="request.type === 'classroom'" class="request-footer">
          <p>{{ teacher.name }} - {{ teacher.id }}</p>
        </div>
      </div>
      <div v-else class="loading">
        <p>Carregando pedido...</p>
      </div>
    </div>
  </main>
</template>

<style scoped>
.view-request {
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #313e4d;
  height: 100vh;
  padding: 20px;
  overflow: auto;
}

h1 {
  font-size: 3.5rem;
  color: #2c3e50;
  margin-bottom: 0;
}

.request-details {
  display: flex;
  flex-direction: column;
  width: 90%;
  height: calc(100% - 6rem);
}

.request-info {
  background-color: #333E4F;
  color: white;
  padding: 15px;
  border-radius: 10px;
  margin-bottom: 20px;
  width: 100%;
}

.request-message {
  flex: 1;
  min-height: 15vh;
  overflow: auto;
  background-color: #333E4F;
  color: white;
  padding: 15px;
  border-radius: 10px;
  margin-bottom: 20px;
}

.response-message {
  background-color: #333E4F;
  color: white;
  padding: 15px;
  border-radius: 10px;
  margin-bottom: 20px;
  height: 20vh;
}

.responder-button {
  background-color: #6454F2;
  color: white;
  border: none;
  border-radius: 25px;
  padding: 10px 20px;
  font-size: 3.5vh;
  cursor: pointer;
  margin-bottom: 20px;
  height: 8vh;
  width: 30vh;
  align-self: center;
}

.request-footer {
  margin-top: auto;
  background-color: #333E4F;
  color: white;
  padding: 10px;
  border-radius: 20px;
  text-align: left;
  font-size: 2vh;
}
</style>