package business.SSAlunos;

import data.AlunoDAO;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collection;

public class GesAlunosFacade implements IGesAlunosFacade {
    private AlunoDAO alunos;

    public GesAlunosFacade() {
        this.alunos = AlunoDAO.getInstance();
    }

    // ---------------------------------------------------------------------------------------
    @Override
    // pedir a pasta ao utilizador no TextUI, e dps chamar esta funcao com a pasta como argumento
    public void importarAlunosDeCSV() {
        String caminhoRelativo = "/inputs/alunos.csv";
        String caminhoCSV = Paths.get(System.getProperty("user.dir"), caminhoRelativo).normalize().toString();

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoCSV))) {
            br.readLine();
            String linha;
            while ((linha = br.readLine()) != null) {
                processarLinhaAlunosCSV(linha);
            }
            System.out.println("Ficheiro CSV de Alunos importado com sucesso!");
        } catch (IOException e) {
            e.printStackTrace();
            throw new RuntimeException("Erro ao ler o ficheiro CSV: " + e.getMessage());
        }
    }

    private void processarLinhaAlunosCSV(String linha) {
        String[] dados = linha.split(";");

        obterOuCriarAluno(dados);
    }

    private void obterOuCriarAluno(String[] dados) {
        String num = dados[2];
        String nome = dados[3];
        String email = dados[4];
        String estatuto = dados.length == 6 ? dados[5] : "";
        String codCurso = dados[0];

        Aluno aluno = this.alunos.get(num);
        if (aluno == null) {
            aluno = new Aluno(num, nome, email, estatuto, codCurso);
        }
        aluno.addUC(dados[1]);

        this.alunos.put(num, aluno);
    }
    // ---------------------------------------------------------------------------------------

    @Override
    public boolean existeAluno(String num) {
        return this.alunos.containsKey(num);
    }

    @Override
    public boolean adicionaAluno(String num, String nome, String email, String estatuto, String cod_curso) {
        this.alunos.put(num, new Aluno(num, nome, email, estatuto, cod_curso));
        return true;
    }

    public boolean adicionaAluno(Aluno aluno) {
        this.alunos.put(aluno.getNumero(), aluno);
        return true;
    }

    @Override
    public Collection<Aluno> getAlunos() {
        return new ArrayList<>(this.alunos.values());
    }

    public Collection<String> getAlunosChave() { return new ArrayList<>(this.alunos.keySet()); }


    public boolean removeTurnoManual(String numAluno, String idTurno) {
        Aluno al = alunos.get(numAluno);
        if (al.removeTurnoManual(idTurno)) {
            this.alunos.put(numAluno, al);
            return true;
        } else {
            return false;
        }
    }

    public boolean adicionaTurnoManual(String numAluno, String idTurno){
        Aluno al = alunos.get(numAluno);
        if(al.adicionaTurnoManual(idTurno)) {
            this.alunos.put(numAluno, al);
            return true;
        } else {
            return false;
        }
    }

    @Override
    public Aluno procuraAluno(String num) {
        return this.alunos.get(num);
    }

}
