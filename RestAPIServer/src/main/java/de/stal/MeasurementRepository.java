package de.stal;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface MeasurementRepository
        extends JpaRepository<Measurement, Integer> {
}
