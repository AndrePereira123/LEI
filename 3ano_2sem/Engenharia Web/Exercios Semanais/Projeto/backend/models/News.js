const mongoose = require('mongoose');

const NewsSchema = new mongoose.Schema({
  titulo: { type: String, required: true },
  conteudo: { type: String, required: true },
  visivel: { type: Boolean, default: false },
  dataCriacao: { type: Date, default: Date.now }
});

module.exports = mongoose.model('News', NewsSchema);