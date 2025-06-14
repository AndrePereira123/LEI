package data;

import business.SSAlunos.Aluno;
import business.SSUtilizadores.DiretorDeCurso;
import business.SSUtilizadores.Utilizador;

import java.sql.*;
import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class UtilizadorDAO implements Map<String, Utilizador> {
    private static UtilizadorDAO singleton = null;

    private UtilizadorDAO() {
        try(Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
            Statement stmt = conn.createStatement()) {
            String sql = "CREATE TABLE IF NOT EXISTS utilizador(" +
                    "Numero varchar(10) NOT NULL PRIMARY KEY," +
                    "Password varchar(45) NOT NULL," +
                    "Tipo varchar(45) NOT NULL)";
            stmt.executeUpdate(sql);

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public static UtilizadorDAO getInstance() {
        if (UtilizadorDAO.singleton == null) {
            UtilizadorDAO.singleton = new UtilizadorDAO();
        }
        return UtilizadorDAO.singleton;
    }


    @Override
    public int size() {
        return 0;
    }

    @Override
    public boolean isEmpty() {
        return false;
    }

    @Override
    public boolean containsKey(Object key) {
        return false;
    }

    @Override
    public boolean containsValue(Object value) {
        return false;
    }

    @Override
    public Utilizador get(Object key) {
        Utilizador user = null;
        String sql = "SELECT * FROM utilizador WHERE Numero = ?";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, key.toString());
            try(ResultSet rs = pstmt.executeQuery()) {
                if(rs.next()) {
                    String numero = rs.getString("Numero");
                    String password = rs.getString("Password");
                    String tipo = rs.getString("Tipo");

                    if (tipo.equals("Aluno")) {
                        user = new Aluno(numero, password);
                    }
                    if (tipo.equals("Diretor de Curso")) {
                        user = new DiretorDeCurso(numero, password);
                    }
                }
            }
        } catch (SQLException e){
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return user;
    }


    @Override
    public Utilizador put(String key, Utilizador user) {
        Utilizador res = null;
        String sql = "INSERT INTO utilizador (Numero, Password, Tipo) VALUES (?, ?, ?)" +
                "ON DUPLICATE KEY UPDATE Password = VALUES(Password), Tipo = VALUES(Tipo)";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {

            pstmt.setString(1, user.getNumero());
            pstmt.setString(2, user.getPassword());
            pstmt.setString(3, user.getTipo());

            pstmt.executeUpdate();

            res = user;

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }
        return res;
    }

    @Override
    public Utilizador remove(Object key) {
        return null;
    }

    @Override
    public void putAll(Map<? extends String, ? extends Utilizador> m) {

    }

    @Override
    public void clear() {

    }

    @Override
    public Set<String> keySet() {
        return Set.of();
    }

    @Override
    public Collection<Utilizador> values() {
        return List.of();
    }

    @Override
    public Set<Entry<String, Utilizador>> entrySet() {
        return Set.of();
    }
}
