#ifndef MAGE_H
#define MAGE_H

#include "player.h"

/**
 * \class Mage
 * \brief Spieler der Klasse Magier
 */
class Mage : public Player {
//Public stuff
	public:
	//Fields
	//Constructors
		/**
		* \fn Mage( int id)
		* \brief Konstruktor
		* \param id Objekt-ID
		*
		* Legt ein neues Mage Objekt an.
		*/
		Mage( int id);


		/**
		 * \fn ~Mage()
		 * \brief Destruktor
		 *
		 * Gibt den Allokierten Speicher wieder frei
		 */
		~Mage();
	//Accessor Methods
	//Operations
		/**
		 * \fn virtual bool init ()
		 * \brief Initialisiert den Magier
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
		 * \fn virtual void calcBaseDamage(Action::ActionType act,Damage& dmg)
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
		 * \brief Updatefunktion des Magiers
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
#endif //MAGE_H
