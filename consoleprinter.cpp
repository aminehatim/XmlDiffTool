#include "consoleprinter.h"

ConsolePrinter::ConsolePrinter()
{
}


void ConsolePrinter::PrintElementChildren(QDomElement element,int *tabCount, QTextStream *outStream)
{
    QDomElement childElement = element.firstChildElement();

    QString tabStr = getTabString(*tabCount);
    if(*tabCount == 0){
        QString path = elementToPath(element);
        *outStream << tabStr + path << endl;
    }
    else{
        QString elementLabel = element.attribute("Label");
        *outStream << tabStr + elementLabel << endl;
    }

    *tabCount += 2;
    while(!childElement.isNull()){
        PrintElementChildren(childElement, tabCount, outStream);
        *tabCount -= 2;
        childElement = childElement.nextSiblingElement();
    }

}

QPair<QStringList, QStringList> ConsolePrinter::getSortedAttibutesAndValuesList(QDomElement element)
{
    QStringList attributeList, valueList;
    QDomNamedNodeMap attributesMap = element.attributes();
    QDomNode attribute;

    for(int i = 0; i < attributesMap.size(); i++){
        attribute = attributesMap.item(i);
        QDomAttr domAttr = attribute.toAttr();
        attributeList.append(domAttr.name());
        valueList.append(domAttr.value());
    }

    QStringList attributeListCopy, valueListCopy;
    attributeListCopy = attributeList;
    valueListCopy = valueList;

    // Sort the attribute List
    attributeList.sort();

    // Update valueList order due to sorting
    for(int i=0; i < attributeListCopy.size(); i++){
        int attrIdx = attributeListCopy.indexOf(attributeList.at(i));
        valueList.insert(i, valueListCopy.at(attrIdx));
    }

    return qMakePair(attributeList, valueList);
}

void ConsolePrinter::printTreeDifference(QDomElement element1, QDomElement element2, QTextStream *outStream)
{
    QDomElement parent1 = element1.parentNode().toElement();
    QDomElement parent2 = element2.parentNode().toElement();
    *outStream << "------------------ File1 --------------------" << endl;
    int tabCount = 0;
    PrintElementChildren(parent1, &tabCount, outStream);
    *outStream << "------------------ File2 --------------------" << endl;
    tabCount = 0;
    PrintElementChildren(parent2, &tabCount, outStream);

}

void ConsolePrinter::printAttributesDifference(QDomElement element1, QDomElement element2, QTextStream *outStream)
{
    *outStream << "------------------ File1 --------------------" << endl;
    printElementAttributes(element1, outStream);
    *outStream << "------------------ File2 --------------------" << endl;
    printElementAttributes(element2, outStream);
}

void ConsolePrinter::printElementAttributes(QDomElement elementToPrint, QTextStream *outStream)
{

    QPair<QStringList, QStringList> attributeValueListPair = getSortedAttibutesAndValuesList(elementToPrint);
    QStringList attributeList = attributeValueListPair.first;
    QStringList valueList = attributeValueListPair.second;

    // This should use path no labels
    QString path = elementToPath(elementToPrint);
    *outStream << path << endl;
    for(int i= 0; i < attributeList.size(); i++){
        *outStream << "----> " + attributeList.at(i) + " = " + valueList.at(i) << endl;
    }


}

QString ConsolePrinter::getTabString(int tabCount)
{
    // Add tabulation for better view
    QString tabStr;
    for(int i=0; i<=tabCount; i++){
        tabStr = tabStr.append("-");
    }
    tabStr = tabStr.append(">");

    return tabStr;

}

QString ConsolePrinter::elementToPath(QDomElement element){
    QStringList pathList;
    QDomElement parentElement = element;

    while( (!parentElement.isNull() ) &&
           (!parentElement.attribute("Label").isEmpty() ) &&
           (parentElement.tagName() != "register_description" )
           ) {
        pathList.push_front(parentElement.attribute("Label"));
        QDomNode parentNode = parentElement.parentNode();
        parentElement = parentNode.toElement();
    }
    QString tmp;
    tmp=parentElement.tagName();
    if (parentElement.tagName() !=  "register_description") {
        return QString();
    }

    QString path = pathList.join("/");
    if (pathList.empty()) {
        path.prepend("root");
    }
    return path;
}
