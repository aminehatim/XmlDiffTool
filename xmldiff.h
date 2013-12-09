#ifndef XMLDIFF_H
#define XMLDIFF_H

#include <QStringList>
#include <QDomDocument>
#include <QTextStream>
#include <QPair>


class XmlDiff
{
public:
    XmlDiff(QDomDocument domDoc1, QDomDocument domDoc2, QTextStream *outStream);
    bool compare();
    void printElementAttributes(QDomElement elementToPrint);
    void PrintElementChildren(QDomElement element, int *tabCount);
    QPair<QStringList, QStringList> getSortedAttibutesAndValuesList(QDomElement element);
    void printTreeDifference(QDomElement element1, QDomElement element2);
    void printAttributesDifference(QDomElement element1, QDomElement element2);
    bool compareNodes(QDomElement element1, QDomElement element2);
    bool compareNodeAttributes(QDomElement element1, QStringList node1AttributesList, QDomElement element2, QStringList node2AttributesList);
    QString elementToPath(QDomElement element);


private:

    // For the view
    QString getTabString(int tabCount);
    QDomDocument domDoc1;
    QDomDocument domDoc2;
    QTextStream *outStream;

};

#endif // XMLDIFF_H
