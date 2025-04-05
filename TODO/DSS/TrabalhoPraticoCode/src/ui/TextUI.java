package ui;

import business.*;

import java.util.Scanner;

public class TextUI {
    // O model tem a 'lógica de negócio'.
    private GesHorariosFacade model;

    // Scanner para leitura
    private Scanner scin;

    public TextUI() {

        this.model = new GesHorariosFacade();
        this.model.putDiretorDeUc();
        scin = new Scanner(System.in);
    }

    public void run() {
        System.out.println("Bem vindo ao Sistema de Gestão de Horários!");
        boolean logged = false;
        while (!logged) {
            logged = menuLogin();
        }

        String userType = this.model.getUserType();
        if(userType.equals("Diretor de Curso")) {
            this.menuPrincipal();
        } else {
            this.menuAluno();
        }

        System.out.println("Até breve...");
    }

    // Métodos auxiliares - Estados da UI

    private boolean menuLogin() {
        System.out.println("Por favor, inicie sessão:");
        System.out.println("Número:");
        String numero = scin.nextLine();
        System.out.println("Palavra-passe:");
        String pass = scin.nextLine();

        return this.model.verificaLogin(numero, pass);
    }

    private void menuAluno() {
        Menu menu = new Menu(new String[]{
                "Consultar Horário",
        });

        menu.setHandler(1, this::consultarHorario);

        menu.run();
    }

    private void consultarHorario() {
        System.out.println(this.model.listaHorarioAluno(this.model.getCurrentUserNum()));
    }

    private void menuPrincipal() {
        Menu menu = new Menu(new String[]{
                "Operações sobre Alunos",
                "Operações sobre Horários",
                "Importar dados",
        });

        menu.setHandler(1, this::gestaoDeAlunos);
        menu.setHandler(2, this::gestaoHorarios);
        menu.setHandler(3, this::importarDados);

        menu.run();
    }

    /**
     *  Estado - Gestão de Alunos
     */
    private void gestaoDeAlunos() {
        Menu menu = new Menu("Gestão de Alunos", new String[]{
                "Adicionar Aluno",
                "Consultar Aluno",
                "Listar Alunos"
        });

        // Registar os handlers
        menu.setHandler(1, this::adicionarAluno);
        menu.setHandler(2, this::consultarAluno);
        menu.setHandler(3, this::listarAlunos);

        menu.run();
    }



    private void adicionarAluno() {
        try {
            System.out.println("Número da novo aluno: ");
            String num = scin.nextLine();
            if (!this.model.existeAluno(num)) {
                System.out.println("Nome da novo aluno: ");
                String nome = scin.nextLine();
                System.out.println("Email da novo aluno: ");
                String email = scin.nextLine();
                System.out.println("Regime do novo aluno: (enter para sem estatuto)");
                String regime = scin.nextLine();
                System.out.println("Codigo de curso do novo aluno:");
                String cod_curso = scin.nextLine();
                var b = this.model.adicionaAluno(num, nome, email, regime, cod_curso);
                if(b) {
                    System.out.println("Aluno adicionado");
                } else {
                    System.out.println("Codigo de curso inserido inválido. Operação cancelada.");
                }
            } else {
                System.out.println("Esse número de aluno já existe!");
            }
        }
        catch (NullPointerException e) {
            System.out.println(e.getMessage());
        }
    }


    private void consultarAluno() {
        try {
            System.out.println("Número a consultar: ");
            String num = scin.nextLine();
            if (this.model.existeAluno(num)) {
                System.out.println(this.model.procuraAluno(num).toString());
                System.out.println(this.model.listaHorarioAluno(num));
            } else {
                System.out.println("Esse número de aluno não existe!");
            }
        }
        catch (NullPointerException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     *  Estado - Listar Alunos
     */
    private void listarAlunos() {
        try {
            System.out.println(this.model.getAlunos().toString());
        }
        catch (NullPointerException e) {
            System.out.println(e.getMessage());
        }
    }


    // -------------------------------------------------------------------------------------------

    private void gestaoHorarios() {
        Menu menu = new Menu("Gestão de Horários", new String[]{
                "Gerar Horários",
                "Atribuição manual de um Aluno",
                "Remoção manual de um Aluno"
        });

        // Registar os handlers
        menu.setHandler(1, this::gerarHorarios);
        menu.setHandler(2, this::abtribuicaoManual);
        menu.setHandler(3, this::remocaoManual);

        menu.run();
    }

    private void gerarHorarios() {
        this.model.horarioGenerator();
        System.out.println("Horarios Gerados!");
    }

    private void abtribuicaoManual(){
        try {
            System.out.println("Número do aluno: ");
            String num = scin.nextLine();
            if (this.model.existeAluno(num)) {
                System.out.println("Identificador do turno: ");
                String id = scin.nextLine();
                if (this.model.existeTurno(id)) {
                    boolean added = this.model.atribuiAlunoManual(num, id);
                    if (added) System.out.println("Aluno adicionado ao turno!");
                    else System.out.println("Aluno já pertence ao Turno selecionado.");
                } else {
                    System.out.println("Esse identificador de turno não existe!");
                }
            } else {
                System.out.println("Esse número de aluno não existe!");
            }
        }
        catch (NullPointerException e) {
            System.out.println(e.getMessage());
        }
    }

    private void remocaoManual(){
        try {
            System.out.println("Identificador do turno: ");
            String id = scin.nextLine();
            if (this.model.existeTurno(id)) {
                System.out.println("Número do aluno: ");
                String num = scin.nextLine();
                if (this.model.existeAluno(num)) {
                    boolean removed = this.model.removeAlunoManual(num, id);
                    if (removed) System.out.println("Aluno removido do turno!");
                    else System.out.println("Aluno não pertence ao Turno seleciondo.");
                } else {
                    System.out.println("Esse número de aluno não existe!");
                }
            } else {
                System.out.println("Esse identificador de turno não existe!");
            }
        }
        catch (NullPointerException e) {
            System.out.println(e.getMessage());
        }
    }

    // -------------------------------------------------------------------------------------------

    private void importarDados(){
        Menu menu = new Menu("Importar Dados", new String[]{
                "Importar Lista de UCs",
                "Importar Lista de Turnos",
                "Importar Lista de Alunos"
        });

        // Registar os handlers
        menu.setHandler(1, this::importarUCs);
        menu.setHandler(2, this::importarTurnos);
        menu.setHandler(3, this::importarAlunos);

        menu.run();
    }

    private void importarUCs(){
        this.model.importarUCsDeCSV();
    }

    private void importarAlunos(){
        this.model.importarAlunosDeCSV();
        this.model.importarUtilizadoresDeCSV();
    }

    private void importarTurnos(){
        this.model.importarTurnosDeCSV();
    }
}
