package com.api.scrapper;

import org.apache.http.client.methods.*;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

public class ApiClient {
    private static final Logger logger = LoggerFactory.getLogger(ApiClient.class);
    private static final CloseableHttpClient httpClient = HttpClients.createDefault();

    public String fetchData(ApiConfig apiConfig) throws IOException {
        HttpRequestBase request = createRequest(apiConfig);
        addAuthHeaders(apiConfig, request);
        
        logger.info("Fetching data from {}: {}", apiConfig.getName(), apiConfig.getUrl());
        
        try (CloseableHttpResponse response = httpClient.execute(request)) {
            int statusCode = response.getStatusLine().getStatusCode();
            if (statusCode >= 200 && statusCode < 300) {
                return EntityUtils.toString(response.getEntity());
            } else {
                throw new IOException("API request failed with status code: " + statusCode);
            }
        }
    }

    private HttpRequestBase createRequest(ApiConfig apiConfig) throws UnsupportedEncodingException {
        String method = apiConfig.getMethod() != null ? apiConfig.getMethod().toUpperCase() : "GET";
        
        switch (method) {
            case "POST":
                HttpPost postRequest = new HttpPost(apiConfig.getUrl());
                if (apiConfig.getRequestBody() != null) {
                    postRequest.setEntity(new StringEntity(apiConfig.getRequestBody()));
                }
                return postRequest;
            case "PUT":
                HttpPut putRequest = new HttpPut(apiConfig.getUrl());
                if (apiConfig.getRequestBody() != null) {
                    putRequest.setEntity(new StringEntity(apiConfig.getRequestBody()));
                }
                return putRequest;
            case "DELETE":
                return new HttpDelete(apiConfig.getUrl());
            default:
                return new HttpGet(apiConfig.getUrl());
        }
    }

    private void addAuthHeaders(ApiConfig apiConfig, HttpRequestBase request) {
        if (apiConfig.getAuthType() != null && apiConfig.getAuthToken() != null) {
            switch (apiConfig.getAuthType().toLowerCase()) {
                case "bearer":
                    request.addHeader("Authorization", "Bearer " + apiConfig.getAuthToken());
                    break;
                case "basic":
                    request.addHeader("Authorization", "Basic " + apiConfig.getAuthToken());
                    break;
                case "apikey":
                    request.addHeader("X-API-KEY", apiConfig.getAuthToken());
                    break;
            }
        }
    }
}
