const axios = require('axios');

axios.put('http://localhost:3000/instrumentos/123',
    {
        "#text":"Kazoo"
    })
    .then(resp => {
        console.log(resp.data)
    })
    .catch(error => {
        console.log(error);
    });  
