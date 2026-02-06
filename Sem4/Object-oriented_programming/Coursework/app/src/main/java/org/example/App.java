package com.api.scrapper;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class App {
    private static final Logger logger = LoggerFactory.getLogger(App.class);
    private static final ObjectMapper objectMapper = new ObjectMapper();

    public static void main(String[] args) {
        if (args.length < 4) {
            System.out.println("Usage: java -jar scrapper.jar <maxThreads> <timeoutSeconds> <outputFormat> <configFile>");
            System.out.println("Example: java -jar scrapper.jar 5 60 json apis.json");
            System.exit(1);
        }

        try {
            int maxThreads = Integer.parseInt(args[0]);
            int timeoutSeconds = Integer.parseInt(args[1]);
            String outputFormat = args[2];
            String configFile = args[3];

            ScrapperConfig config = new ScrapperConfig(maxThreads, timeoutSeconds, outputFormat, configFile);
            loadApiConfigs(config);
            runScrapper(config);
        } catch (NumberFormatException e) {
            logger.error("Invalid number format for maxThreads or timeoutSeconds", e);
            System.exit(1);
        } catch (IOException e) {
            logger.error("Error reading API configuration file", e);
            System.exit(1);
        }
    }

    private static void loadApiConfigs(ScrapperConfig config) throws IOException {
        List<ApiConfig> apiConfigs = objectMapper.readValue(
            new File(config.getConfigFile()),
            new TypeReference<List<ApiConfig>>() {}
        );
        config.setApiConfigs(apiConfigs);
    }

    private static void runScrapper(ScrapperConfig config) {
        ExecutorService executor = Executors.newFixedThreadPool(config.getMaxThreads());
        DataWriter dataWriter = new DataWriter("output." + config.getOutputFormat(), config.getOutputFormat());
        ApiClient apiClient = new ApiClient();

        for (ApiConfig apiConfig : config.getApiConfigs()) {
            executor.submit(new ApiWorker(apiConfig, dataWriter, config.getTimeoutSeconds(), apiClient));
        }

        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            logger.info("Shutting down scrapper...");
            executor.shutdownNow();
            try {
                if (!executor.awaitTermination(5, TimeUnit.SECONDS)) {
                    logger.warn("Executor did not terminate in the specified time.");
                }
                dataWriter.finalizeJsonFile();
                logger.info("Scrapper shutdown complete");
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }));
    }
}
