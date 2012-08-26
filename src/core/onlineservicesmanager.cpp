#include "onlineservicesmanager.h"
#include "config.h"
#include "debug.h"
#include "tinyxml.h"
#include "OgreRoot.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/StreamCopier.h>
#include <Poco/Base64Encoder.h>
#include <Poco/Buffer.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/Task.h>
#include <Poco/TaskManager.h>
#include <Poco/TaskNotification.h>
#include <Poco/Observer.h>

#include <string>
#include <iostream>
#include <fstream>

#include <OgreResourceGroupManager.h>

//std::string backend_url = "http://localhost:8081";
std::string backend_url = "http://sumwars-backend.appspot.com";

class LoginTask: public Poco::Task
{
public:
    std::string username;
    std::string pw;
    std::string sw_token;
    std::string dataPath;
    std::string userDataPath;
    std::vector<OnlineServicesManager::CharacterLight*> characters;

    LoginTask(const std::string& name, std::string username, std::string pw, std::string dataPath): Task(name)
    {
        sw_token = "";
        this->dataPath = dataPath;
        characters.clear();

        this->username = username;
        this->pw = pw;
    }

    void runTask()
    {
        sw_token = "";
        characters.clear();

        try
        {
            // prepare session

            Poco::URI uri(backend_url + "/api/remote_login?username=" + this->username + "&password=" + this->pw);
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

            // prepare path
            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";

            // send request
            Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
            session.sendRequest(req);

            // get response
            Poco::Net::HTTPResponse res;
            std::cout << res.getStatus() << " " << res.getReason() << std::endl;

            // print response
            std::istream &is = session.receiveResponse(res);
            std::ostringstream stream;
            Poco::StreamCopier::copyStream(is, stream);

            std::string temp = stream.str();
            //std::cout << temp << std::endl;
            if(temp == "401")
            {
                std::cout << "Username or password is wrong" << std::endl;
                this->cancel();
            }
            else
            {
                TiXmlDocument doc;
                if(!doc.Parse(temp.c_str()))
                {
                    std::cerr << doc.ErrorDesc() << std::endl;
                }

                TiXmlElement* root = doc.FirstChildElement();
                if(root == NULL)
                {
                    std::cerr << "Failed to load file: No root element." << std::endl;
                    doc.Clear();
                }

                Poco::Path p;
                p.parse(dataPath);
                p.append("save/" + username);
                userDataPath = p.toString();

                Poco::File pf(p.toString());
                pf.createDirectories();

                for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
                {
                    std::string val = elem->Value();
                    if(val == "token")
                    {
                        sw_token = elem->Attribute("value");
                    }
                    else if(val == "Character")
                    {
                        OnlineServicesManager::CharacterLight *cl = new OnlineServicesManager::CharacterLight();
                        elem->Attribute("level", &cl->level);
                        cl->subtype = elem->Attribute("subtype");
                        cl->name = elem->Attribute("name");
                        cl->savegame = std::string(elem->GetText());
                        characters.push_back(cl);

                        std::ofstream f;
                        f.open((p.toString() + "/" + cl->name + ".sav").c_str());
                        f << cl->savegame;
                        f.close();
                    }
                }

                std::cout << "Login finished. Retrieved " << characters.size() << " character infos.";
            }
        }
        catch(Poco::Exception &ex)
        {
        }
    }
};

class LogoutTask: public Poco::Task
{
public:
    std::string sw_token;

    LogoutTask(const std::string& name, std::string token): Task(name)
    {
        sw_token = token;
    }

    void runTask()
    {

        try
        {
            // prepare session
            Poco::URI uri(backend_url + "/api/remote_logout?token=" + sw_token);
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

            // prepare path
            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";

            // send request
            Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
            session.sendRequest(req);

            // get response
            Poco::Net::HTTPResponse res;
            std::cout << res.getStatus() << " " << res.getReason() << std::endl;

            // print response
            std::istream &is = session.receiveResponse(res);
            std::ostringstream stream;
            Poco::StreamCopier::copyStream(is, stream);

            std::string temp = Poco::replace(stream.str(), "{", "");
            std::cout << temp << std::endl;
            sw_token = "";
        }
        catch(Poco::Exception &ex)
        {
        }
    }
};

class SyncCharTask: public Poco::Task
{
public:
    std::string sw_token;
    std::string sw_charname;
    std::string sw_chardata;

    SyncCharTask(const std::string& name, std::string token, std::string charname, std::string chardata): Task(name)
    {
        sw_token = token;
        sw_charname = charname;
        sw_chardata = chardata;
    }

