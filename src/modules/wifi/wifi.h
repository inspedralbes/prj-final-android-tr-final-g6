namespace wifi {
    struct WiFiConfig {
        String ssid;
        String password;
    };

    bool loadWiFiConfig(WiFiConfig& config);
    bool connectToWiFi();
}