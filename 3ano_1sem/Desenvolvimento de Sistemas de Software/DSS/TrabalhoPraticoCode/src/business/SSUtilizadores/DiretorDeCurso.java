package business.SSUtilizadores;

public class DiretorDeCurso implements Utilizador {
    private String numero;
    private String password;

    public DiretorDeCurso(String numero, String password) {
        this.numero = numero;
        this.password = password;
    }

    @Override
    public String getNumero() {
        return this.numero;
    }

    @Override
    public String getPassword() {
        return this.password;
    }

    @Override
    public String getTipo() {
        return "Diretor de Curso";
    }
}
