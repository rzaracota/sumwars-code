#ifndef ONLINESERVICESMANAGER_H
#define ONLINESERVICESMANAGER_H

#include <OgreSingleton.h>
#include "OgreDataStream.h"

namespace Poco
{
    class TaskManager;
	class TaskProgressNotification;
	class TaskFinishedNotification;
}

class OnlineServicesManager : public Ogre::Singleton<OnlineServicesManager>
{
public:
	struct CharacterLight
    {
        std::string name;
        std::string subtype;
        int level;
        std::string savegame;
    };

    class StatusListener
    {
    public:
        virtual void onLoginFinished(std::vector<OnlineServicesManager::CharacterLight*> &characters) = 0;
        virtual void onLogoutFinished() = 0;
        virtual void onSyncCharFinished() = 0;
    };

public:
    OnlineServicesManager(std::string &dataPath);

    bool login(std::string userName, std::string password);
    bool logout();

    bool userLoggedIn() { return mUserLoggedIn; }
    const std::string& getUserName() { return mCurrentUsername; }
    const std::string& getUserDataPath() { return mUserDataPath; }
    const std::string& getUserDataResGroupId() { return mUserDataResGroupId; }
    bool syncCharacterData(std::string charName, std::string data);
    bool syncCharacterAvatar(std::string charName, std::string pathToImage, Ogre::DataStreamPtr ptr);

    void registerLoginStatusListener(StatusListener* l);
    void unregisterLoginStatusListener(StatusListener* l);

    void onProgress(Poco::TaskProgressNotification* notification);
    void onFinished(Poco::TaskFinishedNotification* notification);

    static OnlineServicesManager& getSingleton(void);
    static OnlineServicesManager* getSingletonPtr(void);
private:
    Poco::TaskManager *mTaskManager;
    std::string mCurrentUsername;
    std::string mToken;
    std::string mDataPath;
    std::string mUserDataPath;
    std::string mUserDataResGroupId;
    long mLastSync;
    bool mUserLoggedIn;

    std::vector<StatusListener*> mStatusListener;
};

#endif // ONLINESERVICESMANAGER_H
