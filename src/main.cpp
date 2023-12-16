#include <swiftly/swiftly.h>
#include <swiftly/server.h>
#include <swiftly/database.h>
#include <swiftly/commands.h>
#include <swiftly/configuration.h>
#include <swiftly/logger.h>
#include <swiftly/timers.h>
#include <swiftly/gameevents.h>

Server *server = nullptr;
PlayerManager *g_playerManager = nullptr;
Database *db = nullptr;
Commands *commands = nullptr;
Configuration *config = nullptr;
Logger *logger = nullptr;
Timers *timers = nullptr;

void OnProgramLoad(const char *pluginName, const char *mainFilePath)
{
    Swiftly_Setup(pluginName, mainFilePath);

    server = new Server();
    g_playerManager = new PlayerManager();
    commands = new Commands(pluginName);
    config = new Configuration();
    logger = new Logger(mainFilePath, pluginName);
    timers = new Timers();
}



bool OnPlayerChat(Player *player, const char *text, bool teamonly)
{
    std::string message(ToLower(text));
    uint32_t wordCount = config->FetchArraySize("swiftly_wordfilter.words");

    for (uint32_t i = 0; i < wordCount; i++) {
        std::string word = config->Fetch<const char*>("swiftly_wordfilter.words[%d]", i);

        if (message.find(word) != std::string::npos) {
            player->SendMsg(HUD_PRINTTALK, FetchTranslation("wordfilter.notallowed"));
            return false;
        }
    }

    return true;
}

void OnPluginStart()
{
}

void OnPluginStop()
{
}

const char *GetPluginAuthor()
{
    return "blu1337";
}

const char *GetPluginVersion()
{
    return "1.0.0";
}

const char *GetPluginName()
{
    return "Simple word filter for CS2 made in Swfitly.";
}

const char *GetPluginWebsite()
{
    return "blusourcemod.shop";
}