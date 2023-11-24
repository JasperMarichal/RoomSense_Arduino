#include <ESP_EEPROM.h>

#include <ESP8266WiFi.h>
#include <ArduinoWiFiServer.h>

#define EEPROM_RESET_ON_CHECKSUM_FAIL
#define MAX_SERIALCMD_LENGTH 70
#define DEFAULT_SERVER_PORT 80

struct EEPROMDataStruct {
  uint32_t checksum;
  uint16_t serverPort;
  char ssid[64];
  char password[64];
  uint32_t ip_address;
  uint32_t gw_address;
  uint32_t subnet_mask;
} eepromData;


ArduinoWiFiServer server(DEFAULT_SERVER_PORT);

String sbuff;

void setup() {
  delay(1000);
  sbuff = "";
  Serial.begin(57600);

  Serial.println("\n--- ESP Rebooted ---");
  Serial.print("DEVICE MAC: "); Serial.println(WiFi.macAddress());

  EEPROM.begin(sizeof(EEPROMDataStruct));
  Serial.print("EEPROM USAGE: "); Serial.print(EEPROM.percentUsed()); Serial.println("%");

  Serial.print("CONFIG (EEPROM): ");
  Serial.println(reloadFromEEPROM() ? (eepromData.checksum == 0 ? "EMPTY" : "LOADED") : "!CHECKSUM");

  showConfig();
}

const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
  return "?";
}

uint32_t calculateEEPROMChecksum() {
  uint32_t _checksum = 0;
  _checksum += eepromData.serverPort;
  for(int i = 0; i < 64; i++) {
    _checksum += (uint8_t) eepromData.ssid[i];
    _checksum += (uint8_t) eepromData.password[i];
  }
  _checksum += eepromData.ip_address;
  _checksum += eepromData.gw_address;
  _checksum += eepromData.subnet_mask;
  return _checksum;
}

bool reloadFromEEPROM() {
  EEPROM.get(0, eepromData);
  bool load_checksum = calculateEEPROMChecksum() == eepromData.checksum;
  #ifdef EEPROM_RESET_ON_CHECKSUM_FAIL
  if(!load_checksum) resetConfig();
  #endif

  return load_checksum;
}

void stopStuff() {
  server.close();
  while(server.status()) delay(1);
  Serial.println("Server: STOPPED");
  WiFi.disconnect();
  Serial.print("WiFi: DISCONNECTED ("); Serial.print(wl_status_to_string(WiFi.status())); Serial.println(")");
}

void resetConfig() {
  Serial.println("Resetting configuration...");
  stopStuff();
  eepromData = {0};
  EEPROM.put(0, eepromData);
  Serial.print("Configuration RESET "); Serial.println(EEPROM.commitReset() ? "SUCCESSFULLY" : "COMMIT FAIL");
  if(eepromData.checksum == 0) Serial.println("You can now type config commands.");
}

void showConfig() {
  Serial.println("----- CURRENT CONFIG -----");
  Serial.print("WiFi SSID: "); Serial.println(eepromData.ssid);
  Serial.print("WiFi Password: "); Serial.println(eepromData.password);
  Serial.print("Telnet server port: "); Serial.println(eepromData.serverPort);
  Serial.print("IP address: "); Serial.println(IPAddress(eepromData.ip_address));
  Serial.print("GW address: "); Serial.println(IPAddress(eepromData.gw_address));
  Serial.print("Subnet Mask: "); Serial.println(IPAddress(eepromData.subnet_mask));
  Serial.print("checksum: "); Serial.print(eepromData.checksum);
  if(eepromData.checksum != calculateEEPROMChecksum()) Serial.print(" MOD");
  if(eepromData.checksum == 0) Serial.print(" -> CONFIG OPEN");
  Serial.println("\n"); 
}

void showStatus() {
  Serial.println("----- CURRENT STATUS -----");
  Serial.print("WiFi Status: ");
  Serial.println(wl_status_to_string(WiFi.status()));

  Serial.print("IP address = ");
  Serial.println(WiFi.localIP());
  Serial.print("GW address = ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS address = ");
  Serial.println(WiFi.dnsIP());  
  Serial.print("SUBNET mask = ");
  Serial.println(WiFi.subnetMask());

  Serial.print("Telnet Server: ");
  if(server.status()) Serial.println("RUNNING");
  else Serial.println(server.status());
  Serial.println("");
}

uint32_t parseIPAddressString(String str) {
  char addr[16];
  str.toCharArray(addr, 16);

  char * octetStr;
  octetStr = strtok(addr, ".");

  uint32_t address = 0;
  for(int8_t o = 0; o < 4; o++) {
    address = address >> 8;
    if(octetStr != NULL) {
      uint32_t octet = ((uint32_t) atoi(octetStr)) << 24;
      address |= octet;
      octetStr = strtok(NULL, ".");
    }
  }
  return address;
}

