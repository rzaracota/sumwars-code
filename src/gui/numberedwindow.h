#ifndef NUMBERED_WINDOW_H
#define NUMBERED_WINDOW_H

// CEGUI includes
#include "CEGUI.h"

/**
 * \class NumberedWindow
 * \brief Fenster mit einer bestimmten Nummer, die bei einem Mausklick mit ausgegeben wird
 */
class NumberedWindow : public CEGUI::Window
{
	public:
	
	/**
	 * \fn NumberedWindow (const CEGUI::String &type, const CEGUI::String &name)
	 * \brief Konstruktor
	 * \param type Typ des Fensters
	 * \param Name des Fensters
	 */
	NumberedWindow (const CEGUI::String &type, const CEGUI::String &name);
	
	/**
	 * \fn ~NumberedWindow()
	 * \brief Destruktor
	 */
	~NumberedWindow() {}
	
	/**
	 * \fn void setNumber(int i)
	 * \brief setzt die Nummer des Fensters
	 */
	void setNumber(int i)
	{
		m_number =i;
	}
	
	/**
	 * \fn int getNumber()
	 * \brief Gibt die Nummer des Fenster aus
	 */
	int getNumber()
	{
		return m_number;
	}
	

	private:
		int m_number;
};

/**
 * \class NumberedWindowFactory
 * \brief Factory, die Objekte von Typ NumberedWindow erzeugt
 */
class NumberedWindowFactory : public CEGUI::WindowFactory
{
	public:
	/**
	 * \fn NumberedWindowFactory()
	 * \brief Konstruktor
	 */
	NumberedWindowFactory()
	: CEGUI::WindowFactory("NumberedWindow")
	{
	}
	
	/**
	 * \fn virtual CEGUI::Window *  createWindow (const CEGUI::String &name)
	 * \brief erzeugt ein Fenster vom Typ NumberedWindow
	 * \return Zeiger auf das erzeugte Fenster
	 */
	virtual CEGUI::Window *  createWindow (const CEGUI::String &name);
	
	/**
	 * \fn virtual void  destroyWindow (CEGUI::Window *window)
	 * \brief Zerstoert das uebergebene Objekt
	 * \param window zu zerstoerendes Objekt
	 */
	virtual void  destroyWindow (CEGUI::Window *window);
};	

#endif

