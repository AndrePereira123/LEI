package data;

import business.SSUcs.UC;

import java.sql.*;
import java.util.*;

public class UCDAO implements Map<String, UC> {
    private static UCDAO singleton = null;

    private UCDAO() {
        try(Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
            Statement stmt = conn.createStatement()) {
            String sql = "CREATE TABLE IF NOT EXISTS uc(" +
                    "Codigo varchar(10) NOT NULL PRIMARY KEY," +
                    "Nome varchar(45) DEFAULT NULL," +
                    "Ano integer DEFAULT NULL," +
                    "Curso varchar(10) DEFAULT NULL," +
                    "foreign key(Curso) references curso(Codigo))";
            stmt.executeUpdate(sql);

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public static UCDAO getInstance() {
        if (UCDAO.singleton == null) {
            UCDAO.singleton = new UCDAO();
        }
        return UCDAO.singleton;
    }

    @Override
    public int size() {
        int i = 0;
        String sql = "SELECT COUNT(*) FROM uc";
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
        String sql = "SELECT Codigo FROM uc WHERE Codigo = ?";
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
    public UC get(Object key) {
        UC uc = null;
        String sql = "SELECT * FROM uc WHERE Codigo = ?";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, key.toString());
            try(ResultSet rs = pstmt.executeQuery()) {
                if(rs.next()) {
                    String num = rs.getString("Codigo");
                    String nome = rs.getString("Nome");
                    int ano = rs.getInt("Ano");
                    String curso = rs.getString("Curso");

                    uc = new UC(num, nome, ano, curso);
                }
            }
        } catch (SQLException e){
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return uc;
    }

    @Override
    public UC put(String key, UC uc) {
        UC res = null;
        String sql = "INSERT INTO uc (Codigo, Nome, Ano, Curso) VALUES (?, ?, ?, ?) " +
                "ON DUPLICATE KEY UPDATE Nome = VALUES(nome), Ano = VALUES(ano), Curso = VALUES(curso)";

        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {

            pstmt.setString(1, uc.getCodigo())  ;
            pstmt.setString(2, uc.getNome());
            pstmt.setInt(3, uc.getAno());
            pstmt.setString(4, uc.getCurso());
            pstmt.executeUpdate();

            res = uc;

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }
        return res;
    }

    @Override
    public UC remove(Object key) {
        return null;
    }

    @Override
    public void putAll(Map<? extends String, ? extends UC> m) {

    }

    @Override
    public void clear() {

    }

    @Override
    public Set<String> keySet() {
        return Set.of();
    }

    @Override
    public Collection<UC> values() {
        Collection<UC> res = new HashSet<>();
        String sql = "SELECT Codigo FROM uc";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                String codigo = rs.getString("Codigo");
                UC u = this.get(codigo);
                res.add(u);
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return res;
    }

    @Override
    public Set<Entry<String, UC>> entrySet() {
        return Set.of();
    }
}
