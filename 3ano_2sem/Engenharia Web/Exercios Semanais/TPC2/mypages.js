// mypages.js

export function genMainPage(data) {
    var pagHTML = `
    <!DOCTYPE html>
    <html>
        <head>
                <style>
                    body {
                        background-color: #121212;
                        color: #ffffff;
                        text-align:center;
                    }
                     .w3-container {
            flex: 1; /* Makes the container take full height */
            display: flex;
            flex-direction: column;
        }

        .w3-ul {
            flex: 1; /* Makes the <ul> take up all available space */
            display: flex;
            flex-direction: column;
            justify-content: space-evenly; /* Distributes items evenly */
            align-items: center; /* Centers items horizontally */
            width: 100%;
            padding: 0;
            margin: 0;
        }

        .w3-ul li {
            width: 100%;
            text-align: center;
            padding: 100px 0; /* Adjusts spacing */
        }

        .w3-ul li a {
            text-decoration: none;
            color: white;
            display: block;
            width: 100%;
            padding: 15px;
            background-color: #333333; /* Custom purple */
            border-radius: 8px;
            font-size: 2rem;
        }

        .w3-ul li a:hover {
            background-color: #9030AB;
        }
                </style>
            <meta charset="UTF-8">
            <title>Oficina Automóvel</title>        
            <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css"> 
        </head>
        <body>
            <div class="w3-card-4">
                <header class="w3-container w3-purple">
                    <h1>Consultas</h1>
                </header>

                <div class="w3-container">
                    <ul class="w3-ul" style="border:none;font-size:2rem;">
                        <li style="border:none;">
                            <a href="/alunos">Lista de alunos</a>
                        </li>

                        <li style="border:none;">
                            <a href="/cursos">Lista de Cursos</a>
                        </li>

                        <li style="border:none;">
                            <a href="/instrumentos">Lista de Instrumentos</a>
                        </li>
                    </ul>
                </div>

                <footer class="w3-container w3-purple" style = "position:fixed;text-align:center;width:100%;bottom:0;">
                    <h5>Generated in EngWeb2025 ${data}</h5>
                </footer>
            </div>
        </body>
    </html>  
    `

    return pagHTML
}




export function genAlunosPage(alunos,id_aluno, data) {
    var pagHTML = `
    <!DOCTYPE html>
    <html>
        <head>
                
            <meta charset="UTF-8">
            <title>Alunos</title>        
            <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css"> 
            <style>
        /* Default Dark Mode Styles */
        body {
            background-color: #1e1e1e; /* Dark background */
            color: #ffffff; /* Light text */
        }
        
        

        .w3-card-4 {
            background-color: #1e1e1e; /* Dark card background */
        }

        .w3-container.w3-purple {
            text-align:center;
            background-color: #9030AB !important; /* Darker purple for contrast */
        }

        /* Table Dark Theme */
        .w3-table-all {
            background-color: #1e1e1e; /* Table background */
            color: white; /* Text color */
            border: none;
        }

        .w3-table-all th {
            background-color: dark-red; /* Darker headers */
            color: yellow;
            border: none;
        }

        .w3-table-all tr:nth-child(even) {
            background-color: #333333; /* Alternating row color */
            border: none;
            font-size:1rem;
        }

        .w3-table-all tr:nth-child(odd) {
            background-color: #1e1e1e;
            border: none;
            font-size:1rem;
        }

        .w3-table-all tr:hover
        {
            color:yellow;
        }

        /* Dark Mode Toggle Button */
        .toggle-btn {
            margin: 10px;
            padding: 10px 15px;
            background-color: #6200ea;
            color: white;
            border: none;
            cursor: pointer;
        }

        .butao_voltar h1:hover 
        {
           background-color:#9030AB !important;
        }
    </style>
        </head>
        <body >
                

                        `
    if (id_aluno == null)
    {
        pagHTML += `
        
            <div class="w3-card-4"  style="text-align:center;">
                <header class="w3-container w3-purple" style="display: flex; align-items: center; justify-content: space-between; padding: 10px;">
                    <a href="/" style="text-decoration: none; font-size: 1.4rem;">Início</a>
                    <h1 style="margin: 0; flex-grow: 1; text-align: center;">Lista de Alunos</h1>
                </header>


                <div class="w3-container">
                    <table class="w3-table-all" scrollable>
                        <tr>
                            <th>Id</th>
                            <th>Nome</th>
                            <th>Data_nas.</th>
                            <th>Curso</th>
                            <th>Ano</th>
                            <th>Instrumento</th>
                        </tr>`
        alunos.forEach(aluno => {
            pagHTML += `
            <tr>
                <td>${aluno.id}</td>
                <td> <a href = "/alunos/${aluno.id}">${aluno.nome}</td>
                <td>${aluno.dataNasc}</td>
                <td>${aluno.curso}</td>
                <td>${aluno.anoCurso}</td>
                <td>${aluno.instrumento}</td>
            </tr>
            `
        });
    }
    else
    {
        pagHTML += `
        
            <div class="w3-card-4" >

            <header class="w3-container w3-purple" style="display: flex; align-items: center; justify-content: space-between; padding: 10px;">
                    <a href="/" style="text-decoration: none; font-size: 1.4rem;">Início</a>
                        <h1 style="margin: 0; flex-grow: 1; text-align: center;">Detalhes de Aluno - ${alunos.id}</h1>
                </header>

            <div class="w3-container" >
                    <ul class="w3-ul" style="border:none;font-size:2rem;text-align:center">
                        <li style="border:none;">
                            <a><strong>Nome:</strong> ${alunos.nome}</a>
                        </li>
                         <li style="border:none;background-color:#333333;">
                            <a><strong>Data de nascimento:</strong> ${alunos.dataNasc}</a>
                        </li>
                         <li style="border:none;">
                            <a><strong>Curso</strong> ${alunos.curso} - ${alunos.anoCurso}º <strong>ano</strong></a>
                        </li>
                         <li style="border:none;background-color:#333333;">
                            <a><strong>Instrumento:</strong> ${alunos.instrumento}</a>
                        </li>
                    </ul>
                </div>
            `
        ;
    }

    pagHTML +=
                        `
                    </table>
                </div>
                <div class="butao_voltar" style="display: flex; justify-content: center; align-items: center;">
                        <h1 onclick="window.history.back()" 
                            style="font-size: 1.3rem; padding: 15px 30px; background-color: #333333; 
                                width: 50%; color: white; border: none; border-radius: 10px; 
                                cursor: pointer; text-align: center;">
                            Voltar
                        </h1>
                    </div>

            </div>
        </body>
    </html>  
    `

    return pagHTML
}


