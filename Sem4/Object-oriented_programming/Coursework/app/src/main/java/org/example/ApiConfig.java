package com.api.scrapper;

import com.fasterxml.jackson.annotation.JsonProperty;

public class ApiConfig {
    private String name;
    private String url;
    private String method;
    private String requestBody;
    private String authType;
    private String authToken;
    
    // Конструкторы, геттеры и сеттеры
    public ApiConfig(
            @JsonProperty("name") String name,
            @JsonProperty("url") String url,
            @JsonProperty("method") String method,
            @JsonProperty("requestBody") String requestBody,
            @JsonProperty("authType") String authType,
            @JsonProperty("authToken") String authToken) {
        this.name = name;
        this.url = url;
        this.method = method;
        this.requestBody = requestBody;
        this.authType = authType;
        this.authToken = authToken;
    }

    // Getters
    public String getName() { return name; }
    public String getUrl() { return url; }
    public String getMethod() { return method; }
    public String getRequestBody() { return requestBody; }
    public String getAuthType() { return authType; }
    public String getAuthToken() { return authToken; }
}
