#include "CommandParser.h"

static const char* delimiters = " \r\n";

CommandParser::CommandHandlerInfo::CommandHandlerInfo(const String& command, CommandHandler handler, CaseSensivity cs)
    : cmd(command)
    , handler(handler)
    , cs(cs) {}

CommandParser::CommandParser(size_t buffer_size)
    : buffer_size(buffer_size) {
    buffer = new char[buffer_size];
}

CommandParser::~CommandParser() {
    delete[] buffer;
}

void CommandParser::addChar(char c) {
    char NewLine        = '\n';
    char CarriageReturn = '\r';

    bool isCommand  = c == NewLine || c == CarriageReturn;
    bool isOverflow = index >= buffer_size;

    if (isCommand && index) {
        buffer[index] = '\0';
        handleCommand();
    }

    if (isOverflow) handleOverflow();

    if (isCommand || isOverflow) return resetIndex();

    buffer[index++] = c;
}

void CommandParser::processFromStream(Stream& stream, bool echo) {
    while (stream.available()) {
        char c = stream.read();
        if (echo) stream.write(c);
        addChar(c);
    }
}

void CommandParser::onCommand(const String& command, CommandHandler handler, CaseSensivity cs) {
    commandHandlers.push_back(CommandHandlerInfo(command, handler, cs));
}

void CommandParser::onNotFound(CommandHandler handler) {
    notFoundHandler = handler;
}

void CommandParser::onOverflow(OverflowHandler handler) {
    overflowHandler = handler;
}

void CommandParser::resetIndex() {
    index = 0;
}

void CommandParser::handleOverflow() {
    if (overflowHandler) overflowHandler();
    resetIndex();
}

static const std::vector<String> splitString(char* buffer, const char* delimiter) {
    std::vector<String> result;
    if (!buffer) return result;

    char* token = strtok(buffer, delimiter);
    while (token) {
        result.push_back(String(token));
        token = strtok(nullptr, delimiter);
    }

    return result;
}

static const std::vector<String> splitString(const String& line, const char* delimiter) {
    return splitString((char*)line.c_str(), delimiter);
}

void CommandParser::handleCommand() {
    auto parameters = splitString(buffer, delimiters);

    if (parameters.size() == 0) return;
    String command = parameters[0];

    for (auto& handlerInfo : commandHandlers) {
        bool found = false;

        if ((handlerInfo.cs == CaseSensivity::IGNORE && handlerInfo.cmd.equalsIgnoreCase(command)) ||
            (handlerInfo.cs == CaseSensivity::EQUALS && handlerInfo.cmd.equals(command))) {
            handlerInfo.handler(parameters);
            resetIndex();
            return;
        }
    }

    if (notFoundHandler) notFoundHandler(parameters);

    resetIndex();
}