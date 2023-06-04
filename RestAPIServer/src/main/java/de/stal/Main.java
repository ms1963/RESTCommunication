package de.stal;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@SpringBootApplication
@RequestMapping("measurements/api")
public class Main {

    private final MeasurementRepository measurementRepository;

    public Main(MeasurementRepository measurementRepository) {
        this.measurementRepository = measurementRepository;
    }

    public static void main(String[] args) {
        SpringApplication.run(Main.class, args);
    }

    @GetMapping
    public List<Measurement> getMeasurement() {
        return measurementRepository.findAll();
    }

    @GetMapping("{id}")
    public Optional<Measurement> getMeasurementById(@PathVariable("id") Integer id){
        return measurementRepository.findById(id);
    }

    @PostMapping
    public void addMeasurement(@RequestBody NewMeasurementRequest request) {
        Measurement measurement = new Measurement();
        measurement.setDate(request.date);
        measurement.setTime(request.time);
        measurement.setTemperature(request.temperature);
        measurement.setHumidity(request.humidity);
        measurement.setPressure(request.pressure);
        measurement.setResistance(request.resistance);
        measurementRepository.save(measurement);
    }

    record NewMeasurementRequest(
            java.sql.Date date,
            java.sql.Time time,
            Double temperature,
            Double humidity,
            Double pressure,
            Double resistance
    ){}

    @DeleteMapping("{measurementId}")
    public void deleteMeasurement(@PathVariable("measurementId") Integer id) {
        measurementRepository.deleteById(id);
    }

    // assignment
    @PutMapping("{measurementId}")
    public void updateMeasurement(@PathVariable("measurementId") Integer id,
                               @RequestBody NewMeasurementRequest msmUpdate) {
        Measurement existingMeasurement = measurementRepository.findById(id)
                .orElseThrow();
        existingMeasurement.setDate(msmUpdate.date);
        existingMeasurement.setTime(msmUpdate.time);
        existingMeasurement.setTemperature(msmUpdate.temperature);
        existingMeasurement.setHumidity(msmUpdate.humidity);
        existingMeasurement.setPressure(msmUpdate.pressure);
        existingMeasurement.setResistance(msmUpdate.resistance);


        measurementRepository.save(existingMeasurement);
    }

}
