package de.stal;

import jakarta.persistence.*;

import java.sql.Date;
import java.sql.Time;
import java.util.Objects;

@Entity
public class Measurement {

    @Id
    @SequenceGenerator(
            name = "measurement_id_sequence",
            sequenceName = "measurement_id_sequence",
            allocationSize = 1
    )
    @GeneratedValue(
            strategy = GenerationType.SEQUENCE,
            generator = "measurement_id_sequence"
    )
    private Integer id;
    private Double temperature;
    private Double humidity;
    private Double pressure;
    private Double resistance;
    private java.sql.Date date;
    private java.sql.Time time;

    public Measurement(Integer id, Double temperature, Double humidity, Double pressure, Double resistance, Date date, Time time) {
        this.id = id;
        this.temperature = temperature;
        this.humidity = humidity;
        this.pressure = pressure;
        this.resistance = resistance;
        this.date = date;
        this.time = time;
    }

    public Measurement() {
    }

    @Override
    public String toString() {
        return "Measurement{" +
                "id=" + id +
                ", temperature=" + temperature +
                ", humidity=" + humidity +
                ", pressure=" + pressure +
                ", resistance=" + resistance +
                ", date=" + date +
                ", time=" + time +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Measurement that = (Measurement) o;
        return Objects.equals(id, that.id) && Objects.equals(temperature, that.temperature) && Objects.equals(humidity, that.humidity) && Objects.equals(pressure, that.pressure) && Objects.equals(resistance, that.resistance) && Objects.equals(date, that.date) && Objects.equals(time, that.time);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, temperature, humidity, pressure, resistance, date, time);
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Double getTemperature() {
        return temperature;
    }

    public void setTemperature(Double temperature) {
        this.temperature = temperature;
    }

    public Double getHumidity() {
        return humidity;
    }

    public void setHumidity(Double humidity) {
        this.humidity = humidity;
    }

    public Double getPressure() {
        return pressure;
    }

    public void setPressure(Double pressure) {
        this.pressure = pressure;
    }

    public Double getResistance() {
        return resistance;
    }

    public void setResistance(Double resistance) {
        this.resistance = resistance;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public Time getTime() {
        return time;
    }

    public void setTime(Time time) {
        this.time = time;
    }
}
