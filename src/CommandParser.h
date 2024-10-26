#pragma once

#include <Arduino.h>

#include <functional>
#include <map>
#include <vector>

class CommandParser {
  public:
    using ParameterList   = std::vector<String>;
    using CommandHandler  = std::function<void(const String& command, const ParameterList& parameters)>;
    using OverflowHandler = std::function<void(void)>;
    using CommandMap      = std::map<String, CommandHandler>;

  public:
    CommandParser(size_t buffer_size = 64);
    ~CommandParser();

    void addChar(char c);
    void processFromStream(Stream& stream);

    void onCommand(const String& command, CommandHandler handler);
    void onOverflow(OverflowHandler handler);

  protected:
    void resetIndex();
    void handleOverflow();
    void handleCommand();

  protected:
    size_t          buffer_size;
    char*           buffer;
    size_t          index = 0;
    CommandMap      commandHandlers;
    OverflowHandler overflowHandler = nullptr;
};
