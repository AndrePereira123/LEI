<template>
    <div class="error-container">
        <div class="sidebar">
            <img src="/images/UM_LOGO_VERMELHO.png" alt="Universidade do Minho" class="logo" />
        </div>

        <div class="warning-container">
            <img src="/images/WARNING.png" alt="Aviso" class="warning-image" />
            <div class="warning-text">Credenciais Inválidas</div>
            <button @click="redirecionarParaLogin">Voltar para Login</button>
        </div>
    </div>
</template>

<script>
export default {
    name: 'CredenciaisInvalidas',
    methods: {
        redirecionarParaLogin() {
            this.$router.push({ path: '/' })
        },
    },
}
</script>

<style scoped>
.error-container {
    display: flex;
    height: 100vh; 
    width: 100vw; 
    background-color: #6C2B2C;
    overflow-x: hidden; 
    overflow-y: hidden; 
}

.sidebar {
    width: 20%; 
    height: 100%; 
    background-color: white;
    display: flex;
    flex-direction: column;
    padding: 1%;
    box-shadow: 2px 0 5px rgba(0, 0, 0, 0.1);
}

.sidebar .logo {
  width: 100%;
  margin-bottom: 20px;
}


.sidebar p {
    font-size: 24px;
    font-weight: bold;
    color: #333;
    text-align: center;
}

.warning-container {
    flex: 1;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    text-align: center;
    color: white;
}

.warning-container .warning-image {
    width: 300px;
    margin-bottom: 20px;
}

.warning-container .warning-text {
    background-color: red;
    color: white;
    font-size: 24px;
    font-weight: bold;
    padding: 10px 20px;
    border-radius: 30px;
    margin-bottom: 20px;
}

.warning-container button {
    padding: 15px 30px;
    font-size: 16px;
    background-color: #007bff;
    color: white;
    border: none;
    border-radius: 5px;
    cursor: pointer;
}

.warning-container button:hover {
    background-color: #0056b3;
}
</style>