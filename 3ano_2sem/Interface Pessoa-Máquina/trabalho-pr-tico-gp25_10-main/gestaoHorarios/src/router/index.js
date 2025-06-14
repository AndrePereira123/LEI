import { createRouter, createWebHistory } from 'vue-router'
import { useAuthStore } from '@/stores/auth';
import LoginPage from '../views/LoginPage.vue'


const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'Iniciar Sessão',
      component: LoginPage,
      meta: { hideSidebar: true , hideTerminarSessao: true, requiresAuth: false, title: 'Iniciar Sessão' },
    },
    {
      path: '/pagina_inicial',
      name: 'pagina_inicial',
      component: () => import('@/views/PaginaInicial.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Página Inicial' },
    },
    {
      path: '/aluno/pagina_inicial',
      name: 'pagina_inicial_aluno',
      component: () => import('@/views/Aluno_PaginaInicial.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Aluno', title: 'Página Inicial' },
    },
    {
      path: '/aluno/horario',
      name: 'horario_aluno',
      component: () => import('@/views/Aluno_Horario.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Aluno', title: 'Horário' },
    },
    {
      path: '/aluno/pedidos/enviados',
      name: 'pedidos_enviados_aluno',
      component: () => import('@/views/Aluno_PedidosPendentes.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Aluno', title: 'Pedidos Pendentes' },
    },
    {
      path: '/aluno/pedidos/novopedido',
      name: 'novo_pedido_aluno',
      component: () => import('@/views/Aluno_NovoPedido.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Aluno', title: 'Novo Pedido' },
    },
    {
      path: '/aluno/pedidos/:type/:id',
      name: 'Ver Pedido Aluno',
      component: () => import('@/views/Aluno_VerPedido.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Aluno', title: 'Ver Pedido' },
    },
    {
      path: '/aluno/pedidos',
      name: 'pedidos_aluno',
      component: () => import('@/views/Aluno_Pedidos.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Aluno', title: 'Pedidos' },
    },
    {
      path: '/credenciais_invalidas',
      name: 'Credenciais Inválidas',
      component: () => import('../views/CredenciaisInvalidas.vue'),
      meta: { hideSidebar: true , hideTerminarSessao: true, requiresAuth: false, title: 'Credenciais Inválidas' },
    },
    {
      path: '/consultar-horario-UC',
      name: 'Consultar Horário de UC',
      component: () => import('../views/ConsultarHorario.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Consultar Horário de UC' },
    },
    {
      path: '/consultar-horario-UC/curso/:id/editar-turno/:idTurno',
      name: 'Editar Turno',
      component: () => import('../views/EditarTurno.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Editar Turno' },
    },
    {
      path: '/consultar-horario-UC/curso/:id/listar-alunos/:idTurno',
      name: 'Listar Alunos',
      component: () => import('../views/ListarAlunosTurno.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Listar Alunos do Turno' },
    },
    {
      path: '/consultar-horario-UC/curso/:id',
      name: 'Detalhes UC',
      component: () => import('../views/DetalhesUC.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Detalhes da UC' },
    },
    {
      path: '/gestao-alunos',
      name: 'Detalhes de Aluno',
      component: () => import('../views/GestaoAlunos.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Gestão de Alunos' },
    },
    {
      path: '/gestao-alunos/listar-alunos-curso',
      name: 'Listar Alunos do Curso',
      component: () => import('../views/ListarAlunosCurso.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Listar Alunos do Curso' },
    },
    {
      path: '/gestao-alunos/listar-alunos-curso/:idAluno',
      name: 'Aluno do Curso',
      component: () => import('../views/DetalhesAluno_ListaAlunosCurso.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Detalhes do Aluno' },
    },
    {
      path: '/gestao-alunos/listar-alunos-colisoes',
      name: 'Listar Alunos com Colisões',
      component: () => import('../views/ListaAlunosComColisoes.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Listar Alunos com Colisões' },
    },
    {
      path: '/gestao-alunos/listar-alunos-horario-incompleto',
      name: 'Listar Alunos com Horário Incompleto',
      component: () => import('../views/ListaAlunosComHorarioIncompleto.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Listar Alunos com Horário Incompleto' },
    },
    {
      path: '/gestao-alunos/alocacao-manual-turno',
      name: 'Alocação Manual de Turno',
      component: () => import('../views/AlocacaoManual.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Alocação Manual de Turno' },
    },
    {
      path: '/gestao-alunos/alocacao-manual-turno/lista-alunos',
      name: 'Alocação Manual de Turno Lista Alunos',
      component: () => import('../views/AlocacaoManual_Lista_Alunos.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Alocação Manual de Turno - Lista Alunos' },
    },
    {
      path: '/gestao-alunos/troca-turnos',
      name: 'Troca de Turnos',
      component: () => import('../views/TrocaDeTurnos.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Troca de Turnos' },
    },
    {
      path: '/gestao-alunos/troca-turnos/lista-alunos',
      name: 'Troca de Turnos - Lista Alunos',
      component: () => import('../views/TrocaDeTurnos_Lista_Alunos.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Troca de Turnos - Lista Alunos' },
    },
    {
      path: '/gestao-alunos/remover-alocacao/lista-alunos',
      name: 'Remover Turno de Aluno - Lista Alunos',
      component: () => import('../views/RemoverTurno_Lista_Alunos.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Remover Turno de Aluno - Lista Alunos' },
    },
    {
      path: '/gestao-alunos/remover-alocacao',
      name: 'Remover Turno de Aluno',
      component: () => import('../views/RemoverTurno.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Remover Turno de Aluno' },
    },
    {
      path: '/publicar-horarios',
      name: 'Publicar Horários',
      component: () => import('../views/PublicarHorarios.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Publicar Horários' },
    },
    {
      path: '/gestao-alunos/ver-horario/:id',
      name: 'Ver Horário',
      component: () => import('../views/VerHorario.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Ver Horário' },
    },
    {
      path: '/atendimento-pedidos/enviadas',
      name: 'Pedidos respondidos',
      component: () => import('../views/AtendimentoPedidos_verEnviadas.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Pedidos Respondidos' },
    },
    {
      path: '/atendimento-pedidos/:type/resposta/:id',
      name: 'Responder Pedido',
      component: () => import('../views/AtendimentoPedidos_Resposta.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Responder Pedido' },
    },
    {
      path: '/atendimento-pedidos/:type/:id',
      name: 'Ver Pedido',
      component: () => import('../views/AtendimentoPedidos_VerPedido.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Ver Pedido' },
    },
    {
      path: '/atendimento-pedidos',
      name: 'Atendimento de Pedidos',
      component: () => import('../views/AtendimentoPedidos.vue'),
      meta: { requiresAuth: true, requiredUserType: 'Diretor', title: 'Atendimento de Pedidos' },
    },
    {
      path: '/:pathMatch(.*)*',
      name: 'NotFound',
      component: () => import('../views/NotFound.vue'),
      meta: { hideSidebar: true, hideTerminarSessao: true, title: 'Página Não Encontrada' },
    }
    
  ]
})

router.beforeEach((to, from, next) => {
  const authStore = useAuthStore();
  const isAuthenticated = authStore.checkAuth();
  const userType = authStore.role;
  console.log(isAuthenticated)
  console.log(userType)

  // SE O MATCH FALHAR (não existe rota válida)
  if (to.matched.length === 0) {
    next({ name: 'NotFound' });
    
  } else if ((to.name === 'Iniciar Sessão' || to.name === 'Credenciais Inválidas') && isAuthenticated) {
    if (userType === 'Diretor') {
      next({ name: 'pagina_inicial' });
    } else if (userType === 'Aluno') {
      next({ name: 'pagina_inicial_aluno' });
    } else {
      next();
    }
  }
  else if (to.meta.requiresAuth && !isAuthenticated) {
    next({name: 'Iniciar Sessão'});
  } else if (to.meta.requiredUserType && to.meta.requiredUserType !== userType) {
    next({name: 'Iniciar Sessão'});
  } else {
    if (from && from.path) {
      sessionStorage.setItem('previousRoute', from.path); 
    }
    next();
  }


});

router.afterEach((to) => {
  if (to.meta?.title) {
    document.title = to.meta.title;
  }
});

export default router
