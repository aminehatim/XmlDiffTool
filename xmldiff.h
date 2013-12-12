#ifndef XMLDIFF_H
#define XMLDIFF_H

#include <QStringList>
#include <QDomDocument>
#include <QTextStream>
#include <QPair>


class XmlDiff
{
public:

    static int compare(QDomDocument doc1, QDomDocument doc2, QString *errorMsg);

    static int compare(QDomElement node1, QDomElement node2, QString *errorMsg);

    static int compareChilds(QDomElement node1, QDomElement node2, QString *errorMsg);

    static int compareAttributes(QDomElement node1, QDomElement node2, QString *errorMsg);

    static int compareValues(QDomElement node1, QDomElement node2, QString *errorMsg);

    enum returnCode {
        XMLDIFF_OK = 0,
        XMLDIFF_MISSING_CHILDS = 1,
        XMLDIFF_MISSING_ATTRIBUTES = 2,
        XMLDIFF_DIFFERENT_VALUES = 3,
        XMLDIFF_UNVALID_REG_DESC =4
    };

private:

    static QString elementToPath(QDomElement element);
    static QPair<QStringList, QStringList> getAttrbitesValuesPair(QDomElement element);


};

#endif // XMLDIFF_H
