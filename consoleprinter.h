#ifndef CONSOLEPRINTER_H
#define CONSOLEPRINTER_H

#include <QPair>
#include <QDomDocument>
#include <QTextStream>
#include <QStringList>

class ConsolePrinter
{
public:
    ConsolePrinter();

    static void PrintElementChildren(QDomElement element, int *tabCount, QTextStream *outStream);
    static QPair<QStringList, QStringList> getSortedAttibutesAndValuesList(QDomElement element);
    static void printTreeDifference(QDomElement element1, QDomElement element2, QTextStream *outStream);
    static void printAttributesDifference(QDomElement element1, QDomElement element2, QTextStream *outStream);
    static QString getTabString(int tabCount);
    static void printElementAttributes(QDomElement elementToPrint, QTextStream *outStream);
    static QString elementToPath(QDomElement element);
};

#endif // CONSOLEPRINTER_H
