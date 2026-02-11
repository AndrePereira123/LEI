const http = require('http')
const axios = require('axios')

http.createServer((req, res) => {
    let body_html = `
        <html>
        <head>
            <style>
                body {
                    background-color: #121212;
                    color: #ffffff;
                    font-family: Arial, sans-serif;
                    text-align: left;
                    font-size: 1.2rem;
                    padding: 20px;
                    padding-left: 20%;
                }
                
                h1 {
                    text-align: center;
                }

                li {
                    margin-bottom: 10px;
                }

                li a {
                    text-decoration: none;
                    color: lightblue;
                    font-size: 18px;
                    transition: color 0.3s; /* Smooth color transition */
                }

                li a:hover {
                    color: orange; /* Changes to orange when hovered */
                }

                .sidenav {
                    height: 100%;
                    width: 160px;
                    position: fixed;
                    z-index: 1;
                    top: 0;
                    left: 0;
                    background-color: #111;
                    overflow-x: hidden;
                    padding-top: 20px;
                }

                .sidenav a {
                    padding: 10px 15px;
                    text-decoration: none;
                    font-size: 18px;
                    color: #818181;
                    display: block;
                }

                .sidenav a:hover {
                    color: #f1f1f1;
                }
            </style>
        </head>
        <body>
            <div class="sidenav">
                <a href="/reparacoes">Reparações</a>
                <a href="/viaturas">Viaturas</a>
                <a href="/intervencoes">Intervenções</a>
            </div>
        </body>
        </html>
    `;

    console.log("Method: " + req.method);
    console.log("URL: " + req.url);

    switch (req.method) {
        case "GET":
            res.writeHead(200, { 'Content-Type': 'text/html;charset=utf-8' });
            res.write(body_html); // Send sidebar and base HTML

            if (req.url === "/reparacoes") {
                axios.get("http://localhost:3000/reparacoes?_sort=nome")
                    .then(resp => {
                        var reparacoes = resp.data;
                        res.write(`
                            <h1>Reparações</h1>
                            <hr style="border: 1px solid white; margin: 10px 0;">
                            <div style="text-align: center; margin-bottom: 20px;">
                                ${'ABCDEFGHIJKLMNOPQRSTUVWXYZ'.split('').map(letter => 
                                    `<a href="/reparacoes?letter=${letter}" style="margin: 0 5px; text-decoration: none; color: lightblue; font-size: 18px;">${letter}</a>`
                                ).join(' - ')}
                            </div>
                        `);
                        reparacoes.forEach(element => {
                            res.write(`<li>
                                <a href='/reparacoes/${element.nif}'> ${element.nome} - ${element.viatura.marca} ${element.viatura.modelo}</a>
                               </li>`);
                            }
                        );
                        res.end();
                    })
                    .catch(error => {
                        res.writeHead(501, { 'Content-Type': 'text/html;charset=utf-8' });
                        res.write(error.toString());
                        res.end();
                    });
            } 
            else if (req.url === "/viaturas") 
            {   
                axios.get("http://localhost:3000/reparacoes")
                .then(resp => {
                    var reparacoes = resp.data;
                    const viaturas = reparacoes.sort((a, b) => a.viatura.marca.localeCompare(b.viatura.marca));
                    res.write(`
                        <h1>Viaturas</h1>
                        <hr style="border: 1px solid white; margin: 10px 0;">
                        <div style="text-align: center; margin-bottom: 20px;">
                            ${'ABCDEFGHIJKLMNOPQRSTUVWXYZ'.split('').map(letter => 
                                `<a href="/viaturas?letter=${letter}" style="margin: 0 5px; text-decoration: none; color: lightblue; font-size: 18px;">${letter}</a>`
                            ).join(' - ')}
                        </div>
                    `);

                    viaturas.forEach(element => {
                        res.write(`<li>
                            <a href='/reparacoes/${element.nif}'> ${element.viatura.marca} - ${element.viatura.modelo} - ${element.viatura.matricula}</a>
                            </li>`);
                        }
                    );
                        res.end();
                    })
                    .catch(error => {
                        res.writeHead(501, { 'Content-Type': 'text/html;charset=utf-8' });
                        res.write(error.toString());
                        res.end();
                    });
            }
            else if (req.url === "/intervencoes")
            {
                axios.get("http://localhost:3000/reparacoes")
                .then(resp => {
                    var reparacoes = resp.data;
                    const intervencoes = reparacoes
                    .sort((a, b) => a.nome.localeCompare(b.nome));

                    res.write(`
                        <h1>Intervenções</h1>
                        <hr style="border: 1px solid white; margin: 10px 0;">
                        <div style="text-align: center; margin-bottom: 20px;">
                            ${'ABCDEFGHIJKLMNOPQRSTUVWXYZ'.split('').map(letter => 
                                `<a href="/intervencoes?letter=${letter}" style="margin: 0 5px; text-decoration: none; color: lightblue; font-size: 18px;">${letter}</a>`
                            ).join(' - ')}
                        </div>
                    `);

                    intervencoes.forEach(element => {
                        res.write(`<li>
                            <a href='/reparacoes/${element.nif}'> ${element.nome}</a>  
                            <ul>`); // Start a sublist for interventions
                        
                        element.intervencoes.forEach(interv => {
                            res.write(`
                                <li>
                                    <strong>Cod:</strong> ${interv.codigo} - 
                                    <strong>Nome:</strong> ${interv.nome}
                                </li>
                            `);
                        });
                        
                        res.write(`</ul></li>`); // Close the sublist and main list item
                        
                        }
                    );
                        res.end();
                    })
                    .catch(error => {
                        res.writeHead(501, { 'Content-Type': 'text/html;charset=utf-8' });
                        res.write(error.toString());
                        res.end();
                    });
            }
            else if (req.url.match(/\/intervencoes\?letter=./))
                {
                    var letra = req.url.split("=")[1];
                    res.write(`<h1>${letra}</h1>`);
                    axios.get(`http://localhost:3000/reparacoes?nome_like=^${letra}`)
                        .then(resp => {
                            var reparacoes = resp.data;
                            reparacoes.forEach(element => {
                            res.write(`<li>
                                <a href='/reparacoes/${element.nif}'> ${element.nome}</a>  
                                <ul>`); // Start a sublist for interventions
                            
                            element.intervencoes.forEach(interv => {
                                res.write(`
                                    <li>
                                        <strong>Cod:</strong> ${interv.codigo} - 
                                        <strong>Nome:</strong> ${interv.nome}
                                    </li>
                                `);
                        });
                        
                        res.write(`</ul></li>`); // Close the sublist and main list item
                        
                        }
                            );
                            res.end();
                        })
                }
            else if (req.url.match(/\/reparacoes\?letter=./))
            {
                var letra = req.url.split("=")[1];
                res.write(`<h1>${letra}</h1>`);
                axios.get(`http://localhost:3000/reparacoes?nome_like=^${letra}`)
                    .then(resp => {
                        var reparacoes = resp.data;
                        reparacoes.forEach(element => {
                            res.write(`<li>
                                <a href='/reparacoes/${element.nif}'> ${element.nome} - ${element.viatura.marca} ${element.viatura.modelo}</a>
                               </li>`);
                            }
                        );
                        res.end();
                    })
            }
            else if (req.url.match(/\/viaturas\?letter=./))
                {
                    var letra = req.url.split("=")[1];
                    res.write(`<h1>${letra}</h1>`);
                    axios.get(`http://localhost:3000/reparacoes`)
                        .then(resp => {
                            var reparacoes = resp.data;
                            const viaturas = reparacoes.filter(rep => rep.viatura.marca.startsWith(letra)).sort((a, b) => a.viatura.marca.localeCompare(b.viatura.marca));
                            
                            const marcasUnicas = [...new Set(viaturas.map(v => v.viatura.marca))];
                            res.write(`<p style="font-weight: bold; margin-bottom: 10px;text-align:center;">${marcasUnicas.join(" | ")}</p>`);

            
                            viaturas.forEach(element => {
                                res.write(`<li>
                                    <a href='/reparacoes/${element.nif}'> ${element.viatura.marca} - ${element.viatura.modelo} ${element.viatura.matricula}</a>
                                   </li>`);
                                }
                            );
                            res.end();
                        })
                }
            else if (req.url.match(/\/reparacoes\/.+/)) {
                var nif = req.url.split("/")[2];
                axios.get(`http://localhost:3000/reparacoes?nif=${nif}`)
                    .then(resp => {
                        var reparacao = resp.data[0];
                        res.write(`<h1>${reparacao.nome}</h1>`);
                        res.write(`<pre>${JSON.stringify(reparacao, null, 2)}</pre>`);
                        res.write("<h1 onclick='window.history.back()'style='font-size: 20px; padding: 15px 30px; background-color: darkgray;width:50%; color: white; border: none; border-radius: 10px; cursor: pointer;'> Voltar </h1>");
                        res.end();
                    })
                    .catch(error => {
                        res.writeHead(501, { 'Content-Type': 'text/html;charset=utf-8' });
                        res.write(error.toString());
                        res.end();
                    });
            } 
            else
            {
                res.write("<h1>Erro 404 - Esta página não está atribuida</h1>");
                res.end();
            }
            break;
        default:
            break;
    }
}).listen(12000);

console.log("Servidor iniciado na porta 12000");
