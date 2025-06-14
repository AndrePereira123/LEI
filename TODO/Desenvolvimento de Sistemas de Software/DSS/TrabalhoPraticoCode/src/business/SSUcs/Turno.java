package business.SSUcs;

public class Turno {
    private String id;
    private String codigoUC;
    private String diaSemana;
    private String horaInicio;
    private String horaFim;
    private String sala;
    private String type;
    private int capacidade;
    private int ninscritos;

    public Turno(String id, String codigoUC, String diaSemana, String horaInicio, String horaFim, String sala, String type, int capacidade_uc, int inscritos) {
        this.id = id;
        this.codigoUC = codigoUC;
        this.diaSemana = diaSemana;
        this.horaInicio = horaInicio;
        this.horaFim = horaFim;
        this.sala = sala;
        this.type = type;
        this.capacidade = capacidade_uc;
        this.ninscritos = inscritos;
    }

    public String getId() {
        return id;
    }

    public String getCodigoUC() {
        return codigoUC;
    }

    public String getDiaSemana() {
        return diaSemana;
    }

    public String getHoraInicio() {
        return horaInicio;
    }

    public String getHoraFim() {
        return horaFim;
    }

    public String getSala() {
        return sala;
    }

    public String getType() {
        return type;
    }

    public int getCapacidade() {
        return capacidade;
    }

    public int getNinscritos() {
        return ninscritos;
    }

    public void atualizaInscritos(int val){
        this.ninscritos += val;
    }

    @Override
    public String toString(){
        return "Turno{"+this.id +", "+this.codigoUC+", "+this.diaSemana+", "+this.horaInicio+", "+this.horaFim+", "+this.sala+", "+this.capacidade+", "+this.ninscritos+'}';
    }
}
