const http = require('http')
const axios = require('axios')
const meta = require('./aux')

http.createServer((req,res) => {
    body_html = `
                        <html>
                        <head>
                            <style>
                                body {
                                    background-color: #121212;
                                    color: #ffffff;
                                    font-family: Arial, sans-serif;
                                    text-align: center;
                                    padding: 20px;
                                }
                            </style>
                        </head>
                    `
    console.log("Method: " + req.method)
    console.log("URL: " + req.url)
    
    switch(req.method)
    {
        case "GET":
            {
            if(req.url === "/cidades"){axios.get("http://localhost:3000/cidades?_sort=nome")
                      .then(resp => {
                        var cidades = resp.data
                        res.writeHead(200,{'Content-Type' : 'text/html;charset=utf-8'})
                        res.write("<h1>Cidades</h1>")
                        res.write("<ul>")
                        cidades.forEach(element => {
                            res.write(`<li><a href = '/cidades/${element.id}'>${element.nome}</li>`) //formated string `` equivalente a f{}
                            //res.write(`<li>${element.di}`)
                        });
                        res.write("</ul>")
                        res.end()
                      })
                      .catch(erro => 
                        {
                        res.writeHead(501,{'Content-Type' : 'text/html;charset=utf-8'})
                        res.write(erro)
                        res.end()
                        })}
            else if (req.url.match(/\/cidades\/.+/)){
                var id = req.url.split("/")[2]
                axios.get(`http://localhost:3000/cidades/${id}`)
                    .then(resp => {
                      var cidade = resp.data
                      res.writeHead(200,{'Content-Type' : 'text/html;charset=utf-8'})
                      res.write(`<h1>${cidade.nome}</h1>`)
                      
                      res.write(`<pre>${JSON.stringify(cidade)}</pre>`)
                      res.write("<h6><a href ='/cidades'>Voltar</h6>")
                      res.end()
                    })
                    .catch(erro => 
                      {
                      res.writeHead(501,{'Content-Type' : 'text/html;charset=utf-8'})
                      res.write(erro)
                      res.end()
                      })}


                else if(req.url == "/ligacoes"){
                    res.writeHead(501,{'Content-Type' : 'text/html;charset=utf-8'})
                    res.end()}
                else
                {
                    res.writeHead(404,{'Content-Type' : 'text/html;charset=utf-8'})
                    res.end()
                }
                break;
            }
        
        default: 
            res.writeHead(405,{'Content-Type' : 'text/html;charset=utf-8'})
            res.end()
            break;
    }
    
}).listen(1234)

console.log("Servidor à escuta na porta 1234...")