export function genCursosPage(cursos,id_curso,alunos) {
    var pagHTML = `
    <!DOCTYPE html>
    <html>
        <head>
                
            <meta charset="UTF-8">
            <title>Cursos</title>        
            <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css"> 
            <style>
        /* Default Dark Mode Styles */
        body {
            background-color: #1e1e1e; /* Dark background */
            color: #ffffff; /* Light text */
        }
        
        

        .w3-card-4 {
            background-color: #1e1e1e; /* Dark card background */
        }

        .w3-container.w3-purple {
            text-align:center;
            background-color: #9030AB !important; /* Darker purple for contrast */
        }

        /* Table Dark Theme */
        .w3-table-all {
            background-color: #1e1e1e; /* Table background */
            color: white; /* Text color */
            border: none;
        }

        .w3-table-all th {
            background-color: dark-red; /* Darker headers */
            color: yellow;
            border: none;
        }

        .w3-table-all tr:nth-child(even) {
            background-color: #333333; /* Alternating row color */
            border: none;
            font-size:1.1rem;
        }

        .w3-table-all tr:nth-child(odd) {
            background-color: #1e1e1e;
            border: none;
            font-size:1.1rem;
        }

        .w3-table-all tr:hover
        {
            color:yellow;
        }

        /* Dark Mode Toggle Button */
        .toggle-btn {
            margin: 10px;
            padding: 10px 15px;
            background-color: #6200ea;
            color: white;
            border: none;
            cursor: pointer;
        }

        .butao_voltar h1:hover 
        {
           background-color:#9030AB !important;
        }
    </style>
        </head>
        <body >
                

                        `
    if (id_curso == null)
    {
        pagHTML += `
        
            <div class="w3-card-4"  style="text-align:center;">
                <header class="w3-container w3-purple" style="display: flex; align-items: center; justify-content: space-between; padding: 10px;">
                    <a href="/" style="text-decoration: none; font-size: 1.4rem;">Início</a>
                    <h1 style="margin: 0; flex-grow: 1; text-align: center;">Lista de Cursos</h1>
                </header>


                <div class="w3-container">
                    <table class="w3-table-all" scrollable>
                        <tr>
                            <th>Id</th>
                            <th>Designação</th>
                            <th>Duração</th>
                            <th>Instrumento</th>
                        </tr>`
        cursos.forEach(curso => {
            pagHTML += `
            <tr>
                <td>${curso.id}</td>
                <td><a href="/cursos/${curso.id}">${curso.designacao}</a></td>
                <td>${curso.duracao}</td>
                <td>${curso.instrumento["#text"]}</td>
            </tr>
            `
        });
    }
    else
    {
        pagHTML += `
        
            <div class="w3-card-4"  style="text-align:center;">
                <header class="w3-container w3-purple" style="display: flex; align-items: center; justify-content: space-between; padding: 10px;">
                    <a href="/" style="text-decoration: none; font-size: 1.4rem;">Início</a>
                    <h1 style="margin: 0; flex-grow: 1; text-align: center;">Alunos do ${cursos.designacao} (${cursos.id})</h1>
                </header>


                <div class="w3-container">
                    <table class="w3-table-all" scrollable>
                        <tr>
                            <th>Id</th>
                            <th>Nome</th>
                            <th>Data_nas.</th>
                            <th>Curso</th>
                            <th>Ano</th>
                            <th>Instrumento</th>
                        </tr>`
        alunos.forEach(aluno => {
            pagHTML += `
            <tr>
                <td>${aluno.id}</td>
                <td> <a href = "/alunos/${aluno.id}">${aluno.nome}</td>
                <td>${aluno.dataNasc}</td>
                <td>${aluno.curso}</td>
                <td>${aluno.anoCurso}</td>
                <td>${aluno.instrumento}</td>
            </tr>
            `
        });
    }

    pagHTML +=
                        `
                    </table>
                </div>
                <div class="butao_voltar" style="display: flex; justify-content: center; align-items: center;">
                        <h1 onclick="window.history.back()" 
                            style="font-size: 1.3rem; padding: 15px 30px; background-color: #333333; 
                                width: 50%; color: white; border: none; border-radius: 10px; 
                                cursor: pointer; text-align: center;">
                            Voltar
                        </h1>
                    </div>

            </div>
        </body>
    </html>  
    `

    return pagHTML
}

