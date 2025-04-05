package business.SSUtilizadores;

public interface IGesUtilizadoresFacade {
    void putDiretorDeUc();

    void importarUtilizadoresDeCSV();

    boolean verificaLogin(String usuario, String senha);

    Utilizador procuraUtilizador(String numero);
}
