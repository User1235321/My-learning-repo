package com.api.scrapper;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.concurrent.locks.ReentrantLock;

public class DataWriter {
    private static final Logger logger = LoggerFactory.getLogger(DataWriter.class);
    private final String outputFile;
    private final String format;
    private final ReentrantLock lock = new ReentrantLock();
    private final ObjectMapper objectMapper = new ObjectMapper();

    public DataWriter(String outputFile, String format) {
        this.outputFile = outputFile;
        this.format = format.toLowerCase();
        
        initializeOutputFile();
    }

    private void initializeOutputFile() {
        if (!Files.exists(Paths.get(outputFile))) {
            lock.lock();
            try (FileWriter writer = new FileWriter(outputFile, true)) {
                if (format.equals("json")) {
                    writer.write("[\n");
                } else {
                    writer.write("timestamp,api_name,data\n");
                }
            } catch (IOException e) {
                logger.error("Error initializing output file", e);
            } finally {
                lock.unlock();
            }
        }
    }

    public void writeData(String apiName, String data) {
        lock.lock();
        try (FileWriter writer = new FileWriter(outputFile, true)) {
            String timestamp = LocalDateTime.now().format(DateTimeFormatter.ISO_LOCAL_DATE_TIME);
            
            if (format.equals("json")) {
                String jsonEntry = String.format(
                    "{\"timestamp\":\"%s\",\"api\":\"%s\",\"data\":%s}",
                    timestamp, apiName, data
                );
                
                // Проверяем, не первый ли это элемент в JSON массиве
                if (Files.size(Paths.get(outputFile)) > 2) {
                    writer.write(",\n" + jsonEntry);
                } else {
                    writer.write(jsonEntry);
                }
            } else {
                // CSV формат
                String escapedData = data.replace("\"", "\"\"");
                writer.write(String.format("%s,%s,\"%s\"\n", timestamp, apiName, escapedData));
            }
        } catch (IOException e) {
            logger.error("Error writing data to file", e);
        } finally {
            lock.unlock();
        }
    }
    
    public void finalizeJsonFile() {
        if (format.equals("json")) {
            lock.lock();
            try (FileWriter writer = new FileWriter(outputFile, true)) {
                writer.write("\n]");
            } catch (IOException e) {
                logger.error("Error finalizing JSON file", e);
            } finally {
                lock.unlock();
            }
        }
    }
}
