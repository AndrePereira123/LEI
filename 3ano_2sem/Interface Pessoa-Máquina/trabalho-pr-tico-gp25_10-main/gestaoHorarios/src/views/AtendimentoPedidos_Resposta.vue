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
const student = ref(null);
const classroom = ref(null);
const teacher = ref(null);
const building = ref(null);
const responseMessage = ref(''); 
const warning = ref(false);

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


const enviarResposta = async () => {
  try {
    const { type, id } = route.params;

    const response = await fetch(`http://localhost:3000/${type}Requests/${id}`, {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ 
        response: responseMessage.value, 
        status: true
      }),
    });

    if (!response.ok) {
      throw new Error('Erro ao enviar a resposta');
    }

    alert('Resposta enviada com sucesso!');
    router.push('/atendimento-pedidos'); 
  } catch (err) {
    console.error('Erro ao enviar a resposta:', err);
    alert('Erro ao enviar a resposta.');
  }
};


const checkclick = () => {
  if (responseMessage.value === '') {
    warning.value = true;
  }
};
</script>

<template>
  <main class="view-request">
    <div class="Caixa_branca">
      <h1>Responder Pedido</h1>
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
            <strong style="color: #5EF530;font-weight: 700;"> UC :</strong> {{ course.name }}
          </p>
          <template v-if="request.type === 'shift'">
            <p>
              <strong style="color: #5EF530;font-weight: 700;"> Turno Atual :</strong> {{ shift1.name }}
            </p>
            <p>
              <strong style="color: #5EF530;font-weight: 700;"> Turno Pretendido :</strong> {{ shift2.name }}
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
              
            <div class="request-message">
              <p>
                <strong style="color: #5EF530;font-weight: 700;">Mensagem do Pedido:</strong>
                {{ request.message }}
              </p>
            </div>
        </div>
        <label for="response" style="color: black;">Mensagem:</label>
        <div class="response-field">
          <textarea
            id="response"
            v-model="responseMessage"
            placeholder="Digite sua resposta aqui..."
            style="resize: none;"
          ></textarea>
        </div>
        
          <button 
          class="enviar-button" 
          :class="{ 'active': responseMessage.trim().length > 0 }"
          @click="responseMessage ? enviarResposta() : (warning = true)"
          >
          Enviar 
        </button>
        <p v-if="warning" style="color: black;align-self: center;font-weight: 700;font-size: 2vh;">Preencha o campo de resposta!</p>
        
        <div v-if="request.type === 'shift'" class="request-footer">
          <p>{{ student.name }} - A{{ student.id }}</p>
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

.Caixa-branca {
  max-height: 90vh;
  overflow-y: auto;
}

.view-request {
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #313e4d;
  height: 100vh;
  padding: 20px;
  overflow-y: auto;
}

h1 {
  font-size: 3rem;
  color: #2c3e50;
  margin-bottom: 0;
}
.request-details {
  display: flex;
  flex-direction: column;
  width: 90%;
  height: 100%;
  overflow: auto;
}

.request-info {
  display: block;
  flex-direction: row;
  background-color: #333E4F;
  color: white;
  padding: 15px;
  border-radius: 10px;
  margin-bottom: 20px;
  width: 100%;
}

.request-message  {
  background-color: #333E4F;
  color: white;
  padding: 15px;
  border-radius: 10px;
  margin-bottom: 20px;
  max-height: 15vh;
}

.response-message  {
  background-color: #3498db;
  color: white;
  padding: 15px;
  border-radius: 10px;
  margin-bottom: 20px;
}

.response-field {
  background-color: #333E4F;
  color: white;
  padding: 15px;
  border-radius: 10px;
  margin-bottom: 20px;
  min-height: 15vh;
}

.response-field textarea {
  width: 100%;
  height: 100%;
  border: none;
  background-color: #2c3e50;
  color: white;
  font-size: 1.5vh;
}

.enviar-button {
  background-color: #C83939;
  color: white;
  border: none;
  border-radius: 25px;
  padding: 10px 20px;
  font-size: 3.5vh;
  height: 8vh;
  width: 30vh;
  align-self: center;
}

.enviar-button.active {
  background-color: #1F996E;
  cursor: pointer;
}

.request-footer {
  background-color: #333E4F;
  margin-top: auto;
  color: white;
  padding: 10px;
  border-radius: 20px;
  text-align: left;
  font-size: 2vh;
}
</style>