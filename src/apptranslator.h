/**
  \file
  \brief    Déclaration de la classe AppTranslator.
  \author   Clark Winkelmann
  \date     mars 2015
*/
#ifndef APPTRANSLATOR_H
#define APPTRANSLATOR_H

#include <QTranslator>
#include <QApplication>

class AppTranslator
{
public:
    enum Locale_e{
        FR,
        EN
    };

    AppTranslator(QApplication *m_pApp);
    void setPreferredLocale();

signals:
    void changeLocale(Locale_e locale);

private:
    QApplication *m_pApp;
    QTranslator m_TranslatorEn;
    QTranslator *m_pCurrentTranslator;

};

#endif // APPTRANSLATOR_H
