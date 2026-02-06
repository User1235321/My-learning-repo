package com.api.scrapper;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.util.concurrent.TimeUnit;

public class ApiWorker implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(ApiWorker.class);
    private final ApiConfig apiConfig;
    private final DataWriter dataWriter;
    private final int timeout;
    private final ApiClient apiClient;

    public ApiWorker(ApiConfig apiConfig, DataWriter dataWriter, int timeout, ApiClient apiClient) {
        this.apiConfig = apiConfig;
        this.dataWriter = dataWriter;
        this.timeout = timeout;
        this.apiClient = apiClient;
    }

    @Override
    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            try {
                String data = apiClient.fetchData(apiConfig);
                dataWriter.writeData(apiConfig.getName(), data);
                
                logger.info("Successfully fetched data from {}", apiConfig.getName());
                
                TimeUnit.SECONDS.sleep(timeout);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                logger.info("Worker for {} was interrupted", apiConfig.getName());
            } catch (IOException e) {
                logger.error("Error fetching data from {}: {}", apiConfig.getName(), e.getMessage());
                try {
                    TimeUnit.SECONDS.sleep(timeout);
                } catch (InterruptedException ie) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }
}
