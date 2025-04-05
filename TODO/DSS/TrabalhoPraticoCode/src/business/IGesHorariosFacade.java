package business;

import business.SSAlunos.Aluno;
import business.SSUcs.Curso;

import java.util.Collection;

public interface IGesHorariosFacade{
    boolean existeAluno(String num);

    boolean adicionaAluno(String num, String nome, String email, String estatuto, String cod_curso);

    Collection<Aluno> getAlunos();

    Aluno procuraAluno(String num);

    boolean existeCurso(String num);

    boolean atribuiAlunoManual(String num, String id);

    boolean existeTurno(String id);

    boolean removeAlunoManual(String num, String id);

    String listaHorarioAluno(String num);

    void importarUCsDeCSV();

    void importarTurnosDeCSV();

    void importarAlunosDeCSV();

    void putDiretorDeUc();
}
