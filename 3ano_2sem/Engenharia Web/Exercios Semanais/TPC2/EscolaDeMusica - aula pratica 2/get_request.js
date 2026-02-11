const axios = require('axios')

axios.get('http://localhost:3000/alunos')
    .then(resp => {
        data = resp.data;
        data.forEach(a => {
            console.log(`${a.id},${a.nome},${a.instrumento}`);
        });
    })
    .catch(
        error => {console.log(error);
    });
