package business.SSAlunos;

import java.util.Collection;

public interface IGesAlunosFacade {

    void importarAlunosDeCSV();

    boolean existeAluno(String num);

    boolean adicionaAluno(String num, String nome, String email, String estatuto, String cod_curso);

    boolean adicionaAluno(Aluno aluno);

    Collection<Aluno> getAlunos();

    Collection<String> getAlunosChave();

    Aluno procuraAluno(String num);

    boolean removeTurnoManual(String numAluno, String idTurno);

    boolean adicionaTurnoManual(String numAluno, String idTurno);
}
