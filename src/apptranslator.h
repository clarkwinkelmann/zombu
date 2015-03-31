/**
  \file
  \brief    Déclaration de la classe AppTranslator.
  \author   Clark Winkelmann
  \date     mars 2015
*/
#ifndef APPTRANSLATOR_H
#define APPTRANSLATOR_H

#include <QObject>
#include <QApplication>
#include <QTranslator>

/**
 * Gère les changements de langue de l'app
 */
class AppTranslator : public QObject
{
    Q_OBJECT

public:
    /**
     * Langues disponibles
     */
    enum Locale_e{
        FR,
        EN
    };

    AppTranslator(QApplication *m_pApp);

    void setPreferredLocale();

public slots:
    void changeLocale(AppTranslator::Locale_e locale);

private:
    QApplication *m_pApp;
    QTranslator m_TranslatorEn;
    QTranslator *m_pCurrentTranslator;

};

#endif // APPTRANSLATOR_H
