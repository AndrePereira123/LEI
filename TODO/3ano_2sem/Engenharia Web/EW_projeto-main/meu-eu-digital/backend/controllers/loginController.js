const User = require('../models/User');
const jwt = require('jsonwebtoken');
const passport = require('passport');

passport.use(User.createStrategy());
passport.serializeUser(User.serializeUser());
passport.deserializeUser(User.deserializeUser());

// POST /api/auth/login - Usar Passport authenticate
exports.login = (req, res, next) => {
  
  const { user } = req;
  
  jwt.sign(
    {
      id: user._id,
      username: user.username,
      email: user.email,
      tipo: user.tipo,
      sub: 'aula de EngWeb2025'
    },
    process.env.SECRET_KEY, // Chave secreta para assinar o token
    { expiresIn: 3600 },
    (err, token) => {
      if(err) res.jsonp(err)
      else res.status(201).jsonp({token: token})
    }
  );
};


// POST /api/auth/register - Registrar novo User
exports.register = async (req, res) => {
  try {
    const { nome, username, email, password, tipo } = req.body;

    if (!nome || !username || !email || !password) {
      return res.status(400).json({
        success: false,
        message: 'Todos os campos são obrigatórios'
      });
    }

    if (username === "Deleted User")
      return res.status(403).json({
        success: false,
        message: 'Username "Deleted User" não é permitido'
      });


    // Verificar se User já existe
    const existingUser = await User.findOne({
      $or: [
        { email: email },
        { username: username }
      ]
    });

    if (existingUser) {
      return res.status(409).json({
        success: false,
        message: existingUser.email === email ? 'Email já está em uso' : 'Username já está em uso'
      });
    }

    const newUser = new User({
      username: username,
      nome: nome,
      email: email,
      tipo: tipo || 'produtor'
    });

    User.register(newUser, password, (err, user) => {
      if (err) {
        console.error('Erro no registro:', err);
        return res.status(500).json({
          success: false,
          message: err.message || 'Erro interno do servidor'
        });
      }

      console.log('User registrado:', { user });

      res.status(201).json({
        success: true,
        message: 'User criado com sucesso',
        user: {
          id: user._id,
          username: user.username,
          name: user.nome,
          email: user.email,
          tipo: user.tipo
        }
      });
    });

  } catch (error) {
    console.error('Erro no registro:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
};


exports.validate = (req, res, next) => {
  console.log('=== VALIDATE MIDDLEWARE ===');
  console.log('Headers:', req.headers);
  console.log('Authorization header:', req.get('Authorization'));
  
  const authHeader = req.get('Authorization');
  
  if (!authHeader || !authHeader.startsWith('Bearer ')) {
    console.log('Token inexistente ou mal formatado');
    return res.status(401).json({ error: "Token inexistente ou mal formatado" });
  }

  const token = authHeader.split(' ')[1];
  console.log('Token extraído:', token ? 'SIM (presente)' : 'NÃO');

  jwt.verify(token, process.env.SECRET_KEY, (err, payload) => {
    if (err) {
      console.log('Erro na verificação do token:', err.message);
      return res.status(401).json({ error: "Token inválido: " + err.message });
    }
    
    console.log('Token válido:', payload);
    req.user = payload; 
    next();
  });
};


exports.validateAdmin = (req, res, next) => {
  const authHeader = req.get('Authorization');
  
  if (!authHeader || !authHeader.startsWith('Bearer ')) {
    return res.status(401).json({ error: "Token inexistente" });
  }

  const token = authHeader.split(' ')[1];

  jwt.verify(token, process.env.SECRET_KEY, (err, payload) => {
    if (err) {
      return res.status(401).json({ error: "Token inválido" });
    }
    
    if (payload.tipo !== "admin") {
      return res.status(403).json({ error: "User sem permissão para aceder ao conteúdo" });
    }
    
    console.log('Admin autorizado:', payload);
    req.user = payload;
    next();
  });
};

