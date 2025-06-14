<script setup>
import { ref } from 'vue';
import { useRouter } from 'vue-router';

const router = useRouter();
const shifts = ref([]);
const allocations = ref([]);

const publicarHorarios = async() => {
  try{
    const shiftsResponse = await fetch('http://localhost:3000/shifts');
    if (!shiftsResponse.ok) {
      throw new Error('Erro ao buscar os turnos');
    }
    shifts.value = await shiftsResponse.json();

    const allocationsResponse = await fetch('http://localhost:3000/allocations');
    if (!allocationsResponse.ok) {
      throw new Error('Erro ao buscar as alocações');
    }
    allocations.value = await allocationsResponse.json();


    // Add deep copy using JSON.parse/stringify to break object references
    const deepCopyShifts = JSON.parse(JSON.stringify(shifts.value));
    const deepCopyAllocations = JSON.parse(JSON.stringify(allocations.value));


    await fetch('http://localhost:3000/publishedHorarios/1', {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        id: 1,
        shifts: deepCopyShifts,
        allocations: deepCopyAllocations
      }),
    });

  }
  catch (error) {
    console.error('Erro ao publicar os horários:', error);
    alert('Erro ao publicar os horários. Tente novamente mais tarde.');
    return;
  }

  alert('Horários publicados com sucesso!');

  router.push('/pagina_inicial'); 
};

// Função para cancelar a publicação
const cancelarPublicacao = () => {
  router.push('/pagina_inicial'); 
};
</script>

<template>
  <main class="publicar-horarios">
    <div class="caixa-confirmacao">
      <div class="icone-alerta">
        <h1>Tem a certeza de que quer publicar os horários?</h1>
        <img src="https://cdn-icons-png.flaticon.com/512/595/595067.png" style="width: 20vh;height: 20vh;margin-top: 5vh;" alt="Alerta" />
      </div>
      <div class="botoes">
        <button class="botao-sim" @click="publicarHorarios">Sim</button>
        <button class="botao-nao" @click="cancelarPublicacao">Não</button>
      </div>
    </div>
  </main>
</template>

<style scoped>
.publicar-horarios {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100vh;
  background-color: #2c3e50;
}

.caixa-confirmacao {
  display: flex;
  flex-direction: column;
  background-color: #D9D9D9; 
  padding: 5vh;
  border-radius: 30px;
  
  align-self: center;
  justify-self: center;
  text-align: center;
  justify-content: center;
  align-items: center;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  width: 110vh;
  height: 60vh;
  margin-bottom: 10vh;
}

.caixa-confirmacao h1 {
  line-height: 5rem;
  font-size: 4rem;
  font-weight: 750;
  color: #2c3e50;
  margin-bottom: 10vh;
}

.icone-alerta {
  text-align: left;
  display: flex;
  flex-direction: row;
}

.botoes {
  display: flex;
  justify-content: space-around;
  margin-top: 20px;
  gap: 20vh;
}

.botao-sim {
  background-color:  #1F996E; 
  color: white;
  border: none;
  border-radius: 50px;
  padding: 10px 20px;
  font-size: 2rem;
  width: 30vh;
  height: 10vh;
  cursor: pointer;
  transition: background-color 0.3s;
}

.botao-sim:hover {
  background-color: #008000; /* Verde mais claro */
}

.botao-nao {
  background-color: #C83939; /* Vermelho */
  color: white;
  border: none;
  border-radius: 50px;
  padding: 10px 20px;
  font-size: 2rem;
  width: 30vh;
  height: 10vh;
  cursor: pointer;
  transition: background-color 0.3s;
}

.botao-nao:hover {
  background-color: red; 
}
</style>