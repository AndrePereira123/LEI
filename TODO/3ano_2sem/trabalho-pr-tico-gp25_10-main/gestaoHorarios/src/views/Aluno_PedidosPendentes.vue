<script setup>
import { ref, onMounted, computed } from 'vue';
import { useRouter } from 'vue-router';
import { useAuthStore } from '../stores/auth.js';

const authStore = useAuthStore();
const router = useRouter();

const allRequests = ref([]);
const error = ref(null);
const currentPage = ref(1);
const itemsPerPage = ref(3);

const totalPages = computed(() => {
    if (allRequests.value.length === 0) {
        return 1;
    }
    else return Math.ceil(allRequests.value.length / itemsPerPage.value);
});

const paginatedPedidos = computed(() => {
  const start = (currentPage.value - 1) * itemsPerPage.value;
  const end = start + itemsPerPage.value;
  return allRequests.value.slice(start, end);
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
    const shiftResponse = await fetch(`http://localhost:3000/shiftRequests?studentId=${authStore.id}`);

    if (!shiftResponse.ok) {
      throw new Error('Erro ao buscar os pedidos');
    }

    allRequests.value = await shiftResponse.json();

    allRequests.value.sort((a, b) => {
      
      const dateTimeA = new Date(`${a.date} ${a.hour}`);
      const dateTimeB = new Date(`${b.date} ${b.hour}`);
      
      
      return dateTimeB - dateTimeA;
    });
    
    allRequests.value = allRequests.value.filter(req => req.status === false)

  } catch (err) {
    error.value = err.message;
    console.error('Erro:', err);
  }
});

const navigateToPedidosAluno = () => {
  router.push('/aluno/pedidos');
};


const goToRequestPage = (type, id) => {
    if (type === 'new') {
        router.push('/aluno/pedidos/novopedido');
        return;
    }
    router.push(`/aluno/pedidos/${type}/${id}`);
};


</script>

<template>
  <main class="shift-requests">
    <div class="Caixa_branca">
        <div style="display: flex; flex-direction: row;width: 100%;align-items: center;justify-content: space-evenly;">
            <h1>Pedidos Pendentes</h1>
            <button class="ver-recebidas" @click="navigateToPedidosAluno">Ver concluídos</button>
        </div>
      <div v-if="error" class="error">
        <p>{{ error }}</p>
      </div>
      <div v-else-if="allRequests.length > 0" class="requests-list">
        <div
          v-for="request in paginatedPedidos"
          :key="request.id"
          class="request-item"
          @click="goToRequestPage(request.type, request.id)"
        >
          <p>
            <span v-if="request.type === 'shift'">Pedido Troca de Turno</span>
            <span v-else-if="request.type === 'classroom'">Pedido Troca de Sala</span>
            - {{ request.date }} |
            <span v-if="request.type === 'shift'">
              A{{ request.studentId }} - {{ request.message }}
            </span>
            <span v-else-if="request.type === 'classroom'">
              Professor {{ request.teacherId }} - {{ request.message }}
            </span>
          </p>
        </div>
      </div>
      <div v-else class="no-requests">
        <p>Não há Mensagens.</p>
        </div>
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
      <button class="new-request" @click="goToRequestPage('new')">Enviar Novo Pedido</button>
    </div>
  </main>
</template>

<style scoped>
.shift-requests {
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #313E4D;
  height: 100vh;
  padding: 20px;
  color: #333e4f;
}

.Caixa_branca {
  display: flex;
  flex-direction: column;
  background-color: #D9D9D9;
  width: 90%;
  height: 80vh;
  align-items: center;
  padding: 20px;
  border-radius: 20px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}


h1 {
  font-size: 5em;
  color: #2c3e50;
  font-weight: bold;
}

.ver-recebidas {
  background-color: #6454F2;
  color: white;
  border: none;
  cursor: pointer;
  height: 50%;
  width: 20%;
  font-size: 2em;
  font-weight: bold;
  border-radius: 30px;
}

.requests-list {
  margin-top: 2%;
  display: flex;
  flex-direction: column;
  gap: 10px;
  width: 93%;
}

.request-item {
  background-color: #34495e;
  color: white;
  padding: 1%;
  padding-left: 3%;
  border-radius: 30px;
  font-size: 1.5em;
  width: 100%;
  white-space: nowrap; 
  overflow: hidden; 
  text-overflow: ellipsis; 
  margin-bottom: 1%;
  cursor: pointer;
}
.request-item span {
  font-weight: 630;
}
.request-item p {
  font-weight: 630;
}

.no-requests {
  text-align: center;
  color: #7f8c8d;
  font-size: 1.2rem;
}

.new-request {
  background-color: #6454F2;
  color: white;
  width: 80%;
  border: none;
  height: 10%;
  border-radius: 40px;
  font-size: 1rem;
  cursor: pointer;
  margin-bottom: 2%;
  font-size: 3em;
  font-weight: bold;
}
</style>