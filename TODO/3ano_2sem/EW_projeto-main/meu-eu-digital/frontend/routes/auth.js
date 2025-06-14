var express = require('express');
var axios = require('axios');

var router = express.Router();

// Show login page (only for guests)
router.get('/login', function(req, res, next) {
  res.render('login', {
    success: req.query.success,
    error: req.query.error
  });
});

// Show register page (only for guests)
router.get('/register', function(req, res, next) {
  res.render('register', {
    success: req.query.success,
    error: req.query.error
  });
});


router.post('/login/password', async function(req, res, next) {
  try {
    const { username, password } = req.body;
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    
    const response = await axios.post(`${backendUrl}/api/auth/login`, {
      username: username,
      password: password
    });


    if (response.data && response.data.token) {

      res.cookie('token', response.data.token, {
        httpOnly: true,     
        secure: false,      
        maxAge: 24 * 60 * 60 * 1000, 
        sameSite: 'lax'     
      });

      res.redirect('/');
      
    } else {
      res.redirect('/login?error=Erro no servidor - token não recebido');
    }

  } catch (error) {
    
    let errorMessage = 'Erro no servidor';
    if (error.response) {
      
      if (error.response.status === 401) {
        errorMessage = 'Username ou password incorretos';
      } else if (error.response.status === 404) {
        errorMessage = 'Serviço de login não encontrado';
      } else if (error.response.data && error.response.data.message) {
        errorMessage = error.response.data.message;
      }
    } else if (error.code === 'ECONNREFUSED') {
      errorMessage = 'Não foi possível conectar ao servidor';
    }
    
    res.redirect('/login?error=' + encodeURIComponent(errorMessage));
  }
});


router.post('/register', async function(req, res, next) {
  try {
    const { nome, username, email, password, confirmPassword, tipo } = req.body;

    
    if (!nome || !username || !email || !password || !confirmPassword) {
      return res.redirect('/register?error=Todos os campos são obrigatórios');
    }

    if (password !== confirmPassword) {
      return res.redirect('/register?error=As passwords não coincidem');
    }

    if (password.length < 6) {
      return res.redirect('/register?error=Password deve ter pelo menos 6 caracteres');
    }

    // Chamar backend para registar
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const response = await axios.post(`${backendUrl}/api/auth/register`, {
      nome: nome,
      username: username,
      email: email,
      password: password,
      tipo: tipo || 'produtor'
    });

    if (response.data.success) {
      console.log('Registo bem-sucedido:', response.data.user);
      return res.redirect('/login?success=Conta criada com sucesso! Faça login.');
    } else {
      return res.redirect('/register?error=' + encodeURIComponent(response.data.message));
    }

  } catch (error) {
    console.error('Erro no registo:', error.message);
    
    if (error.response) {
      return res.redirect('/register?error=' + encodeURIComponent(error.response.data.message));
    }
  }
});

// Logout route
router.get('/logout', function(req, res, next) {
  res.clearCookie('token');
  res.redirect('/login');
});

module.exports = router;