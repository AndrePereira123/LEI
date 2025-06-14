const mongoose = require('mongoose');

const ItemSchema = new mongoose.Schema({
  titulo: { type: String, required: true },
  tipoRecurso: { type: String, required: true }, // Ex: 'viagem', 'foto', etc.
  dataCriacao: { type: Date, required: true },
  dataSubmissao: { type: Date, default: Date.now },
  produtor: { type: String, required: true },      // Ex: nome do criador
  submetidoPor: { type: mongoose.Schema.Types.ObjectId, ref: 'User', required: true },  // Referência ao utilizador que fez a submissão
  visibilidade: { type: String, enum: ['publico', 'privado'], default: 'privado' },
  descricao: { type: String},                        // Descrição do item
  classificadores: [{ type: String }], // Lista de categorias 
  ficheiros: [{                        // Lista de ficheiros associados
    nomeOriginal: String,
    caminho: String
  }],  
  comentarios: [{                     // Lista de comentários associados
    texto: String,
    autor: String,
    autorId: { type: mongoose.Schema.Types.ObjectId, ref: 'User' },
    data: { type: Date, default: Date.now }
  }]
});

module.exports = mongoose.model('Item', ItemSchema);
