package data;

import business.SSAlunos.Aluno;

import java.sql.*;
import java.util.*;

public class AlunoDAO implements Map<String, Aluno> {
    private static AlunoDAO singleton = null;

    private AlunoDAO() {
        try(Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
            Statement stmt = conn.createStatement()) {
            String sql = "CREATE TABLE IF NOT EXISTS aluno(" +
                    "Num varchar(10) NOT NULL PRIMARY KEY," +
                    "Nome varchar(45) DEFAULT NULL," +
                    "Email varchar(45) DEFAULT NULL," +
                    "Estatuto varchar(4) DEFAULT NULL," +
                    "Curso varchar(10) NOT NULL," +
                    "foreign key(Curso) references curso(Codigo))";
            stmt.executeUpdate(sql);

            sql = "CREATE TABLE IF NOT EXISTS aluno_tem_uc(" +
                    "AlunoNum VARCHAR(10)," +
                    "UCCodigo VARCHAR(10)," +
                    "PRIMARY KEY (AlunoNum, UCCodigo)," +
                    "FOREIGN KEY (AlunoNum) REFERENCES aluno(Num)," +
                    "FOREIGN KEY (UCCodigo) REFERENCES uc(Codigo))";
            stmt.executeUpdate(sql);

            sql = "CREATE TABLE IF NOT EXISTS horario(" +
                    "AlunoNum VARCHAR(10)," +
                    "IdTurno VARCHAR(10)," +
                    "PRIMARY KEY (AlunoNum, IdTurno)," +
                    "FOREIGN KEY (AlunoNum) REFERENCES aluno(Num)," +
                    "FOREIGN KEY (IdTurno) REFERENCES turno(Id))";
            stmt.executeUpdate(sql);


        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public static AlunoDAO getInstance() {
        if (AlunoDAO.singleton == null) {
            AlunoDAO.singleton = new AlunoDAO();
        }
        return AlunoDAO.singleton;
    }

    @Override
    public int size() {
        int i = 0;
        String sql = "SELECT COUNT(*) FROM aluno";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             Statement stm = conn.createStatement();
             ResultSet rs = stm.executeQuery(sql)) {
            if(rs.next()) {
                i = rs.getInt(1);
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }
        return i;
    }

    @Override
    public boolean isEmpty() {
        return this.size() == 0;
    }

    @Override
    public boolean containsKey(Object key) {
        boolean r;
        String sql = "SELECT Num FROM aluno WHERE Num = ?";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, key.toString());
            try(ResultSet rs = pstmt.executeQuery()) {
                r = rs.next();
            }
        } catch (SQLException e){
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return r;
    }

    @Override
    public boolean containsValue(Object o) {
        return false;
    }

    @Override
    public Aluno get(Object key) {
        Aluno aluno = null;
        String sql = "SELECT * FROM aluno WHERE Num = ?";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, key.toString());
            try(ResultSet rs = pstmt.executeQuery()) {
                if(rs.next()) {
                    String num = rs.getString("Num");
                    String nome = rs.getString("Nome");
                    String email = rs.getString("Email");
                    String estatuto = rs.getString("Estatuto");
                    String curso = rs.getString("Curso");

                    aluno = new Aluno(num, nome, email, estatuto, curso);
                }
            }

            if(aluno != null){
                String sqlAlunoUC = "SELECT UCCodigo FROM aluno_tem_uc WHERE AlunoNum = ?";
                try (PreparedStatement stmtAlunoUC = conn.prepareStatement(sqlAlunoUC)) {
                    stmtAlunoUC.setString(1, aluno.getNumero());
                    try (ResultSet rs = stmtAlunoUC.executeQuery()) {
                        while (rs.next()) {
                            aluno.addUC(rs.getString("UCCodigo"));
                        }
                    }
                }

                String sqlHorario = "SELECT IdTurno FROM horario WHERE AlunoNum = ?";
                try (PreparedStatement stmtHorario = conn.prepareStatement(sqlHorario)) {
                    stmtHorario.setString(1, aluno.getNumero());
                    try (ResultSet rs = stmtHorario.executeQuery()) {
                        List<String> horario = new ArrayList<>();
                        while (rs.next()) {
                            horario.add(rs.getString("IdTurno"));
                        }
                        aluno.setHorario(horario);
                    }
                }
            }

        } catch (SQLException e){
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return aluno;
    }

    @Override
    public Aluno put(String key, Aluno aluno) {
        Aluno res = null;
        String sql = "INSERT INTO aluno (Num, Nome, Email, Estatuto, Curso) VALUES (?, ?, ?, ?, ?) " +
                "ON DUPLICATE KEY UPDATE Nome = VALUES(nome), Email = VALUES(email), Estatuto = VALUES(Estatuto)";

        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {

            pstmt.setString(1, aluno.getNumero());
            pstmt.setString(2, aluno.getNome());
            pstmt.setString(3, aluno.getEmail());
            pstmt.setString(4, aluno.getEstatuto());
            pstmt.setString(5, aluno.getCurso());
            pstmt.executeUpdate();

            res = aluno;

            //deletar entradas anteriores
            String sqlDeleteAlunoUC = "DELETE FROM aluno_tem_uc WHERE AlunoNum = ?";
            try (PreparedStatement stmtDelete = conn.prepareStatement(sqlDeleteAlunoUC)) {
                stmtDelete.setString(1, aluno.getNumero());
                stmtDelete.executeUpdate();
            }

            //adicionar novas ucs
            String sqlAlunoUC = "INSERT INTO aluno_tem_uc (AlunoNum, UCCodigo) VALUES (?, ?)";
            try (PreparedStatement stmtAlunoUC = conn.prepareStatement(sqlAlunoUC)) {
                for (String uc : aluno.getUcs()) {
                    stmtAlunoUC.setString(1, aluno.getNumero());
                    stmtAlunoUC.setString(2, uc);
                    stmtAlunoUC.executeUpdate();
                }
            }


            String sqlDeleteHorario = "DELETE FROM horario WHERE AlunoNum = ?";
            try (PreparedStatement stmtDelete = conn.prepareStatement(sqlDeleteHorario)) {
                stmtDelete.setString(1, aluno.getNumero());
                stmtDelete.executeUpdate();
            }

            String sqlHorario = "INSERT INTO horario (AlunoNum, IdTurno) VALUES (?, ?)";
            try (PreparedStatement stmtAlunoUC = conn.prepareStatement(sqlHorario)) {
                for (String turno : aluno.getHorario()) {
                    stmtAlunoUC.setString(1, aluno.getNumero());
                    stmtAlunoUC.setString(2, turno);
                    stmtAlunoUC.executeUpdate();
                }
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }
        return res;
    }


    @Override
    public Aluno remove(Object o) {
        return null;
    }

    @Override
    public void putAll(Map<? extends String, ? extends Aluno> alunos) {
        for(Aluno a : alunos.values()) {
            this.put(a.getNumero(), a);
        }
    }

    @Override
    public void clear() {

    }

    @Override
    public Set<String> keySet() {
        Set<String> res = new HashSet<>();
        String sql = "SELECT Num FROM aluno";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                res.add(rs.getString("Num"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }
        return res;
    }

    @Override
    public Collection<Aluno> values() {
        Collection<Aluno> res = new HashSet<>();
        String sql = "SELECT Num FROM aluno";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                String num = rs.getString("Num");
                Aluno a = this.get(num);
                res.add(a);
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return res;
    }

    @Override
    public Set<Entry<String, Aluno>> entrySet() {
        return Set.of();
    }
}
