package business;

import business.SSAlunos.Aluno;
import business.SSUcs.*;
import business.SSAlunos.GesAlunosFacade;
import business.SSAlunos.IGesAlunosFacade;
import business.SSUtilizadores.GesUtilizadoresFacade;
import business.SSUtilizadores.IGesUtilizadoresFacade;
import business.SSUtilizadores.Utilizador;

import java.util.*;

public class GesHorariosFacade implements IGesHorariosFacade {

    private IGesAlunosFacade alunosFacade;
    private IGesUCsFacade ucsFacade;
    private IGesUtilizadoresFacade utilizadoresFacade;
    private Utilizador currentUser;

    public GesHorariosFacade() {
        this.ucsFacade = new GesUCsFacade();
        this.alunosFacade = new GesAlunosFacade();
        this.utilizadoresFacade = new GesUtilizadoresFacade();
    }

    //----------------------------------------------------------------------------------------

    public void horarioGenerator() {
        Collection<String> turnos = this.ucsFacade.getTurnos();
        Collection<String> turnos_copia = this.ucsFacade.getTurnos();

        Collection<String> alunos = this.alunosFacade.getAlunosChave();

        for (String a_chave : alunos) {
            Aluno a = this.alunosFacade.procuraAluno(a_chave);
            List<Turno> horario = new ArrayList<>();

            for (String t_chave : turnos) {
                Turno t = this.ucsFacade.procuraTurno(t_chave);
                if(turnos_copia.contains(t.getId()))
                {
                    UC uc = this.ucsFacade.procuraUC(t.getCodigoUC());
                    if (a.getUcs().contains(uc.getCodigo())) {
                        long turnosDaUc = horario.stream()
                                .filter(turno -> turno.getCodigoUC().equals(t.getCodigoUC()))
                                .count();

                        boolean temTurnoT = horario.stream()
                                .anyMatch(turno -> turno.getCodigoUC().equals(t.getCodigoUC()) && turno.getType().equals("T"));

                        boolean temMaisDeUmTpOuPl = horario.stream()
                                .anyMatch(turno -> turno.getCodigoUC().equals(t.getCodigoUC()) &&
                                        (turno.getType().equals("TP") || turno.getType().equals("PL")));

                        var turnosDeDia = horario.stream()
                                .filter(turno -> turno.getDiaSemana().equals(t.getDiaSemana()))
                                .toList();

                        boolean horarioLivre = turnosDeDia.stream()
                                .noneMatch(turno -> Float.parseFloat(t.getHoraInicio()) >= Float.parseFloat(turno.getHoraFim())
                                        && Float.parseFloat(t.getHoraFim()) <= Float.parseFloat(turno.getHoraInicio()));

                        if (turnosDaUc < 2 && horarioLivre) {
                            boolean condicao_t = t.getType().equals("T") && temTurnoT;
                            boolean condicao_tp = (t.getType().equals("TP") || t.getType().equals("PL")) && temMaisDeUmTpOuPl;

                            if (!condicao_t && !condicao_tp) {
                                int inscritos = t.getNinscritos();
                                int capacidade = t.getCapacidade();
                                int vagasRestantes = capacidade - inscritos;

                                if (vagasRestantes > 0 && (vagasRestantes > 5 || !a.getEstatuto().isEmpty())) {
                                    horario.add(t);
                                    t.atualizaInscritos(1);
                                    this.ucsFacade.putTurno(t.getId(), t);

                                    if (t.getNinscritos() == capacidade) {
                                        turnos_copia.remove(t.getId());
                                    }
                                }
                            }
                        }
                    }
                }

            }

            List<String> horario2 = new ArrayList<>();
            for (Turno t : horario) {
                horario2.add(t.getId());
            }

            if (a.getUcs().size() * 2 > horario2.size()) {
                List<String> ucsEmFalta = a.getUcs().stream()
                        .filter(uc -> horario.stream()
                                .filter(turno -> turno.getCodigoUC().equals(uc))
                                .count() < 2)
                        .toList();

                for (String uc : ucsEmFalta) {
                    List<Turno> turnosDisponiveis = turnos_copia.stream()
                            .map(chave -> this.ucsFacade.procuraTurno(chave))
                            .filter(t -> t.getCodigoUC().equals(uc))
                            .toList();

                    if (!turnosDisponiveis.isEmpty()) {
                        boolean temTurnoT = horario.stream()
                                .anyMatch(turno -> turno.getCodigoUC().equals(uc) && turno.getType().equals("T"));

                        boolean temMaisDeUmTpOuPl = horario.stream()
                                .anyMatch(turno -> turno.getCodigoUC().equals(uc) &&
                                        (turno.getType().equals("TP") || turno.getType().equals("PL")));

                        Turno disponivel_t = null;
                        Turno disponivel_tp = null;
                        if(temTurnoT) {
                            disponivel_t = turnosDisponiveis.stream()
                                    .filter(t->(t.getType().equals("TP") || t.getType().equals("PL")))
                                    .findFirst()
                                    .orElse(null);
                        }
                        if(temMaisDeUmTpOuPl) {
                            disponivel_tp = turnosDisponiveis.stream()
                                    .filter(t->(t.getType().equals("T")))
                                    .findFirst()
                                    .orElse(null);
                        }

                        if(disponivel_tp != null) {
                            horario2.add(disponivel_tp.getId());
                            disponivel_tp.atualizaInscritos(1);
                            this.ucsFacade.putTurno(disponivel_tp.getId(), disponivel_tp);

                            if (disponivel_tp.getNinscritos() == disponivel_tp.getCapacidade()) {
                                turnos_copia.remove(disponivel_tp.getId());
                            }
                        }
                        if(disponivel_t != null) {
                            horario2.add(disponivel_t.getId());
                            disponivel_t.atualizaInscritos(1);
                            this.ucsFacade.putTurno(disponivel_t.getId(), disponivel_t);

                            if (disponivel_t.getNinscritos() == disponivel_t.getCapacidade()) {
                                turnos_copia.remove(disponivel_t.getId());
                            }
                        }
                    }
                }
            }

            a.setHorario(horario2);
            this.alunosFacade.adicionaAluno(a);
        }
    }
    // --------------------------------------------------------------------------------------------------
    @Override
    public String listaHorarioAluno(String num) {
        Aluno a = this.alunosFacade.procuraAluno(num);

        if (a == null) {
            return "Aluno não encontrado!";
        }

        StringBuilder horario = new StringBuilder();

        Collection<String> turnos = a.getHorario();

        for (String t_chave : turnos) {
            Turno t = this.ucsFacade.procuraTurno(t_chave);
            horario.append("ID: ").append(t.getId()).append(" | ")
                    .append("UC: ").append(t.getCodigoUC()).append(" | ")
                    .append("Dia: ").append(t.getDiaSemana()).append(" | ")
                    .append("Início: ").append(t.getHoraInicio()).append(" | ")
                    .append("Fim: ").append(t.getHoraFim()).append(" | ")
                    .append("Tipo: ").append(t.getType()).append(" | ")
                    .append("Sala: ").append(t.getSala()).append(" | ")
                    .append("inscritos: ").append(t.getNinscritos()).append(" | ")
                    .append("Capacidade: ").append(t.getCapacidade()).append("\n");
        }

        return !horario.isEmpty() ? horario.toString() : "Este aluno não tem turnos atribuídos.";
    }
    // --------------------------------------------------------------------------------------------------


