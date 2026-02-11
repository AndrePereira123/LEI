var express = require('express');
var axios = require('axios'); // Add axios for HTTP requests
var router = express.Router();

/* GET home page. */
router.get('/', async function(req, res, next) {
  try {
    const response = await axios.get('http://localhost:17000/books');
    const books = response.data;

    res.render('index', { title: 'Lista de Registos', books: books });
  } catch (error) {
    console.error('Error :', error.message);
    res.render('index', { title: 'Lista de Registos', books: [], error: 'Failed to fetch books' });
  }
});

router.get('/entidades/:idAutor', async function(req, res, next) {
  try {
    const response = await axios.get('http://localhost:17000/books?author=' + req.params.idAutor);
    const books = response.data;
    const extractedAuthor = req.params.idAutor.split('(')[0].trim();

    res.render('author', { title: 'Author -' + req.params.idAutor + " - " + extractedAuthor, books: books });
  } catch (error) {
    console.error('Error :', error.message);
    res.render('author', { title: 'Author -' + req.params.idAutor + " - " + extractedAuthor, books: [], error: 'Failed to fetch books' });
  }
});

router.get('/:id', async function(req, res, next) {
  try {
    const response = await axios.get(`http://localhost:17000/books/${req.params.id}`);
    const book = response.data;

    res.render('book', { title: `Livro ${book.title}`, book: book });
  } catch (error) {
    console.error('Error :', error.message);
    res.render('book', { title: 'Livro', books: [], error: 'Failed to fetch books' });
  }
});

module.exports = router;
