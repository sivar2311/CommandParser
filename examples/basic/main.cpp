#include <Arduino.h>
#include <CommandParser.h>

CommandParser cp;

void printHelpText() {
    Serial.println(R"(supported commands:
  help : shows this text
  hello <name>: shows a welcome text.)");
}

void handleHello(const String& command, const CommandParser::ParameterList& params) {
    if (params.size() < 1) {
        Serial.println("This command excepts at least one paramater!");
        printHelpText();
        return;
    }

    Serial.printf("Hello %s! Nice to meet you!", params[0].c_str());
}

void handleHelp(const String& command, const CommandParser::ParameterList& params) {
    printHelpText();
}

void setup() {
    Serial.begin(115200);
    Serial.println("Hello World");

    cp.onCommand("hello", handleHello);
    cp.onCommand("help", handleHelp);
}

void loop() {
    cp.processFromStream(Serial);
}