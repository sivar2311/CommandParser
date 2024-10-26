#include "CommandParser.h"

static const char* delimiters = " \r\n";

CommandParser::CommandParser(size_t buffer_size)
    : buffer_size(buffer_size) {
    buffer = new char[buffer_size];
}

CommandParser::~CommandParser() {
    delete[] buffer;
}

void CommandParser::addChar(char c) {
    bool isCommand  = c == '\n' || c == '\r';
    bool isOverflow = index >= buffer_size;

    if (isCommand && index) {
        buffer[index] = '\0';
        handleCommand();
    }

    if (isOverflow) handleOverflow();

    if (isCommand || isOverflow) return resetIndex();

    buffer[index++] = c;
}

void CommandParser::processFromStream(Stream& stream) {
    while (stream.available()) addChar(stream.read());
}

void CommandParser::onCommand(const String& command, CommandHandler handler) {
    commandHandlers[command] = handler;
}

void CommandParser::onOverflow(OverflowHandler handler) {
    overflowHandler = handler;
}

void CommandParser::resetIndex() {
    index = 0;
}

void CommandParser::handleOverflow() {
    resetIndex();
    if (overflowHandler) overflowHandler();
}

void CommandParser::handleCommand() {
    ParameterList pl;
    char*         _command;
    _command = strtok(buffer, delimiters);

    String command(_command);

    char* _parameter = strtok(NULL, delimiters);
    while (_parameter) {
        pl.push_back(String(_parameter));
        _parameter = strtok(NULL, delimiters);
    };

    auto handler = commandHandlers[command];
    if (handler) handler(command, pl);

    resetIndex();
}
