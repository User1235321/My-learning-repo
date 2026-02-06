plugins {
    id 'java'
    id 'application'
}

group 'com.api'
version '1.0-SNAPSHOT'

repositories {
    mavenCentral()
}

dependencies {
    // HTTP Client
    implementation 'org.apache.httpcomponents:httpclient:4.5.13'
    
    // JSON Processing
    implementation 'com.fasterxml.jackson.core:jackson-databind:2.13.3'
    
    // Logging
    implementation 'org.slf4j:slf4j-api:1.7.36'
    implementation 'ch.qos.logback:logback-classic:1.2.11'
    
    // Testing
    testImplementation 'org.junit.jupiter:junit-jupiter-api:5.8.2'
    testRuntimeOnly 'org.junit.jupiter:junit-jupiter-engine:5.8.2'
    testImplementation 'org.mockito:mockito-core:4.3.1'
    testImplementation 'com.github.tomakehurst:wiremock-jre8:2.33.2'
}

application {
    mainClass = 'com.api.scrapper.App'
}

test {
    useJUnitPlatform()
}

jar {
    manifest {
        attributes 'Main-Class': 'com.api.scrapper.App'
    }
    from {
        configurations.runtimeClasspath.collect { it.isDirectory() ? it : zipTree(it) }
    }
    duplicatesStrategy = DuplicatesStrategy.EXCLUDE
}
