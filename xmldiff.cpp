#include "xmldiff.h"
#include <QFile>

XmlDiff::XmlDiff(QDomDocument domDoc1, QDomDocument domDoc2, QTextStream *outStream)
{

    this->domDoc1 = domDoc1;
    this->domDoc2 = domDoc2;

    this->outStream = outStream;

}

bool XmlDiff::compare(){
    // Start comparing
    QDomElement regDescElement1 = domDoc1.documentElement();
    QDomElement regDescElement2 = domDoc2.documentElement();
    QDomElement child1 = regDescElement1.firstChildElement();
    QDomElement child2 = regDescElement2.firstChildElement();

    bool isDifferent = false;
    while (!child1.isNull() || !child2.isNull()){
        //debug
        QString str1 = child1.attribute("Label");
        QString str2 = child2.attribute("Label");

        bool retValue = compareNodes(child1, child2);
        if(!retValue){
            // error
            isDifferent = true;
            break;
        }


        QDomElement child1Copy = child1;
        QDomElement child2Copy = child2;
        child1 = child1.nextSiblingElement();
        child2 = child2.nextSiblingElement();


        if((child1.isNull() && !child2.isNull()) || (!child1.isNull() && child2.isNull()) ){
            // If the xml trees are not the same
            if(child1.isNull()){
                // Missing Node from the first file
                QString pathOfMissingNode = elementToPath(child2);
                *outStream << "Missing Node from file 1("+pathOfMissingNode+")" << endl;
                printTreeDifference(child1Copy, child2);

            }
            else if(child2.isNull()){
                // Missing Node fron the second file
                QString pathOfMissingNode = elementToPath(child1);
                *outStream << "Missing Node from file 2("+pathOfMissingNode+")" << endl;
                printTreeDifference(child1, child2Copy);

            }
        }
    }

    if(!isDifferent){
        return false;
    }

    return true;

}

void XmlDiff::printElementAttributes(QDomElement elementToPrint)
{

    QPair<QStringList, QStringList> attributeValueListPair = getSortedAttibutesAndValuesList(elementToPrint);
    QStringList attributeList = attributeValueListPair.first;
    QStringList valueList = attributeValueListPair.second;

    // This should use path no labels
    int labelIdx = attributeList.indexOf("Label");
    QString label = valueList.at(labelIdx);

    *outStream << label << endl;
    for(int i= 0; i < attributeList.size(); i++){
        *outStream << "----> " + attributeList.at(i) + " = " + valueList.at(i) << endl;
    }


}

void XmlDiff::PrintElementChildren(QDomElement element,int *tabCount)
{
    QDomElement childElement = element.firstChildElement();

    QString tabStr = getTabString(*tabCount);
    QString elementLabel = element.attribute("Label");
    *outStream << tabStr + elementLabel << endl;
    *tabCount += 2;
    while(!childElement.isNull()){
        PrintElementChildren(childElement, tabCount);
        *tabCount -= 2;
        childElement = childElement.nextSiblingElement();
    }

}

QPair<QStringList, QStringList> XmlDiff::getSortedAttibutesAndValuesList(QDomElement element)
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

void XmlDiff::printTreeDifference(QDomElement element1, QDomElement element2)
{
    QDomElement parent1 = element1.parentNode().toElement();
    QDomElement parent2 = element2.parentNode().toElement();
    *outStream << "------------------ File1 --------------------" << endl;
    int tabCount = 0;
    PrintElementChildren(parent1, &tabCount);
    *outStream << "------------------ File2 --------------------" << endl;
    tabCount = 0;
    PrintElementChildren(parent2, &tabCount);

}

void XmlDiff::printAttributesDifference(QDomElement element1, QDomElement element2)
{
    *outStream << "------------------ File1 --------------------" << endl;
    printElementAttributes(element1);
    *outStream << "------------------ File2 --------------------" << endl;
    printElementAttributes(element2);
}

bool XmlDiff::compareNodes(QDomElement element1, QDomElement element2)
{

    // get sorted attributes

    QPair element1attributesValuesPair = getSortedAttibutesAndValuesList(element1);
    QPair element2attributesValuesPair = getSortedAttibutesAndValuesList(element2);

    QStringList element1attributesList = element1attributesValuesPair.first;
    QStringList element2attributesList = element2attributesValuesPair.first;

    QStringList element1ValuesList = element1attributesValuesPair.second;
    QStringList element2ValuesList = element2attributesValuesPair.second;

    // verify missing attributes
    bool retValue = compareNodeAttributes(element1attributesList, element2attributesList);
    if(!retValue){
        return false;
    }

    // verify diffrence in values
    for(int i=0; i<maxCount; i++){

    }
}

bool XmlDiff::compareNodeAttributes(QDomElement element1, QStringList node1AttributesList, QDomElement element2, QStringList node2AttributesList)
{

    if(node1AttributesList == node2AttributesList){
        return true;
    }

    // Missing attributes
    printAttributesDifference(element1, element2);
    return false;

}

QString XmlDiff::getTabString(int tabCount)
{
    // Add tabulation for better view
    QString tabStr;
    for(int i=0; i<=tabCount; i++){
        tabStr = tabStr.append("-");
    }
    tabStr = tabStr.append(">");

    return tabStr;

}

QString XmlDiff::elementToPath(QDomElement element) {
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
