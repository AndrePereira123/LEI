var express = require('express');
var router = express.Router();
var Livro = require('../controllers/livro');

/* GET Livros. */
router.get('/', function(req, res, next) {
  if (req.query.entidade) {
    Livro.getLivrosByEntidade(req.query.entidade)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).send(`Erro na listagem de Livros: ${error}`));
  } 
  else if (req.query.character) {
    Livro.getLivrosByCharacter(req.query.character)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).send(`Erro na listagem de Livros por personagem: ${error}`));
  } 
  else if (req.query.genre) {
    Livro.getLivrosByGenre(req.query.genre)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).send(`Erro na listagem de Livros por género: ${error}`));
  } 
  else if (req.query.author) {
    Livro.getLivrosByAuthor(req.query.author)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).send(`Erro na listagem de Livros por género: ${error}`));
  } 
  else {
    Livro.getLivros()
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).send(`Erro na listagem de Livros: ${error}`));
  }
});

/* GET lista de géneros ordenada alfabeticamente e sem repetições. */
router.get('/genres', function(req, res, next) {
  Livro.getGenres()
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro na listagem de géneros: ${error}`));
});

/* GET lista de personagens ordenada alfabeticamente e sem repetições. */
router.get('/characters', function(req, res, next) {
  Livro.getCharacters()
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro na listagem de personagens: ${error}`));
});

router.get('/:id', function(req, res, next) {
  Livro.getLivroById(req.params.id)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).send(`Erro na procura do Livro: ${error}`));
});

/* POST inserir novo Livro. */
router.post('/', function(req, res, next) {
  Livro.insert(req.body)
    .then(data => res.status(201).jsonp(data))
    .catch(error => res.status(500).send(`Erro ao adicionar Livro: ${error}`));
});

/* DELETE Livro by id. */
router.delete('/:id', function(req, res, next) {
  Livro.delete(req.params.id)
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro ao eliminar Livro: ${error}`));
});

/* PUT atualizar info Livro */
router.put('/:id', function(req, res, next) {
  Livro.update(req.params.id, req.body)
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro ao atualizar Livro: ${error}`));
});

module.exports = router;