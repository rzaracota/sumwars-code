#ifndef SCRIPTOBJECT_H
#define SCRIPTOBJECT_H

#include "worldobject.h"

/**
 * \class ScriptObject
 * \brief Klasse fuer ausschlieslich durch Scripte gesteuerte Objekte
 */
class ScriptObject: public WorldObject
{
	public:
		/**
		 * \fn ScriptObject(int id)
		 * \brief Konstruktor
		 * \param id ID des Objekts
		 */
		ScriptObject(int id);
	
		/**
		 * \fn virtual ~ScriptObject()
		 * \brief Destruktor
		 */
		virtual ~ScriptObject();
		
				
		/**
		 * \fn bool takeDamage(Damage* damage)
		 * \brief Das Objekt nimmt Schaden in der in damage angegebenen Art und Hoehe.
		 * \param damage Schadensart und -hoehe
		 */
		virtual bool takeDamage(Damage* damage);
		
		/**
		 * \fn virtual int getValue(std::string valname)
		 * \brief Schiebt den gewuenschten Attributwert eines Objektes auf den Lua Stack
		 * \param valname Name des Wertes
		 */
		virtual int getValue(std::string valname);
	
		/**
		 * \fn virtual bool setValue(std::string valname)
		 * \brief Setzt den gewuenschten Attributwert eines Objektes
		 * \param valname Name des Wertes
		 */
		virtual bool setValue(std::string valname);
	
		/**
		 * \fn virtual bool isCreature()
		 * \brief Gibt aus, ob es sich um eine Kreatur handelt
		 */
		virtual bool isCreature()
		{
			return false;
		}
		
		/**
		 * \fn virtual bool update ( float time)
		 * \brief Aktualisiert das Object, nachdem eine bestimmte Zeit vergangen ist. Alle Aktionen des Objekts werden auf diesem Weg ausgeloest.
		 * \param time Menge der vergangenen Zeit in Millisekunden
		 * \return bool, der angibt, ob die Aktualisierung fehlerfrei verlaufen ist
		 */
		virtual  bool  update ( float time);
		
		/**
		 * \fn std::string getRenderInfo()
		 * \brief Gibt die Informationen zum Rendern der Objektes aus
		 */
		std::string getRenderInfo()
		{
			return m_render_info;
		}
		
		/**
		 * \fn void setRenderInfo(std::string render_info)
		 * \brief Setzt die INformationen zum Rendern des Objektes
		 */
		void setRenderInfo(std::string render_info)
		{
			m_render_info = render_info;
		}
		
		/**
		 * \fn std::string getActionString()
		 * \brief Gibt die aktuelle Aktion als String aus
		 */
		virtual std::string getActionString()
		{
			return m_animation;
		}
		
		/**
		 * \fn virtual float getActionPercent()
		 * \brief Gibt den Prozentsatz, zu dem die aktuelle Aktion fortgeschritten ist aus
		 */
		virtual float getActionPercent()
		{
			return m_animation_elapsed_time / m_animation_time;
		}
		
		/**
		 * \fn void setAnimation(std::string anim, float time, bool repeat = false)
		 * \brief Setzt die Animation eines Objekts
		 * \param anim Animation
		 * \param time Dauer der Animation in ms
		 * \param wenn auf true gesetzt, wird die Animation wiederholt, bis eine andere gesetzt wird
		 */
		void setAnimation(std::string anim, float time, bool repeat = false);
		
		/**
		 * \fn virtual void writeNetEvent(NetEvent* event, CharConv* cv)
		 * \brief Schreibt die Daten zu einem NetEvent in den Bitstream
		 * \param event NetEvent das beschrieben wird
		 * \param cv Bitstream
		 */
		virtual void writeNetEvent(NetEvent* event, CharConv* cv);
	
	
		/**
		 * \fn virtual void processNetEvent(NetEvent* event, CharConv* cv)
		 * \brief Fuehrt die Wirkung eines NetEvents auf das Objekt aus. Weitere Daten werden aus dem Bitstream gelesen
		 */
		virtual void processNetEvent(NetEvent* event, CharConv* cv);
		
	private:
		/**
		 * \var std::string m_render_info
		 * \brief Information zum Rendern des Objekts
		 */
		std::string m_render_info;
		
		/**
		 * \var std::string m_animation
		 * \brief aktuell gesetzte Animationen
		 */
		std::string m_animation;
		
		/**
		 * \var float m_animation_time
		 * \brief Gesamtdauer der aktuellen Animation
		 */
		float m_animation_time;
		
		/**
		 * \var float m_animation_elapsed_time
		 * \brief bisher verstrichene Zeit der Animation
		 */
		float m_animation_elapsed_time;
		
		/**
		 * \var bool m_animation_repeat
		 * \brief wenn auf true gesetzt, so wird die Animation wiederholt
		 */
		bool m_animation_repeat;
		
};
#endif

