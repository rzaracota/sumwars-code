#ifndef INI_H
#define INI_H

#include "string"
#include "windows.h"

/*
CIniFile
Klasse zum arbeiten mit INI-Files
Für Kassadaja
(c) Johannes Leimbach
*/

class CIniFile
{
public :
	//Bevor mit Dateien gearbeitet werden kann muss dem System
	//hiermit mitgeteilt werden welche Datei benutzt werden soll
	void SetFileName (std::string filename) {m_strFileName=filename;};

	//Liefert den Namen der aktuellen Datei
	void GetFileName (std::string& filename) {filename=m_strFileName;};

	//Eigentlich selbsterklärend. Standardparameter beachten ;-)
	//Wenn man int's schreiben will muss man diese zuerst
	//z.b. mit wsprintf in einen char packen und dann als
	//parameter übergeben
	BOOL WriteString (std::string section = "GENERAL",
					  std::string key = "Unitname",
					  std::string value = "HeavyTank");

	
	//Naja, liefert einen int in value zurück.
	//Floats können nicht gespeichert werden.
	//Wenn man z.b. 1.9 speichern will, schreibt man 1.9*10, und
	//übergibt das
	//Wenn der Schlüssel nicht existiert, gibt die Funktion
	//value = defaultValue zurück
	BOOL GetIntValue (int& value,
					  std::string section = "GENERAL",
					  std::string key = "Unitstrength",
					  int defaultValue =0);

	//Liest einen Stringwert in buffer rein
	//Die Buffergröße ist 256 Zeichen groß (wird intern geregelt);
	BOOL GetStringValue (std::string& buffer,
					     std::string section = "GENERAL",
						 std::string key = "Unitstrength");


private :
	//DateiName
	std::string m_strFileName;
};

#endif