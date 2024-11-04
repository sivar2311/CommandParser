#include <Arduino.h>
#include <CommandParser.h>

CommandParser cp;

void handleHello(const std::vector<String>& params) {
    if (params.size() > 2) {
        Serial.printf("Hello %s! Nice to meet you!\r\n", params[1].c_str());
    } else {
        Serial.println("Hello unknown!");
    }
}

void handleIP(const std::vector<String>& params) {
    if (params.size() < 2) {
        Serial.println("Please provide an IP address as parameter");
        return;
    }

    IPAddress ip;
    if (ip.fromString(params[1])) {
        Serial.println(ip);
    } else {
        Serial.println("Invalid IP address!");
    }
}

void handleTemp(const std::vector<String>& params) {
  if (params.size() < 2) {
    Serial.println("Please provide a temperature as parameter");
    return;
  }

  float temperature = params[1].toFloat();
  Serial.printf("Temperature set to %f\r\n", temperature);
}

void handleCount(const std::vector<String>& params) {
    if (params.size() < 3) {
        Serial.println("Count <from> <to>");
        return;
    }

    size_t from = params[1].toInt();
    size_t to = params[2].toInt();

    for (size_t i = from; i<=to; i++) {
        Serial.println(i);
    }
}


void handleNotFound(const std::vector<String>& params) {
    Serial.printf("Command \"%s\" is not implemented!\r\n", params[0].c_str());
}

void handleOverflow() {
    Serial.println("Buffer overflow!");
}

void setup() {
    Serial.begin(115200);
    Serial.println("Hello World");

    cp.onCommand("hello", handleHello);
    cp.onCommand("ip", handleIP);
    cp.onCommand("temp", handleTemp, CommandParser::CaseSensivity::IGNORE);
    cp.onCommand("count", handleCount);
    cp.onOverflow(handleOverflow);
    cp.onNotFound(handleNotFound);
}

void loop() {
    cp.processFromStream(Serial);
}