void processCommand(String sbuff) {
  if(sbuff == "STOP") {
    stopStuff(); 
  }else if(sbuff == "STATUS") {
    showStatus(); 
  }else if(sbuff == "CONFIG SHOW") {
    showConfig(); 
  }else if(sbuff == "CONFIG RESET") {
    resetConfig();
  }else if(sbuff == "CONFIG EDIT") {
    stopStuff();
    eepromData.checksum = 0;
    if(eepromData.checksum == 0) Serial.println("You can now type config commands.");
  }else if(sbuff == "CONFIG COMMIT") {
    if(eepromData.checksum == calculateEEPROMChecksum()) {
      Serial.println("No modification detected. Not doing anything.");
    }else {
      Serial.print("Recalculating checksum... ");
      eepromData.checksum = calculateEEPROMChecksum();
      Serial.println(eepromData.checksum);

      Serial.print("Writing to EEPROM... ");
      EEPROM.put(0, eepromData);
      Serial.println(EEPROM.commit() ? "OKAY" : "COMMIT FAIL");
    }
  }else if(sbuff == "CONFIG RESTORE") {
    Serial.print("Reloading from eeprom... "); Serial.println(reloadFromEEPROM() ? "DONE" : "CHECKSUM ERROR");
  }else if(eepromData.checksum == 0 && sbuff.startsWith("SSID ")) {
    sbuff.substring(5).toCharArray(eepromData.ssid, 64);
    Serial.print("SSID set to: \""); Serial.print(eepromData.ssid); Serial.println("\"");
  }else if(eepromData.checksum == 0 && sbuff.startsWith("PASS ")) {
    sbuff.substring(5).toCharArray(eepromData.password, 64);
    Serial.print("PASSWORD set to: \""); Serial.print(eepromData.password); Serial.println("\"");
  }else if(eepromData.checksum == 0 && sbuff.startsWith("PORT ")) {
    uint16_t nPort = sbuff.substring(5).toInt();
    eepromData.serverPort = nPort == 0 ? DEFAULT_SERVER_PORT : nPort;
    Serial.print("PORT for telnet server set to: \""); Serial.print(eepromData.serverPort); Serial.println("\"");
  }else if(eepromData.checksum == 0 && sbuff.startsWith("IP ")) {
    eepromData.ip_address = parseIPAddressString(sbuff.substring(3));
    Serial.print("IP address set to: \""); Serial.print(IPAddress(eepromData.ip_address)); Serial.println("\"");
  }else if(eepromData.checksum == 0 && sbuff.startsWith("GW ")) {
    eepromData.gw_address = parseIPAddressString(sbuff.substring(3));
    Serial.print("GW address set to: \""); Serial.print(IPAddress(eepromData.gw_address)); Serial.println("\"");
  }else if(eepromData.checksum == 0 && sbuff.startsWith("SN ")) {
    eepromData.subnet_mask = parseIPAddressString(sbuff.substring(3));
    Serial.print("SN mask set to: \""); Serial.print(IPAddress(eepromData.subnet_mask)); Serial.println("\"");
  }else if(eepromData.checksum == 0 && sbuff.startsWith("DISABLE STATIC")) {
    Serial.print("Resetting static ip configuration... ");
    eepromData.ip_address = 0;
    eepromData.gw_address = 0;
    eepromData.subnet_mask = 0;
    Serial.println("DONE");
  }else if(server.status()) {
    //Transfer all lines not recognized as commands as data to all clients on the telnet server.
    server.print(sbuff); server.print("\n");
  }
}

void loop() {
  while(Serial.available()) {
    char c = (char) Serial.read();
    if(c == '\n' || sbuff.length() > MAX_SERIALCMD_LENGTH) {
      // Process commands
      processCommand(sbuff);
      sbuff = "";
    }else {
      sbuff += c;
    }
  }
  if(eepromData.checksum != 0) {
    if(WiFi.status() != WL_CONNECTED) {
      Serial.print("Connecting to WiFi... ");
      WiFi.config(IPAddress(eepromData.ip_address), IPAddress(eepromData.gw_address), IPAddress(eepromData.subnet_mask));
      WiFi.begin(eepromData.ssid, eepromData.password);
      Serial.println(wl_status_to_string((wl_status_t) WiFi.waitForConnectResult()));

      delay(500);

      showStatus();
    }else {
      if(!server.status()) {
        Serial.print("Starting server on port: "); Serial.println(eepromData.serverPort);
        server.begin(eepromData.serverPort);
      }else {

      }
    }
  }
}
