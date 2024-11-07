#pragma once

#include <Arduino.h>

#include <functional>
#include <map>
#include <vector>

class CommandParser {
  public:
    enum class CaseSensivity {
        EQUALS,
        IGNORE
    };

  public:
    using CommandHandler    = std::function<void(const std::vector<String>&)>;
    using OverflowHandler   = std::function<void(void)>;
    using CommandHandlerMap = std::map<String, CommandHandler>;

  public:
    CommandParser(size_t buffer_size = 64);
    ~CommandParser();

    void addChar(char c);
    void processFromStream(Stream& stream, bool echo = false);

    void onCommand(const String& command, CommandHandler handler, CaseSensivity cs = CaseSensivity::EQUALS);
    void onNotFound(CommandHandler handler);
    void onOverflow(OverflowHandler handler);

  protected:
    void resetIndex();
    void handleOverflow();
    void handleCommand();

  protected:
    struct CommandHandlerInfo {
        CommandHandlerInfo(const String& command, CommandHandler handler, CaseSensivity cs);
        String         cmd;
        CommandHandler handler;
        CaseSensivity  cs;
    };

  protected:
    size_t                          buffer_size;
    char*                           buffer;
    size_t                          index = 0;
    std::vector<CommandHandlerInfo> commandHandlers;
    OverflowHandler                 overflowHandler = nullptr;
    CommandHandler                  notFoundHandler = nullptr;
};
