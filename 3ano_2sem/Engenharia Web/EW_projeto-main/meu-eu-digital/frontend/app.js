var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var jwt = require('jsonwebtoken');
var axios = require('axios'); // Add axios for proxy requests

var indexRouter = require('./routes/index');
var authRouter = require('./routes/auth');

var app = express();
app.use(cookieParser());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');
app.use(logger('dev'));
app.use(express.static(path.join(__dirname, 'public')));


app.use((req, res, next) => {
  const token = req.cookies.token;
  if (token) {
    jwt.verify(token, process.env.SECRET_KEY, (err, payload) => {
      if (!err && payload) {
        console.log('✅ Usuário autenticado para layout:', payload.username);
        req.user = payload;
        res.locals.user = payload; 
      } else {
        console.log('❌ Token inválido:', err?.message);
        req.user = null;
        res.locals.user = null;
        res.clearCookie('token');
      }
      next();
    });
  } else {
    console.log('🔍 Sem token - layout sem sidebar');
    req.user = null;
    res.locals.user = null;
    next();
  }
});

app.use('/', authRouter);
app.use('/', indexRouter);

// API proxy middleware - forward /api/* requests to backend
app.use('/api/*', async (req, res) => {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const apiPath = req.originalUrl; // This includes /api/...
    const fullUrl = `${backendUrl}${apiPath}`;
    
    console.log(`🔄 Proxying API request: ${req.method} ${fullUrl}`);
    
    const config = {
      method: req.method,
      url: fullUrl,
      headers: {
        'Content-Type': 'application/json',
        ...req.headers
      }
    };
    
    // Add request body for POST/PUT requests
    if (req.body && Object.keys(req.body).length > 0) {
      config.data = req.body;
    }
    
    // Add query parameters
    if (req.query && Object.keys(req.query).length > 0) {
      config.params = req.query;
    }
    
    const response = await axios(config);
    
    // Forward the response
    res.status(response.status).json(response.data);
    
  } catch (error) {
    console.error('❌ API proxy error:', error.message);
    
    if (error.response) {
      // Backend returned an error response
      res.status(error.response.status).json(error.response.data);
    } else {
      // Network or other error
      res.status(500).json({ 
        success: false, 
        error: 'Backend connection error' 
      });
    }
  }
});

app.use(function(req, res, next) {
  next(createError(404));
});

app.use(function(err, req, res, next) {
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
