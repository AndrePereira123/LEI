package business.SSUcs;

import data.CursoDAO;
import data.TurnoDAO;
import data.UCDAO;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collection;


public class GesUCsFacade implements IGesUCsFacade {
    private CursoDAO cursos;
    private UCDAO ucs;
    private TurnoDAO turnos;

    public GesUCsFacade() {
        this.cursos = CursoDAO.getInstance();
        this.ucs = UCDAO.getInstance();
        this.turnos = TurnoDAO.getInstance();
    }

    // ---------------------------------------------------------------------------------------
    @Override
    public void importarUCsDeCSV() {
        String caminhoRelativo = "/inputs/ucs.csv";
        String caminhoCSV = Paths.get(System.getProperty("user.dir"), caminhoRelativo).normalize().toString();

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoCSV))) {
            br.readLine();
            String linha;
            while ((linha = br.readLine()) != null) {
                processarLinhaUCsCSV(linha);
            }
            System.out.println("Ficheiro CSV de UCs importado com sucesso!");
        } catch (IOException e) {
            e.printStackTrace();
            throw new RuntimeException("Erro ao ler o ficheiro CSV: " + e.getMessage());
        }
    }

    private void processarLinhaUCsCSV(String linha) {
        String[] dados = linha.split(";");

        criarCurso(dados[0], dados[1]);
        criarUC(dados[3], dados[4], Integer.parseInt(dados[2]), dados[0]);

    }

    private void criarCurso(String codCurso, String nomeCurso) {
        this.cursos.computeIfAbsent(codCurso, c -> new Curso(c, nomeCurso));
    }

    private void criarUC(String codUC, String nomeUC, int ano, String curso) {
        this.ucs.computeIfAbsent(codUC, c -> new UC(c, nomeUC, ano, curso));
    }
    // ---------------------------------------------------------------------------------------

    @Override
    public void importarTurnosDeCSV() {
        String caminhoRelativo = "/inputs/turnos.csv";
        String caminhoCSV = Paths.get(System.getProperty("user.dir"), caminhoRelativo).normalize().toString();

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoCSV))) {
            br.readLine();
            String linha;
            while ((linha = br.readLine()) != null) {
                processarLinhaTurnosCSV(linha);
            }
            System.out.println("Ficheiro CSV de Turnos importado com sucesso!");
        } catch (IOException e) {
            e.printStackTrace();
            throw new RuntimeException("Erro ao ler o ficheiro CSV: " + e.getMessage());
        }
    }

    private void processarLinhaTurnosCSV(String linha) {
        String[] dados = linha.split(";");

        criarTurno(dados);
    }

    private void criarTurno(String[] dados) {
        String idTurno = dados[0];
        String codUC = dados[1];
        String diaSemana = dados[2];
        String HoraInicio = dados[3];
        String HoraFim = dados[4];
        String sala = dados[5];
        String tipo = dados[6];
        int capacidade = Integer.parseInt(dados[7]);

        Turno t = new Turno(idTurno, codUC, diaSemana, HoraInicio, HoraFim, sala, tipo, capacidade, 0);
        this.turnos.put(idTurno, t);
    }

    // --------------------------------------------------------------------------------

    public void diminuirTurno(String numTurno) {
        Turno t = this.turnos.get(numTurno);
        if (t != null) {
            t.atualizaInscritos(-1);
            turnos.put(numTurno, t);
        }
    }

    public void aumentarTurno(String idTurno){
        Turno t = this.turnos.get(idTurno);
        if (t != null) {
            t.atualizaInscritos(1);
            turnos.put(idTurno, t);
        }
    }

    public boolean existeCurso(String num){
        return this.cursos.containsKey(num);
    }

    public boolean existeTurno(String idTurno) {
        return this.turnos.containsKey(idTurno);
    }

    public Collection<String> getTurnos() {
        return new ArrayList<>(this.turnos.keySet());
    }

    public Turno procuraTurno(String idTurno) {
        return this.turnos.get(idTurno);
    }

    public Collection<String> getUcs() {
        return new ArrayList<>(this.ucs.keySet());
    }

    public void putTurno(String chave, Turno turno) {
        this.turnos.put(chave, turno);
    }


    public UC procuraUC(String idUC) {
        return this.ucs.get(idUC);
    }

}
