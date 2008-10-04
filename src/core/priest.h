#ifndef PRIEST_H
#define PRIEST_H

#include "player.h"

/**
 * \class Priest
 * \brief Spieler der Klasse Priester
 */
class Priest : public Player {
//Public stuff
	public:
	//Fields
	//Constructors
	/**
	 * \fn Priest(World* world, unsigned int id)
	 * \brief Konstruktor
	 * \param world Welt
	 * \param id Objekt-ID
	 *
	 * Legt ein neues Priest Objekt an.
	 */
		Priest(World* world, int id);


	/**
		 * \fn ~Priest()
		 * \brief Destruktor
		 *
		 * Gibt den Allokierten Speicher wieder frei
	 */
		~Priest();
	//Accessor Methods
	//Operations
	/**
		 * \fn virtual bool init ()
		 * \brief Initialisiert den Priester
		 * \return bool, der angibt ob die initialisierung erfolgreich war
	 */
		virtual bool init ();

		/**
		 * \fn virtual bool destroy ()
		 * \brief Zerstoert das Objekt. Die Funktion ist virtuell und wird durch die abgeleiteten Klassen &uuml;berschrieben.
		 * \return bool, der angibt ob die Zerstoerung erfolgreich war
		 */
		virtual bool destroy()
		{
			return Player::destroy();
		}

		/**
		 * \fn virtual void calcDamage(ActionType type,Damage& dmg)
		 * \brief Berechnet den Basisschaden einer Aktion
		 */
		virtual void calcBaseDamage(Action::ActionType act,Damage& dmg)
		{
			Player::calcBaseDamage(act,dmg);
		}

		/**
		 * \fn virtual void calcBaseAttrMod()
		 * \brief Die modifizierten Basisattribute werden neu berechnet. Verwendet die Basisattribute, verwendete Items, sowie temporaere Modifikationen.
		 */
		virtual void calcBaseAttrMod()
		{
			Player::calcBaseAttrMod();
		}

		/**
		 * \fn bool update (float time)
		 * \brief Updatefunktion des Priesters
		 * \param time Verstrichene Zeit
		 * \return bool, Erfolgreiches Update?
		 *
		 **/
		bool update(float time);

	protected:

	/**
	 * \fn virtual void gainLevel()
	 * \brief Wird aufgerufen, wenn das Lebewesen ein Levelup erhält. Passt Erfahrung und alle Basisattribute an.
	 */
		virtual void gainLevel()
		{
			Player::gainLevel();
		}
};
#endif //PRIEST_H
