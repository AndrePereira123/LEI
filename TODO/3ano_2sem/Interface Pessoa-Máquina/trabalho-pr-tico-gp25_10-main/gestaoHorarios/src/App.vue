<script>
import SideBar from './components/SideBar.vue';
import TerminarSessao from './components/TerminarSessao.vue';

export default {
  components: {
    SideBar,
    TerminarSessao
  },
  data() {
    return {
    
    }
  },
  methods: { 
      
  }
   
}

</script>


<template>
  <div id="app">
    <side-bar v-if="!$route.meta.hideSidebar"></side-bar>
    <div class="content">
      <terminar-sessao v-if="!$route.meta.hideTerminarSessao"></terminar-sessao> 
      <router-view></router-view>
    </div>

    
  </div>
</template>

<style>

#app {
    display: flex;
    flex-direction: row;
    min-width: 100%; 
    min-height: 100%; 
    margin: 0;
    padding: 0;
    overflow: hidden;
}

.content {
    display: flex;
    flex: 1;
    flex-direction: column;
    min-height: 100%;
    min-width: 80%;
    overflow: hidden;
}

html, body {
    overflow: hidden; 
}


</style>

