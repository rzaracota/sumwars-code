
#include <stdio.h>
#include "application.h"

#if defined(WIN32)
#include "windows.h"

#endif
//INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
int main (int argc, char *argv[]) {


	// Applikation anlegen
	Application* app = new Application();

	// Debugging: Kommandozeilen Parameter auslesen und Savefile setzen
	string save;
	#ifndef WIN32
	if (argc>=2)
	{
		save = argv[1];
	}
	else
	{
		save = "../save/default.sav";
	}
	#else
        save = "../save/default.sav";
    #endif

	bool server = true;
	if (argc>=3)
	{
		if (std::string(argv[2]) == "-c")
		{
			server = false;
		}
	}
	
	/*
	Document* doc = app->getDocument();
	doc->setSaveFile(save);
	
	doc->setServer(server);
	doc->setState(Document::START_GAME);
	doc->getGUIState()->m_sheet= Document::GAME_SCREEN;
	doc->getGUIState()->m_shown_windows = Document::NO_WINDOWS;
	*/
	// laufen lassen

	try
	{

		app->run();
	}
	catch (std::exception &e)
	{
		ERRORMSG("Error message: %s",e.what());
	}



	// Applikation loeschen
	delete app;

	// Ende
	return 0;
}
