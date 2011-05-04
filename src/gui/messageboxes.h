#ifndef MESSAGEBOXES_H
#define MESSAGEBOXES_H


#include "window.h"


/**
 * \class SaveExitWindow
 * \brief Fenster Spiel beenden
 */
class SaveExitWindow : public Window
{
	public:
	/**
	 * \fn SaveExitWindow (Document* doc
	 * \brief Konstruktor
	 */
		SaveExitWindow (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
		/**
		 * \fn virtual void updateTranslation
		 * \brief aktualisiert die Uebersetzungen
		 */
		virtual void updateTranslation();
	
	private:
		/**
		 * \fn bool onExitGameConfirmed(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Spiel beenden
		 */
		bool onExitGameConfirmed(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onExitGameAborted(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Spiel beenden
		 */
		bool onExitGameAborted(const CEGUI::EventArgs& evt);
};

/**
 * \class MessageQuestion
 * \brief Fenster fuer eine Ja / Nein Frage
 */
class MessageQuestionWindow : public Window
{

	public:
		/**
		 * \fn MessageQuestionWindow (Document* doc
		* \brief Konstruktor
		*/
		MessageQuestionWindow (Document* doc, std::string name, std::string question, std::string button1,CEGUI::Event::Subscriber subscriber1,  std::string button2, CEGUI::Event::Subscriber subscriber2);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update()
		{
			
		}
		
		/**
		 * \fn virtual void updateTranslation
		 * \brief aktualisiert die Uebersetzungen
		 */
		virtual void updateTranslation();
		
		/**
		 * \fn void setQuestion(std::string question)
		 * \brief Aendert den Text der Frage
		 */
		void setQuestion(std::string question);
	
	private:
		/**
		 * \var std::string m_name
		 * \brief Name des Fensters
		 */
		std::string m_name;
		
		/**
		 * \var std::string m_question
		 * \brief Text der Frage
		 */
		std::string m_question;
		
		/**
		 * \var std::string m_button1
		 * \brief Beschriftung des ersten Buttons
		 */
		std::string m_button1;
		
		/**
		 * \var std::string m_button2
		 * \brief Beschriftung des zweiten Buttons
		 */
		std::string m_button2;
		
};

/**
 * \class ErrorDialogWindow
 * \brief Dialog for Errors
 */
class ErrorDialogWindow : public Window
{
public:
	/**
	 * \fn SaveExitWindow (Document* doc
	 * \brief Konstruktor
	 */
    ErrorDialogWindow (Document* doc);
	
    /**
     * \fn virtual void update()
     * \brief aktualisiert den Inhalt des Fensters
     */
    virtual void update();
    
    /**
     * \fn virtual void updateTranslation
     * \brief aktualisiert die Uebersetzungen
     */
    virtual void updateTranslation();
    
    /**
     * \fn void setError(std::string error)
     * \brief Set the error text
     */
    void setError(std::string error);
	
private:
    /**
     * \fn bool onConfirmed(const CEGUI::EventArgs& evt)
     * \brief User clicked OK
     */
    bool onConfirmed(const CEGUI::EventArgs& evt);
    
    /**
     * \var std::string m_error
     * \brief The error text
     */
    std::string m_error;
};


#endif

