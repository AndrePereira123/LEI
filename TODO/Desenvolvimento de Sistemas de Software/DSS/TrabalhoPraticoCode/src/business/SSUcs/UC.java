package business.SSUcs;

public class UC {
    private String curso;
    private String codigo;
    private String nome;
    private int ano;

    public UC(String codigo, String nome, int ano, String curso) {
        this.codigo = codigo;
        this.nome = nome;
        this.ano = ano;
        this.curso = curso;
    }

    public String getCodigo() {
        return codigo;
    }

    public String getNome() {
        return nome;
    }

    public int getAno() {
        return ano;
    }

    public String getCurso() { return curso; }

    @Override
    public String toString() {
        return "UC{"+this.codigo+", "+this.nome+", "+this.ano+'}';
    }
}
