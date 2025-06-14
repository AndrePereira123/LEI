var express = require('express');
var router = express.Router();
const loginController = require('../controllers/loginController');
const passport = require('passport');

// Rotas de autenticação
router.post('/api/auth/login', 
  passport.authenticate('local', { session: false }), // session: false para JWT
  loginController.login
);

router.post('/api/auth/register', loginController.register);

module.exports = router;