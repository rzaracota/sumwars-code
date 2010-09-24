
#include <stdio.h>
#include "application.h"

#if defined(WIN32)
#include "windows.h"

#endif
//INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
int main (int argc, char *argv[]) {

	try
	{

	// Applikation anlegen
	Application* app = new Application(argv[0]);

	// Debugging: Kommandozeilen Parameter auslesen und Savefile setzen
	std::string save;
	#ifndef WIN32
	if (argc>=2)
	{
		save = "./save/";
		save += argv[1];
	}
	else
	{
		save = "./save/default.sav";
	}
	#else
        save = "./save/default.sav";
	#endif

	bool server = true;
	if (argc>=3)
	{
		if (std::string(argv[2]) == "-c")
		{
			server = false;
		}
	}
	
	if (argc>=2)
	{
		Document* doc = app->getDocument();
		doc->setSaveFile(save);
		
		doc->setServer(server);
		doc->setState(Document::START_GAME);
		doc->getGUIState()->m_sheet= Document::GAME_SCREEN;
		doc->getGUIState()->m_shown_windows = Document::NO_WINDOWS;
	}
	// laufen lassen

	/*
	try
	{

		app->run();
	}
	catch (std::exception &e)
	{
		ERRORMSG("Error message: %s",e.what());
	}
	*/
	app->run();
	
	
	// Applikation loeschen
	delete app;

	}
	/*
	catch (std::exception& e)
	{
		DEBUG("Programm died with exception %s",e.what());
	}
	*/
	catch (CEGUI::Exception& e)
	{
		DEBUG("Programm died with exception %s",e.getMessage().c_str());
	}
	// Ende
	return 0;
}
