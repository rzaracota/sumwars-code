
#include <stdio.h>
#include "client.h"

#if defined(WIN32)
#include "windows.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main (int argc, char *argv[]) {
#endif


	// Applikation anlegen
	Client* app = new Client();

	// Debugging: Kommandozeilen Parameter auslesen und Savefile setzen
	string save;
	#ifndef WIN32
	if (argc>=2)
	{
		save = argv[1];
	}
	else
	{
		save = "save/default.sav";
	}
	#else
        save = "save/default.sav";
    #endif
	Document* doc = app->getDocument();
	doc->setSaveFile(save);
	doc->setState(Document::CONNECT_REQUEST);
	doc->getGUIState()->m_sheet= Document::GAME_SCREEN;
	doc->getGUIState()->m_shown_windows = Document::NO_WINDOWS;

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
