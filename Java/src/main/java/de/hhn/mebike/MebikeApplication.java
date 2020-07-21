package de.hhn.mebike;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class MebikeApplication implements CommandLineRunner {

    Logger logger = LoggerFactory.getLogger(MebikeApplication.class);

    public static void main(String[] args) {
        SpringApplication.run(MebikeApplication.class, args);
    }

    @Override
    public void run(String... args) throws Exception {
        logger.info("Successfully started");
    }
}
