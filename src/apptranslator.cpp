/**
  \file
  \brief    Définition de la classe AppTranslator.
  \author   Clark Winkelmann
  \date     mars 2015
*/
#include "apptranslator.h"

#include "resources.h"

AppTranslator::AppTranslator(QApplication *app)
{
    m_pApp = app;

    m_TranslatorEn.load("en", GameFramework::localesPath());
}

/**
 * @brief Charge la locale appropriée à l'environnement actuel
 */
void AppTranslator::setPreferredLocale() {
    // Locale par défaut si aucune n'est trouvée
    Locale_e newLocale = EN;

    // Langues de l'utilisateur dans l'ordre de préférence
    QStringList languages = QLocale::system().uiLanguages();

    for(int i = 0; i < languages.size(); ++i) {
        if(languages[i].startsWith("fr", Qt::CaseInsensitive)) {
            newLocale = FR;
            break; // Stop looping
        }

        if(languages[i].startsWith("en", Qt::CaseInsensitive)) {
            newLocale = EN;
            break; // Stop looping
        }
    }

    changeLocale(newLocale);
}

/**
 * @brief Change la langue de l'application
 * @param locale Locale à utiliser
 */
void AppTranslator::changeLocale(Locale_e locale)
{
    QTranslator *pNewTranslator = NULL;

    switch(locale) {
    case FR:
        // Rien à faire, langue par défaut
        break;
    case EN:
        pNewTranslator = &m_TranslatorEn;
        break;
    }

    // Si la langue a changé
    if(pNewTranslator != m_pCurrentTranslator) {
        // Retire l'ancien si présent
        if(m_pCurrentTranslator) {
            m_pApp->removeTranslator(m_pCurrentTranslator);
        }

        // Ajoute le nouveau si précisé
        if(pNewTranslator) {
            m_pApp->installTranslator(pNewTranslator);
        }

        // Sauvegarde le nouvel état
        m_pCurrentTranslator = pNewTranslator;
    }
}
