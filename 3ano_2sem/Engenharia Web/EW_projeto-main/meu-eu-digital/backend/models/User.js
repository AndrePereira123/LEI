const mongoose = require('mongoose');
const passportLocalMongoose = require('passport-local-mongoose');

const UserSchema = new mongoose.Schema({
  nome: { type: String, required: true },
  username: { type: String, required: true, unique: true },
  email: { type: String, required: true, unique: true },
  tipo: { type: String, enum: ['admin', 'produtor'], default: 'produtor' },
  googleId: { type: String },   // Para login Google
  facebookId: { type: String }, // Para login Facebook
  dataCriacao: { type: Date, default: Date.now }
});


UserSchema.plugin(passportLocalMongoose, {
  usernameField: 'username' 
});

module.exports = mongoose.model('User', UserSchema);
