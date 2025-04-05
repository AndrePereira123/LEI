package data;

import business.SSUcs.Turno;

import java.sql.*;
import java.util.*;

public class TurnoDAO implements Map<String, Turno> {
    private static TurnoDAO singleton = null;

    private TurnoDAO() {
        try(Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
            Statement stmt = conn.createStatement()) {
            String sql = "CREATE TABLE IF NOT EXISTS turno(" +
                    "Id varchar(10) NOT NULL PRIMARY KEY," +
                    "Uc varchar(10) DEFAULT NULL," +
                    "DiaSemana varchar(45) DEFAULT NULL," +
                    "HoraInicio varchar(15) DEFAULT NULL," +
                    "HoraFim varchar(15) NOT NULL," +
                    "Sala varchar(45) NOT NULL," +
                    "Tipo varchar(3) NOT NULL," + // T,TP,PL
                    "Capacidade integer NOT NULL," +
                    "NInscritos integer NOT NULL," +
                    "foreign key(Uc) references uc(Codigo))";
            stmt.executeUpdate(sql);

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public static TurnoDAO getInstance() {
        if (TurnoDAO.singleton == null) {
            TurnoDAO.singleton = new TurnoDAO();
        }
        return TurnoDAO.singleton;
    }

    @Override
    public int size() {
        int i = 0;
        String sql = "SELECT COUNT(*) FROM turno";
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
        String sql = "SELECT Id FROM turno WHERE Id = ?";
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
    public Turno get(Object key) {
        Turno res = null;
        String sql = "SELECT * FROM turno WHERE Id = ?";
        try(Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
            PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, key.toString());
            try (ResultSet rs = pstmt.executeQuery()) {
                if (rs.next()) {
                    String id = rs.getString("Id");
                    String uc = rs.getString("Uc");
                    String diaSemana = rs.getString("DiaSemana");
                    String horaInicio = rs.getString("HoraInicio");
                    String horaFim = rs.getString("HoraFim");
                    String sala = rs.getString("Sala");
                    String tipo = rs.getString("Tipo");
                    int capacidade = rs.getInt("Capacidade");
                    int ninscritos = rs.getInt("NInscritos");

                    res = new Turno(id, uc, diaSemana, horaInicio, horaFim, sala, tipo, capacidade, ninscritos);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return res;
    }

    @Override
    public Turno put(String key, Turno turno) {
        Turno res = null;
        String sql = "INSERT INTO turno (Id, Uc, DiaSemana, HoraInicio, HoraFim, Sala, Tipo, Capacidade, NInscritos) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?) " +
                "ON DUPLICATE KEY UPDATE Uc = VALUES(Uc), DiaSemana = VALUES(DiaSemana), HoraInicio = VALUES(HoraInicio), HoraFim = VALUES(HoraFim), Sala = VALUES(Sala), Tipo = VALUES(Tipo), Capacidade = VALUES(Capacidade), NInscritos = VALUES(NInscritos)";

        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {

            pstmt.setString(1, turno.getId());
            pstmt.setString(2, turno.getCodigoUC());
            pstmt.setString(3, turno.getDiaSemana());
            pstmt.setString(4, turno.getHoraInicio());
            pstmt.setString(5, turno.getHoraFim());
            pstmt.setString(6, turno.getSala());
            pstmt.setString(7, turno.getType());
            pstmt.setInt(8, turno.getCapacidade());
            pstmt.setInt(9, turno.getNinscritos());
            pstmt.executeUpdate();

            res = turno;

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return res;
    }

    @Override
    public Turno remove(Object key) {
        return null;
    }

    @Override
    public void putAll(Map<? extends String, ? extends Turno> turnos) {
        for(Turno t : turnos.values()) {
            this.put(t.getId(), t);
        }
    }

    @Override
    public void clear() {

    }

    @Override
    public Set<String> keySet() {
        Set<String> res = new HashSet<>();
        String sql = "SELECT Id FROM turno";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                res.add(rs.getString("Id"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }
        return res;
    }

    @Override
    public Collection<Turno> values() {
        Collection<Turno> res = new HashSet<>();
        String sql = "SELECT Id FROM turno";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                String id = rs.getString("Id");
                Turno t = this.get(id);
                res.add(t);
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }

        return res;
    }

    @Override
    public Set<Entry<String, Turno>> entrySet() {
        Set<Entry<String, Turno>> res = new HashSet<>();
        String sql = "SELECT Id FROM turno";
        try (Connection conn = DriverManager.getConnection(DAOconfig.URL, DAOconfig.USERNAME, DAOconfig.PASSWORD);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                String id = rs.getString("Id");
                Turno turno = this.get(id);
                res.add(new AbstractMap.SimpleEntry<>(id, turno));
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw new NullPointerException(e.getMessage());
        }
        return res;

    }


}
