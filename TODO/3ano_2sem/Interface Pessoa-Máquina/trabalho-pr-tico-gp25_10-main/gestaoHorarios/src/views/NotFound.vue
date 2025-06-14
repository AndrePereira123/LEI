<template>
    <div class="not-found">
      <h1>404</h1>
      <p>Página não encontrada.</p>
      <router-link to="/">Voltar à página inicial</router-link>
    </div>
  </template>
  
<script setup>
</script>
  
<style scoped>
.not-found {
text-align: center;
margin-top: 100px;
}

.not-found h1 {
font-size: 6rem;
margin-bottom: 20px;
}

.not-found p {
font-size: 1.5rem;
margin-bottom: 20px;
}

.not-found a {
color: #42b983;
text-decoration: none;
}
</style>
  