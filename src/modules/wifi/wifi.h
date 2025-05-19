namespace wifi {
    struct WiFiConfig {
        String ssid;
        String password;
    };

    bool loadWiFiConfig(WiFiConfig& config);
    bool connectToWiFi();
    String getMacAddress();
    String getIp();
    bool isConnected();
}