const express = require('express');
const path = require('path');
const cookieParser = require('cookie-parser');
const logger = require('morgan');
const mongoose = require('mongoose');
var passport = require('passport');
var session = require('express-session')

const mongoDB = process.env.MONGO_URL  
mongoose.connect(mongoDB)
var connection = mongoose.connection
connection.on("error", console.error.bind(console, "Erro na conexão com o MongoDB"))
connection.once("open",() => console.log("Conexão ao MongoDB realizada com sucesso."))


const {v4 : uuidv4} = require('uuid')
var LocalStratagy = require('passport-local').Strategy

var User = require('./models/User')
passport.use(new LocalStratagy(User.authenticate()))
passport.serializeUser(User.serializeUser())
passport.deserializeUser(User.deserializeUser())


const sipRoutes = require('./routes/sip');
const itemRoutes = require('./routes/itemRoutes');
const loginRoutes = require('./routes/login');
const userRoutes = require('./routes/userRoutes');

const app = express();

app.use(session({
  genid: req => {
    return uuidv4()
  },
  secret: process.env.SECRET_KEY,
  resave: false,
  saveUninitialized: true
}))


app.use('/uploads', express.static(path.join(__dirname, 'uploads')));

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));


app.use('/', loginRoutes);
app.use('/api/sip', sipRoutes);
app.use('/api/items', itemRoutes);
app.use('/api/users', userRoutes);


module.exports = app;
