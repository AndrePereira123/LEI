package data;

import business.SSUcs.Curso;

import java.sql.*;
import java.util.*;

public class CursoDAO implements Map<String, Curso> {
    private static CursoDAO singleton = null;

    private CursoDAO() {
        try(Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
            Statement stmt = conn.createStatement()) {
            String sql = "CREATE TABLE IF NOT EXISTS curso(" +
                    "Codigo varchar(10) NOT NULL PRIMARY KEY," +
                    "Nome varchar(45) DEFAULT NULL)";
            stmt.executeUpdate(sql);

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public static CursoDAO getInstance() {
        if (CursoDAO.singleton == null) {
            CursoDAO.singleton = new CursoDAO();
        }
        return CursoDAO.singleton;
    }

    @Override
    public int size() {
        int i = 0;
        String sql = "SELECT COUNT(*) FROM curso";
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
        String sql = "SELECT Codigo FROM curso WHERE Codigo = ?";
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
    public boolean containsValue(Object value) {
        return false;
    }

    @Override
    public Curso get(Object key) {
        Curso curso = null;
        String sql = "SELECT * FROM curso WHERE Codigo = ?";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, key.toString());
            try(ResultSet rs = pstmt.executeQuery()) {
                if(rs.next()) {
                    String codigo = rs.getString("Codigo");
                    String nome = rs.getString("Nome");

                    curso = new Curso(codigo, nome);
                }
            }
        } catch (SQLException e){
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return curso;
    }

    @Override
    public Curso put(String key, Curso curso) {
        Curso res = null;
        String sql = "INSERT INTO curso (Codigo, Nome) VALUES (?, ?)" +
                "ON DUPLICATE KEY UPDATE Nome = VALUES(nome)";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {

            pstmt.setString(1, curso.getCodigo());
            pstmt.setString(2, curso.getNome());

            pstmt.executeUpdate();

            res = curso;

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }
        return res;
    }

    @Override
    public Curso remove(Object key) {
        return null;
    }

    @Override
    public void putAll(Map<? extends String, ? extends Curso> m) {

    }

    @Override
    public void clear() {

    }

    @Override
    public Set<String> keySet() {
        return Set.of();
    }

    @Override
    public Collection<Curso> values() {
        Collection<Curso> res = new HashSet<>();
        String sql = "SELECT Codigo FROM curso";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                String codigo = rs.getString("Codigo");
                Curso c = this.get(codigo);
                res.add(c);
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return res;
    }

    @Override
    public Set<Entry<String, Curso>> entrySet() {
        return Set.of();
    }
}