    void runTask()
    {

        try
        {

            // prepare session
            Poco::URI uri(backend_url + "/api/update_character_data");
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

            // prepare path
            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";

            // send request
            Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, path, Poco::Net::HTTPMessage::HTTP_1_1);

            std::string body = "<Post><Character charname='" + sw_charname +"' >";
            body += sw_chardata;
            body += "</Character>";
            body += "<Token value='" + sw_token + "'/></Post>";

            req.setContentLength(body.length());
            req.setContentType("text/xml; charset=utf-8");
            std::ostream& send = session.sendRequest(req);
            send << body << std::flush;

            // get response
            Poco::Net::HTTPResponse res;
            std::cout << "CharSyncTask: " << res.getStatus() << " " << res.getReason() << std::endl;

            // print response
            std::istream &is = session.receiveResponse(res);
            std::ostringstream stream;
            Poco::StreamCopier::copyStream(is, stream);
            std::cout << stream.str() << std::endl;
        }
        catch(Poco::Exception &ex)
        {
        }
    }
};

class SyncCharAvatarTask: public Poco::Task
{
public:
    std::string sw_token;
    std::string sw_charname;
    std::string sw_avatar_file;
    Ogre::DataStreamPtr mImgPtr;

    SyncCharAvatarTask(const std::string& name, std::string token, std::string charName, std::string charAvatarPath, Ogre::DataStreamPtr ptr): Task(name)
    {
        sw_token = token;
        sw_charname = charName;
        sw_avatar_file = charAvatarPath;
        mImgPtr = ptr;
    }

    void runTask()
    {

        try
        {
            // prepare session
            Poco::URI uri(backend_url + "/api/update_character_avatar");
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

            // prepare path
            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";

            Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, path, Poco::Net::HTTPRequest::HTTP_1_1);
            Poco::Net::HTMLForm form;
            form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
            form.add("token", sw_token);
            form.add("charname", sw_charname);

            Poco::Buffer<char> imgBuffer(mImgPtr->size());
            mImgPtr->read(imgBuffer.begin(), imgBuffer.end()-imgBuffer.begin());
            std::string s(imgBuffer.begin(), mImgPtr->size());

            std::ostringstream out;
            Poco::Base64Encoder b64enc(out);
            b64enc.write(imgBuffer.begin(), imgBuffer.end()-imgBuffer.begin());
            b64enc.close();
            Poco::Net::StringPartSource *prtsrc = new Poco::Net::StringPartSource(out.str());
            form.addPart("imagedata", prtsrc);

            std::ostringstream ostr;
            form.write(ostr);
            req.setContentLength(ostr.str().length());

            std::ostream& send = session.sendRequest(req);
            form.prepareSubmit(req);
            form.write(send);

            // get response
            Poco::Net::HTTPResponse res;
            std::cout << "SyncCharAvatarTask: " << res.getStatus() << " " << res.getReason() << std::endl;

            // print response
            std::istream &is = session.receiveResponse(res);
            std::ostringstream stream;
            Poco::StreamCopier::copyStream(is, stream);
            std::cout << stream.str() << std::endl;

/*            std::ostringstream out;
            Poco::Base64Encoder b64enc(out);
            b64enc << sw_token << sw_charname << imgBuffer;
            b64enc.close();

            // prepare session
            Poco::URI uri(backend_url + "/api/update_character_avatar");
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

            // prepare path
            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";

            // send request
            Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, path, Poco::Net::HTTPMessage::HTTP_1_1);

            //req.setContentLength(50+mImgPtr->size());
            //req.setContentLength((4*mImgPtr->size()+200+2)/3);
            req.setContentLength(out.str().size());
            req.setContentType("application/octet-stream");
            std::ostream& send = session.sendRequest(req);
            send << out.str() << std::flush;

            // get response
            Poco::Net::HTTPResponse res;
            std::cout << "SyncCharAvatarTask: " << res.getStatus() << " " << res.getReason() << std::endl;

            // print response
            std::istream &is = session.receiveResponse(res);
            std::ostringstream stream;
            Poco::StreamCopier::copyStream(is, stream);
            std::cout << stream.str() << std::endl;
			*/
//            delete[] imgBuffer;
        }
		catch(Poco::Exception &ex)
		{
            std::cout << "Error syncing character avatar image: " << ex.className() << " " << ex.displayText() << std::endl;
		}
    }
};



template<> OnlineServicesManager* Ogre::Singleton<OnlineServicesManager>::SUMWARS_OGRE_SINGLETON = 0;

