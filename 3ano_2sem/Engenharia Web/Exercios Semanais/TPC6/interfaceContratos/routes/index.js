var express = require('express');
var axios = require('axios');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  axios.get('http://localhost:16000/contratos')
    .then(function (contratos) {
      res.render('index', { title: 'Lista de Contratos', contratos: contratos.data });
    })
    .catch(function (error) {
      console.log(error);
    });
});


router.get('/entidades/:id', function(req, res, next) {
  axios.get('http://localhost:16000/contratos/entidades/NIPC/' + req.params.id)
    .then(function (contratos) {
      res.render('entidade', {NIPC: req.params.id, contratos: contratos.data });
    })
    .catch(function (error) {
      console.log(error);
    });
});

router.get('/:id', function(req, res, next) {
  axios.get('http://localhost:16000/contratos/' + req.params.id)
    .then(function (contrato) {
      res.render('contrato', { title: 'Contrato', contrato: contrato.data });
    })
    .catch(function (error) {
      console.log(error);
    });
});



module.exports = router;