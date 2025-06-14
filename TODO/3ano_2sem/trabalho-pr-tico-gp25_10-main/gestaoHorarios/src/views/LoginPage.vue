<template>
    <div class="login-container">
        <div class="sidebar">
            <img src="/images/UM_LOGO.png" class="logo" alt="logo_UM"/>
        </div>

        <div class="login-form">
            <h1>Iniciar Sessão</h1>
            <form @submit.prevent="iniciarSessao(email, pass)">
                <input aria-label="email" type="email" v-model="email" placeholder="Email" required />
                <input aria-label="password" type="password" v-model="pass" placeholder="Palavra-Passe" required />
                <button type="submit">Sign In</button>
            </form>
        </div>
    </div>
</template>

<script>
import { useAuthStore } from '../stores/auth'

export default {
    name: 'Iniciar Sessão',
    data() {
        return {
            email: '',
            pass: '',
            auth: useAuthStore()
        }
    },
    methods: {
        async iniciarSessao() {
            const role = await this.auth.login(this.email, this.pass)

            if (role === 'diretor') {
                this.$router.push({ name: 'pagina_inicial' })
            } else if (role === 'aluno') {
                this.$router.push({ name: 'pagina_inicial_aluno' })
            } else {
                this.$router.push({ name: 'Credenciais Inválidas' })
            }
        }
    }
}
</script>

<style scoped>
.login-container {
    display: flex;
    height: 100vh; 
    width: 100vw; 
    background-color: #2c3e50; 
    overflow-x: hidden; 
    overflow-y: hidden; 
}

.sidebar {
  width: 20%; 
  height: 100%;
  background-color: white;
  display: flex;
  flex-direction: column;
  align-items: center;
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

.login-form {
    flex: 1;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    padding: 60px;
    overflow-x: hidden; 
    overflow-y: hidden; 
}

.login-form h1 {
    font-size: 50px;
    color: white;
    font-weight: bold;
}

.login-form form {
    background-color: #55647D;
    padding: 20px; 
    padding-bottom: 60px;
    border-radius: 15px;
    box-shadow: 0 6px 10px rgba(0, 0, 0, 0.15);
    width: 600px;
    text-align: center;
    display: flex; 
    flex-direction: column; 
    align-items: center;
    gap: 20px; 
}

.login-form input {
    width: 100%;
    height: 20%;
    padding: 15px;
    margin: 15px 0;
    margin-top: 40px ;
    border: none;
    border-radius: 10px;
    font-size: 20px;
}

.login-form button {
    width: 100%;
    padding: 15px;
    background-color: #2B2424;
    border: none;
    border-radius: 10px;
    color: white;
    font-size: 20px;
    cursor: pointer;
}

.login-form button:hover {
    background-color: #16a085;
}
</style>