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
    std::string message(text);
    std::transform(message.begin(), message.end(), message.begin(), ::tolower);

    bool messageChanged = false;

    uint32_t wordCount = config->FetchArraySize("swiftly_wordfilter.words");

    for (uint32_t i = 0; i < wordCount; ++i) {
        std::string wordKey = "swiftly_wordfilter.words[" + std::to_string(i) + "]";
        std::string word = config->Fetch<const char*>(wordKey.c_str());

        std::size_t found = message.find(word);
        if (found != std::string::npos) {
            message = FetchTranslation("wordfilter.notallowed");
            messageChanged = true;
            break;
        }
    }

    if (messageChanged) {
        player->SendMsg(HUD_PRINTTALK, message.c_str());
        return false;
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