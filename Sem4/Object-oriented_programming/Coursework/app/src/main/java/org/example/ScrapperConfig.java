package com.api.scrapper;

import java.util.List;

public class ScrapperConfig {
    private int maxThreads;
    private int timeoutSeconds;
    private String outputFormat;
    private String configFile;
    private List<ApiConfig> apiConfigs;
    
    public ScrapperConfig(int maxThreads, int timeoutSeconds, String outputFormat, String configFile) {
        this.maxThreads = maxThreads;
        this.timeoutSeconds = timeoutSeconds;
        this.outputFormat = outputFormat;
        this.configFile = configFile;
    }
    
    // Getters and setters
    public int getMaxThreads() { return maxThreads; }
    public int getTimeoutSeconds() { return timeoutSeconds; }
    public String getOutputFormat() { return outputFormat; }
    public String getConfigFile() { return configFile; }
    public List<ApiConfig> getApiConfigs() { return apiConfigs; }
    public void setApiConfigs(List<ApiConfig> apiConfigs) { this.apiConfigs = apiConfigs; }
}