OnlineServicesManager::OnlineServicesManager(std::string &dataPath)
{
    mTaskManager = new Poco::TaskManager();
    mTaskManager->addObserver(Poco::Observer<OnlineServicesManager, Poco::TaskProgressNotification>(*this, &OnlineServicesManager::onProgress));
    mTaskManager->addObserver(Poco::Observer<OnlineServicesManager, Poco::TaskFinishedNotification>(*this, &OnlineServicesManager::onFinished));
    mDataPath = dataPath;
    mUserLoggedIn = false;
    mUserDataPath = "";
    mUserDataResGroupId = "Savegame";
    mCurrentUsername = "Default";
    mToken = "";
}

void OnlineServicesManager::onProgress(Poco::TaskProgressNotification *notification)
{
    notification->release();
}

void OnlineServicesManager::onFinished(Poco::TaskFinishedNotification *notification)
{
    std::string tName = notification->task()->name();
    if(tName == "Login Task")
    {
        mToken = ((LoginTask*)notification->task())->sw_token;

        mUserDataPath = ((LoginTask*)notification->task())->userDataPath;
        mUserDataResGroupId = "Savegame_" + ((LoginTask*)notification->task())->username;
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mUserDataPath, "FileSystem", mUserDataResGroupId);
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(mUserDataResGroupId);
        mCurrentUsername = ((LoginTask*)notification->task())->username;
        mUserLoggedIn = true;

        std::vector<StatusListener*>::iterator iter;
        for(iter = mStatusListener.begin(); iter != mStatusListener.end(); iter++)
            (*iter)->onLoginFinished(((LoginTask*)notification->task())->characters);

    }
    else if(tName == "Logout Task")
    {
        mUserDataPath = "";
        Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(mUserDataResGroupId);
        mUserDataResGroupId = "Savegame";
        mCurrentUsername = "Default";
        mUserLoggedIn = false;

        std::vector<StatusListener*>::iterator iter;
        for(iter = mStatusListener.begin(); iter != mStatusListener.end(); iter++)
            (*iter)->onLogoutFinished();
    }
    else if(tName == "Sync Character Task")
    {
        std::vector<StatusListener*>::iterator iter;
        for(iter = mStatusListener.begin(); iter != mStatusListener.end(); iter++)
            (*iter)->onSyncCharFinished();
    }
    else if(tName == "Sync Character Avatar Task")
    {
        // do nothing for now
    }

    notification->release();
}

bool OnlineServicesManager::syncCharacterData(std::string charName, std::string data)
{
    if(Ogre::Root::getSingletonPtr())
    {
        unsigned long timeSinceLastSync = Ogre::Root::getSingleton().getTimer()->getMilliseconds() - mLastSync;
        if(timeSinceLastSync > 2000)
        {
            mTaskManager->start(new SyncCharTask("Sync Character Task", mToken, charName, data));
            mLastSync = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
        }
    }
    return true;
}

bool OnlineServicesManager::syncCharacterAvatar(std::string charName, std::string pathToImage, Ogre::DataStreamPtr ptr)
{
    std::cout << "syncing avatar image" << std::endl;
    mTaskManager->start(new SyncCharAvatarTask("Sync Character Avatar Task", mToken, charName, pathToImage, ptr));
    return true;
}

bool OnlineServicesManager::login(std::string userName, std::string password)
{
    mTaskManager->start(new LoginTask("Login Task", userName, password, mDataPath));
    return true;
}

bool OnlineServicesManager::logout()
{
    mTaskManager->start(new LogoutTask("Logout Task", mToken));
    return true;
}

void OnlineServicesManager::registerLoginStatusListener(StatusListener* l)
{
    std::vector<StatusListener*>::iterator iter;
    for(iter = mStatusListener.begin(); iter != mStatusListener.end(); iter++)
        if(*iter == l)
            return;

    mStatusListener.push_back(l);
}

void OnlineServicesManager::unregisterLoginStatusListener(StatusListener* l)
{
    std::vector<StatusListener*>::iterator iter;
    for(iter = mStatusListener.begin(); iter != mStatusListener.end(); iter++)
        if(*iter == l)
            mStatusListener.erase(iter);
}

OnlineServicesManager* OnlineServicesManager::getSingletonPtr(void)
{
    return SUMWARS_OGRE_SINGLETON;
}

OnlineServicesManager& OnlineServicesManager::getSingleton(void)
{
    assert( SUMWARS_OGRE_SINGLETON );
    return ( *SUMWARS_OGRE_SINGLETON );
}
