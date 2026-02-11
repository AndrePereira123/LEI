var express = require('express');
var router = express.Router();
var Aluno = require("../controllers/alunos");

/* GET students list page. */
router.get('/', function(req, res, next) {
  Aluno.list()
    .then(data => {
      res.render('studentsListPage', { slist: data, d: new Date().toISOString() });
    })
    .catch(erro => {
      res.render('errorPage', { errorMessage: erro.message, d: new Date().toISOString() });
    });
});

/* GET student registration form. */
router.get('/regist', function(req, res, next) {
  try {
  res.render('studentFormPage', { d: new Date().toISOString() });
  } catch (error) {
    console.error('Error rendering studentPage:', error);
    next(error);
  }
});

/* GET student edit form. */
router.get('/edit/:id', function(req, res, next) {
  Aluno.findById(req.params.id)
    .then(data => {
      res.render('studentFormEditPage', { a: data, d: new Date().toISOString() });
    })
    .catch(erro => {
      res.render('errorPage', { errorMessage: erro.message, d: new Date().toISOString() });
    });
});

/* DELETE student. */
router.get('/delete/:id', function(req, res, next) {
  Aluno.delete(req.params.id)
    .then(data => {
      res.render('deletePage', { id: req.params.id, d: new Date().toISOString() });
    })
    .catch(erro => {
      res.render('errorPage', { errorMessage: "Erro ao apagar", d: new Date().toISOString() });
    });
});

/* GET individual student page. */
router.get('/:id', function(req, res, next) {
  Aluno.findById(req.params.id)
    .then(data => {
      res.render('studentPage', { aluno: data, d: new Date().toISOString() });
    })
    .catch(erro => {
      res.render('errorPage', { errorMessage: erro.message, d: new Date().toISOString() });
    });
});


/* POST new student. */
router.post('/regist', function(req, res, next) {
  Aluno.insert(req.body)
    .then(data => {
      console.log("Aluno inserido com sucesso:", data);
      res.render('registeredPage', { aluno: req.body , d: new Date().toISOString(), data });
    })
    .catch(erro => {
      res.render('errorPage', { errorMessage: erro.message, d: new Date().toISOString() });
    });
});

/* PUT update student. */
router.post('/edit/:id', function(req, res, next) {
  
  for (let i = 1; i <= 6; i++) {
    req.body[`tpc${i}`] = req.body[`tpc${i}`] === "1" ? 1 : 0;
  }

  Aluno.update(req.params.id, req.body)
    .then(data => {
      res.render('editedPage', { id: data.id, d: new Date().toISOString(), data });
    })
    .catch(erro => {
      res.render('errorPage', { errorMessage: erro.message, d: new Date().toISOString() });
    });
});




module.exports = router;