    @Override
    public boolean existeCurso(String num) {
        return this.ucsFacade.existeCurso(num);
    }


    @Override
    public boolean atribuiAlunoManual(String numAluno, String id) {
        if(this.alunosFacade.adicionaTurnoManual(numAluno, id)) {
            this.ucsFacade.aumentarTurno(id);
            return true;
        }
        return false;
    }


    @Override
    public boolean removeAlunoManual(String numAluno, String id) {
        if (this.alunosFacade.removeTurnoManual(numAluno, id)) {
            this.ucsFacade.diminuirTurno(id);
            return true;
        }
        return false;
    }

    @Override
    public boolean existeTurno(String id) {
        return this.ucsFacade.existeTurno(id);
    }

    @Override
    public boolean existeAluno(String num) {
        return this.alunosFacade.existeAluno(num);
    }

    @Override
    public boolean adicionaAluno(String num, String nome, String email, String estatuto, String cod_curso) {
        if(this.existeCurso(cod_curso)) {
            return this.alunosFacade.adicionaAluno(num, nome, email, estatuto, cod_curso);
        }
        return false;
    }

    @Override
    public Collection<Aluno> getAlunos() {
        return this.alunosFacade.getAlunos();
    }

    @Override
    public Aluno procuraAluno(String num) {
        return this.alunosFacade.procuraAluno(num);
    }

    public void setCurrentUser(String numero){
        this.currentUser = this.utilizadoresFacade.procuraUtilizador(numero);
    }

    public String getUserType(){
        return this.currentUser.getTipo();
    }

    public String getCurrentUserNum(){
        return this.currentUser.getNumero();
    }

    public boolean verificaLogin(String numero, String pass){
        boolean res = false;
        res = this.utilizadoresFacade.verificaLogin(numero, pass);
        if(res){
            setCurrentUser(numero);
        }
        return res;
    }
    // ----------------------------------------------------------------------------------------

    @Override
    public void importarTurnosDeCSV() {
        this.ucsFacade.importarTurnosDeCSV();
    }

    @Override
    public void importarAlunosDeCSV() {
        this.alunosFacade.importarAlunosDeCSV();
    }

    @Override
    public void putDiretorDeUc() {
        this.utilizadoresFacade.putDiretorDeUc();
    }

    public void importarUtilizadoresDeCSV() {
        this.utilizadoresFacade.importarUtilizadoresDeCSV();
    }

    @Override
    public void importarUCsDeCSV() {
        this.ucsFacade.importarUCsDeCSV();
    }
}

