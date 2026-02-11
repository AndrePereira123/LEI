var jwt = require('jsonwebtoken');

// Middleware de autenticação JWT para páginas web

const requireAuth = (req, res, next) => {
  if (req.user) {
    console.log('✅ Usuário autenticado:', req.user.username);
    next();
  } else {
    console.log('❌ Usuário não autenticado, redirecionando para login');
    res.redirect('/login');
  }
};

const requireAdmin = (req, res, next) => {
  if (req.user && req.user.tipo === "admin") {
    console.log('✅ Usuário é admin:', req.user.username);
    next();
  } else {
    console.log('❌ Usuário não é admin');
    res.status(403).render('error', {
      message: 'Acesso negado',
      error: { status: 403, stack: 'Você não tem permissão para acessar esta página' }
    });
  }
};

const requireProdutor = (req, res, next) => {
  if (req.user && (req.user.tipo === "produtor" || req.user.tipo === "admin")) {
    console.log('✅ Usuário é produtor/admin:', req.user.username);
    next();
  } else {
    console.log('❌ Usuário não é produtor/admin');
    res.status(403).render('error', {
      message: 'Acesso negado',
      error: { status: 403, stack: 'Você precisa ser produtor ou admin para acessar esta página' }
    });
  }
};

module.exports = {
  requireAuth,
  requireAdmin,
  requireProdutor
};