export function genInstrumentosPage(instrumentos,id_instrumento,alunos) {
    var pagHTML = `
    <!DOCTYPE html>
    <html>
        <head>
                
            <meta charset="UTF-8">
            <title>Cursos</title>        
            <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css"> 
            <style>
        /* Default Dark Mode Styles */
        body {
            background-color: #1e1e1e; /* Dark background */
            color: #ffffff; /* Light text */
        }
        
        

        .w3-card-4 {
            background-color: #1e1e1e; /* Dark card background */
        }

        .w3-container.w3-purple {
            text-align:center;
            background-color: #9030AB !important; /* Darker purple for contrast */
        }

        /* Table Dark Theme */
        .w3-table-all {
            background-color: #1e1e1e; /* Table background */
            color: white; /* Text color */
            border: none;
        }

        .w3-table-all th {
            background-color: dark-red; /* Darker headers */
            color: yellow;
            border: none;
        }

        .w3-table-all tr:nth-child(even) {
            background-color: #333333; /* Alternating row color */
            border: none;
            font-size:1.1rem;
        }

        .w3-table-all tr:nth-child(odd) {
            background-color: #1e1e1e;
            border: none;
            font-size:1.1rem;
        }

        .w3-table-all tr:hover
        {
            color:yellow;
        }

        /* Dark Mode Toggle Button */
        .toggle-btn {
            margin: 10px;
            padding: 10px 15px;
            background-color: #6200ea;
            color: white;
            border: none;
            cursor: pointer;
        }

        .butao_voltar h1:hover 
        {
           background-color:#9030AB !important;
        }
    </style>
        </head>
        <body >
                

                        `
    if (id_instrumento == null)
    {
        pagHTML += `
        
            <div class="w3-card-4"  style="text-align:center;">
                <header class="w3-container w3-purple" style="display: flex; align-items: center; justify-content: space-between; padding: 10px;">
                    <a href="/" style="text-decoration: none; font-size: 1.4rem;">Início</a>
                    <h1 style="margin: 0; flex-grow: 1; text-align: center;">Lista de Instrumentos</h1>
                </header>


                <div class="w3-container">
                    <table class="w3-table-all" scrollable>
                        <tr>
                            <th>Id</th>
                            <th>Nome</th>
                        </tr>`
        instrumentos.forEach(instrumento => {
            pagHTML += `
            <tr>
                <td>${instrumento.id}</td>
                <td><a href="/instrumentos/${instrumento.id}">${instrumento["#text"]}</a></td>
            </tr>
            `
        });
    }
    else
    {
        pagHTML += `
        
            <div class="w3-card-4"  style="text-align:center;">
                <header class="w3-container w3-purple" style="display: flex; align-items: center; justify-content: space-between; padding: 10px;">
                    <a href="/" style="text-decoration: none; font-size: 1.4rem;">Início</a>
                    <h1 style="margin: 0; flex-grow: 1; text-align: center;">Alunos que tocam ${instrumentos["#text"]}</h1>
                </header>


                <div class="w3-container">
                    <table class="w3-table-all" scrollable>
                        <tr>
                            <th>Id</th>
                            <th>Nome</th>
                            <th>Data_nas.</th>
                            <th>Curso</th>
                            <th>Ano</th>
                            <th>Instrumento</th>
                        </tr>`
        alunos.forEach(aluno => {
            pagHTML += `
            <tr>
                <td>${aluno.id}</td>
                <td> <a href = "/alunos/${aluno.id}">${aluno.nome}</td>
                <td>${aluno.dataNasc}</td>
                <td>${aluno.curso}</td>
                <td>${aluno.anoCurso}</td>
                <td>${aluno.instrumento}</td>
            </tr>
            `
        });
    }

    pagHTML +=
                        `
                    </table>
                </div>
                <div class="butao_voltar" style="display: flex; justify-content: center; align-items: center;">
                        <h1 onclick="window.history.back()" 
                            style="font-size: 1.3rem; padding: 15px 30px; background-color: #333333; 
                                width: 50%; color: white; border: none; border-radius: 10px; 
                                cursor: pointer; text-align: center;">
                            Voltar
                        </h1>
                    </div>

            </div>
        </body>
    </html>  
    `

    return pagHTML
}