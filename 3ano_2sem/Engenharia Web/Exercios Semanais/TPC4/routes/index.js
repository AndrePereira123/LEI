var express = require('express');
var router = express.Router();
var axios = require('axios');

/* GET home page. */
router.get('/', function(req, res) { //- pedido - resposta -
  res.render('index', 
    { 
    title: 'Engenharia Web 2025',
    docente: "jcr",
    instituicao: "DI-UM"
  });
});


router.get('/filmes', function(req, res) { //- pedido - resposta -
  axios.get("http://localhost:3000/filmes")
    .then(resp => {
      res.render("filmes",{lfilmes:resp.data,tit:"Lista de Filmes"})
    })
    .catch(error =>{
        console.log(error);
        res.render("error",{error: error})
        }
    )
});

router.get('/filmes/delete/:id', function(req, res) { //- pedido - resposta -
  const filmeId = req.params.id;
  var tit;
  axios.get('http://localhost:3000/filmes/' + filmeId)
  .then(response => {
    tit = response.data.title;  // This will log the title of the movie
    axios.delete("http://localhost:3000/filmes/" + filmeId)
    .then(resp => {
      res.render("deletefilme",{tit:`Filme "${tit}" apagado com sucesso`})
    })
    .catch(error =>{
        console.log(error);
        res.render("error",{error: error})
        }
    )
  })
  .catch(error => {
    console.error(error);
  });
  
});

router.get('/ator/:nome', function(req, res) { //- pedido - resposta -
  const nome_ator = req.params.nome
  axios.get("http://localhost:3000/filmes")
    .then(resp => {
      res.render("ator",{lfilmes:resp.data,tit:"Lista de Filmes com o ator " + nome_ator,ator: nome_ator})
    })
    .catch(error =>{
        console.log(error);
        res.render("error",{error: error})
        }
    )
});

router.get('/filmes/edit/:id', function(req, res) { //- pedido - resposta -
  const filmeId = req.params.id;
  axios.get("http://localhost:3000/filmes/" + filmeId)
    .then(resp => {
      res.render("editfilme",{filme:resp.data,cast:resp.data.cast,genres:resp.data.genres,tit:`Editar filme "${resp.data.title}"`})
    })
    .catch(error =>{
        console.log(error);
        res.render("error",{error: error})
        }
    )
});


router.post('/filmes/edit/:id', function(req, res) { //- pedido - resposta -
  const filme_editado = req.body
  if (typeof filme_editado.cast === 'string') {
    filme_editado.cast = [filme_editado.cast];  
  }
  if (typeof filme_editado.genres === 'string') {
    filme_editado.genres = [filme_editado.genres];  
  }

  axios.put("http://localhost:3000/filmes/" + filme_editado.id,filme_editado)
    .then(resp => {
      console.log(resp.data)
      res.status(200).redirect("/filmes")
    })
    .catch(error =>{
        console.log(error);
        res.render("error",{error: error})
        }
    )
});






module.exports = router;
