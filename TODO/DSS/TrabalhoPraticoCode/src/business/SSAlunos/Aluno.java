package business.SSAlunos;

import business.SSUtilizadores.Utilizador;

import java.util.*;

public class Aluno implements Utilizador {
    private String numero;
    private String nome;
    private String email;
    private String password;
    private String estatuto;
    private String curso;
    private Set<String> ucs;
    private List<String> horario;

    public Aluno(String numero, String nome, String email, String estatuto, String curso) {
        this.numero = numero;
        this.nome = nome;
        this.email = email;
        this.estatuto = estatuto;
        this.curso = curso;
        this.ucs = new HashSet<>();
        this.horario = new ArrayList<>();
    }

    public Aluno(String numero, String password) {
        this.numero = numero;
        this.password = password;
    }

    public String getNumero() {
        return this.numero;
    }

    public String getNome() {
        return this.nome;
    }

    public String getEmail() {
        return this.email;
    }

    public String getPassword() {
        return this.password;
    }

    @Override
    public String getTipo() {
        return "Aluno";
    }

    public String getEstatuto() {
        return this.estatuto;
    }

    public String getCurso() {
        return this.curso;
    }

    public Set<String> getUcs() {
        return this.ucs;
    }

    public List<String> getHorario() {
        return this.horario;
    }

    public void addUC(String uc){
        this.ucs.add(uc);
    }

    public void setHorario(List<String> horario) {
        this.horario = horario;
    }

    public boolean removeTurnoManual(String idTurno) {
        if (this.horario.contains(idTurno)) {
            this.horario.remove(idTurno);
            return true;
        } else {
            return false;
        }
    }

    public boolean adicionaTurnoManual(String idTurno) {
        if (!this.horario.contains(idTurno)) {
            this.horario.add(idTurno);
            return true;
        } else {
            return false;
        }
    }

    @Override
    public String toString() {
        return "Aluno{"+this.numero+", "+this.nome+", "+this.email+", "+this.estatuto+", "+this.curso+", "+this.ucs+'}'+"\n";
    }
}
