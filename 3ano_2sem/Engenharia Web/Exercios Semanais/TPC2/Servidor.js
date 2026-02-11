import { createServer } from 'http'
import axios from 'axios'
import { genMainPage, genAlunosPage, genCursosPage, genInstrumentosPage } from './mypages.js'
import { readFile } from 'fs'


createServer(function(req, res) {
    var d = new Date().toISOString().substring(0,16)
    console.log(req.method + " " + req.url + " " + d)

    if(req.url === '/') {
        res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
        res.write(genMainPage(d))
        res.end()
    }
    else if(req.url === '/alunos') {
        axios.get('http://localhost:3000/alunos')
          .then(function(alunoss){
            var alunos = alunoss.data
            res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
            res.write(genAlunosPage(alunos,null,d))
            res.end()
          })
          .catch(erro => {
            console.log("Erro " + erro)
            res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
            res.end('<p>Operação na obtenção de dados: '+ erro + '</p>')
          })
        }
    else if(req.url.match(/alunos\/[A]/)) {
        var id_aluno = req.url.split("/")[2];
        axios.get(`http://localhost:3000/alunos?id=${id_aluno}`)
        .then(function(alunoss){
            var alunos = alunoss.data
            if (alunos.length >= 1){
                res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
                res.write(genAlunosPage(alunos[0],id_aluno,d))
                res.end()
            }
            else
            {
                res.writeHead(404, { 'Content-Type': 'text/html;charset=utf-8' });
                res.end('<p>Erro: Aluno não encontrado</p>');
            }
        })
        .catch(erro => {
            console.log("Erro " + erro)
            res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
            res.end('<p>Operação na obtenção de dados: '+ erro + '</p>')
        })
        }
    else if(req.url === '/cursos') {
    axios.get('http://localhost:3000/cursos')
        .then(function(cursoss){
        var cursos = cursoss.data
        res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
        res.write(genCursosPage(cursos,null,null))
        res.end()
        })
        .catch(erro => {
        console.log("Erro " + erro)
        res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
        res.end('<p>Operação na obtenção de dados: '+ erro + '</p>')
        })
    }
    else if(req.url.match(/cursos\/[C]/)) {
        var id_curso = req.url.split("/")[2];
        axios.get(`http://localhost:3000/cursos?id=${id_curso}`)
        .then(function(cursoss){
            var cursos = cursoss.data;
            if (cursos.length >= 1){
                axios.get(`http://localhost:3000/alunos?curso=${id_curso}`)
                .then(function(alunoss){
                    var alunos = alunoss.data;
                    res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
                    res.write(genCursosPage(cursos[0],id_curso,alunos))
                    res.end()})
            }
            else
            {
                res.writeHead(404, { 'Content-Type': 'text/html;charset=utf-8' });
                res.end('<p>Erro: Aluno não encontrado</p>');
            }
        })
        .catch(erro => {
            console.log("Erro " + erro)
            res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
            res.end('<p>Operação na obtenção de dados: '+ erro + '</p>')
        })
        }
        else if(req.url === '/instrumentos') {
            axios.get('http://localhost:3000/instrumentos')
                .then(function(instrumentoss){
                var instrumentos = instrumentoss.data
                res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
                res.write(genInstrumentosPage(instrumentos,null,null))
                res.end()
                })
                .catch(erro => {
                console.log("Erro " + erro)
                res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
                res.end('<p>Operação na obtenção de dados: '+ erro + '</p>')
                })
            }
            else if(req.url.match(/instrumentos\/[I]/)) {
                var id_instrumento = req.url.split("/")[2];
                axios.get(`http://localhost:3000/instrumentos?id=${id_instrumento}`)
                .then(function(instrumentos){
                    var instrumento = instrumentos.data;
                    if (instrumento.length >= 1){
                        axios.get(`http://localhost:3000/alunos?instrumento=${instrumento[0]["#text"]}`)
                        .then(function(alunoss){
                            var alunos = alunoss.data;
                            res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
                            res.write(genInstrumentosPage(instrumento[0],id_instrumento,alunos))
                            res.end()})
                    }
                    else
                    {
                        res.writeHead(404, { 'Content-Type': 'text/html;charset=utf-8' });
                        res.end('<p>Erro: Aluno não encontrado</p>');
                    }
                })
                .catch(erro => {
                    console.log("Erro " + erro)
                    res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
                    res.end('<p>Operação na obtenção de dados: '+ erro + '</p>')
                })
    }
    else {
        res.writeHead(200, {'Content-Type': 'text/html;charset=utf-8'})
        res.end('<p>Operação não suportada: '+ req.url + '</p>')
    }

}).listen(12000)

