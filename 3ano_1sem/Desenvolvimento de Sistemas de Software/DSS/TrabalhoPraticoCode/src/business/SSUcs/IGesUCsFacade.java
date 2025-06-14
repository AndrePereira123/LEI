package business.SSUcs;

import java.util.Collection;

public interface IGesUCsFacade {

    void importarUCsDeCSV();

    void importarTurnosDeCSV();

    boolean existeTurno(String idTurno);

    boolean existeCurso(String num);

    Collection<String> getTurnos();

    Turno procuraTurno(String idTurno);

    Collection<String> getUcs();

    UC procuraUC(String idUC);

    void putTurno(String chave, Turno turno);

    void diminuirTurno(String numTurno);

    void aumentarTurno(String numTurno);
}
