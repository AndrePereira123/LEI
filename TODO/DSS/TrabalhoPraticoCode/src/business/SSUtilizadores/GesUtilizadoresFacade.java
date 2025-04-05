package business.SSUtilizadores;

import business.SSAlunos.Aluno;
import data.UtilizadorDAO;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Paths;

public class GesUtilizadoresFacade implements IGesUtilizadoresFacade {
    private UtilizadorDAO utilizadores;

    public GesUtilizadoresFacade() {
        this.utilizadores = UtilizadorDAO.getInstance();
    }

    //----------------------------------------------------------------------------------
    public void importarUtilizadoresDeCSV() {
        String caminhoRelativo = "/inputs/utilizadores.csv";
        String caminhoCSV = Paths.get(System.getProperty("user.dir"), caminhoRelativo).normalize().toString();

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoCSV))) {
            br.readLine();
            String linha;
            while ((linha = br.readLine()) != null) {
                processarLinhaUtilizadoresCSV(linha);
            }
        } catch (IOException e) {
            e.printStackTrace();
            throw new RuntimeException("Erro ao ler o ficheiro CSV: " + e.getMessage());
        }
    }

    private void processarLinhaUtilizadoresCSV(String linha) {
        String[] dados = linha.split(";");

        criarUtilizador(dados);
    }

    private void criarUtilizador(String[] dados) {
        String num = dados[0];
        String password = dados[1];

        Aluno aluno = new Aluno(num, password);
        this.utilizadores.put(num, aluno);
    }
    //----------------------------------------------------------------------------------

    public void putDiretorDeUc(){
        DiretorDeCurso duc = new DiretorDeCurso("duc1", "pass");
        this.utilizadores.put(duc.getNumero(), duc);
    }

    public boolean verificaLogin(String num, String pass) {
        Utilizador user = this.utilizadores.get(num);
        if (user != null) {
            return user.getPassword().equals(pass);
        }
        return false;
    }

    public Utilizador procuraUtilizador(String numero) {
        return this.utilizadores.get(numero);
    }
}
