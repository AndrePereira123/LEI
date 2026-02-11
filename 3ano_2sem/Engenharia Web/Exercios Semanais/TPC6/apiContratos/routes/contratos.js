var express = require('express');
var router = express.Router();
var Contrato = require('../controllers/contrato');

/* GET contratos. */
router.get('/', function(req, res, next) {

  if (req.query.entidade) {
    Contrato.getContratosByEntidade(req.query.entidade)
      .then
      (data => res.status(200).jsonp(data))
      .catch(error => res.status(500).send(`Erro na listagem de contratos: ${error}`))
  } 
  else {
  Contrato.getContratos()
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro na listagem de contratos: ${error}`))
    }
  });

/* GET entidades com NIPC. */
router.get('/entidades/NIPC/:id', function(req, res, next) {
  Contrato.getContratosByNIPC(req.params.id)
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro na listagem de entidades: ${error}`));
});


/* GET entidades. */
router.get('/entidades', function(req, res, next) {
  Contrato.getEntidades()
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro na listagem de entidades: ${error}`));
});


/* GET tipos de procedimento. */
router.get('/tipos', function(req, res, next) {
  Contrato.getTipos()
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro na listagem de tipos de procedimento: ${error}`));
});


/* GET contratos by tipo. */
router.get('/entidades/:tipo', function(req, res, next) {
  if (req.params.tipo) {
    Contrato.getContratosByTipo(req.params.tipo)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).send(`Erro na listagem de contratos por tipo: ${error}`));
  } else {
    res.status(400).send('Tipo de procedimento não especificado.');
  }
});

router.get('/favicon.ico', function(req, res, next) {
  res.status(200).end()
});

/* GET contrato by id. */
router.get('/:id', function(req, res, next) {
  Contrato.getContratoById(req.params.id)
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro na listagem de contratos: ${error}`))
});


/* POST inserir novo contrato. */
router.post('/', function(req, res, next) {
  Contrato.insert(req.body)
    .then(data => res.status(201).jsonp(data))
    .catch(error => res.status(500).send(`Erro ao adicionar contrato: ${error}`));
});

/* PUT atualizar info contrato */
router.put('/:id', function(req, res, next) {
  Contrato.update(req.params.id, req.body)
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro ao atualizar contrato: ${error}`));
});



/* DELETE contrato by id. */
router.delete('/:id', function(req, res, next) {
  Contrato.delete(req.params.id)
    .then(data => res.status(200).jsonp(data))
    .catch(error => res.status(500).send(`Erro ao eliminar contrato: ${error}`));
});



module.exports = router;
