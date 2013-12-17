#include "xmldiff.h"


int XmlDiff::compare(QDomDocument doc1, QDomDocument doc2, QDomElement *errorNode1, QDomElement *errorNode2, QString *errorMsg)
{
    QDomElement registerDescElement1 = doc1.documentElement().firstChildElement();
    if(registerDescElement1.isNull()){
        return XMLDIFF_UNVALID_REG_DESC;
    }
    QDomElement registerDescElement2 = doc2.documentElement().firstChildElement();
    if(registerDescElement2.isNull()){
        return XMLDIFF_UNVALID_REG_DESC;
    }

    return compare(registerDescElement1, registerDescElement2, errorNode1, errorNode2, errorMsg);
}

int XmlDiff::compare(QDomElement node1, QDomElement node2, QDomElement *errorNode1, QDomElement *errorNode2, QString *errorMsg)
{

    int retValue;

    // Save the current nodes
    *errorNode1 = node1;
    *errorNode2 = node2;

    // Comapre childs
    retValue = compareChilds(node1, node2, errorMsg);
    if(retValue != XMLDIFF_OK){
        return retValue;
    }

    // Compare attributes
    retValue = compareAttributes(node1, node2, errorMsg);
    if(retValue != XMLDIFF_OK){
        return retValue;
    }

    // Compare values
    retValue = compareValues(node1, node2, errorMsg);
    if(retValue != XMLDIFF_OK){
        return retValue;
    }

    QDomElement child1 = node1.firstChildElement();
    QDomElement child2 = node2.firstChildElement();

    while(!child1.isNull() || !child2.isNull()){
        // Compare childs
        int retVal = compare(child1, child2, errorNode1, errorNode2, errorMsg);
        if(retVal != XMLDIFF_OK){
            return retVal;
        }

        child1 = child1.nextSiblingElement();
        child2 = child2.nextSiblingElement();
    }

}

int XmlDiff::compareChilds(QDomElement node1, QDomElement node2, QString *errorMsg){

    QDomNodeList node1ChildNodes = node1.childNodes();
    QDomNodeList node2ChildNodes = node2.childNodes();

    int node1ChildCount = node1ChildNodes.count();
    int node2ChildCount = node2ChildNodes.count();

    if(node1ChildCount != node2ChildCount){
        return XMLDIFF_MISSING_CHILDS;
    }

    for (int i=0; i<node1ChildNodes.count(); i++){

        QString child1Label = node1ChildNodes.at(i).toElement().attribute("Label");
        QString child2Label = node2ChildNodes.at(i).toElement().attribute("Label");

        if(child1Label != child2Label){
            return XMLDIFF_MISSING_CHILDS;
        }

    }

    return XMLDIFF_OK;
}

int XmlDiff::compareAttributes(QDomElement node1, QDomElement node2, QString *errorMsg)
{

    QStringList node1AttributesList = getAttrbitesValuesPair(node1).first;
    QStringList node2AttributesList = getAttrbitesValuesPair(node2).first;

    if(node1AttributesList != node2AttributesList){
            return XMLDIFF_MISSING_ATTRIBUTES;
    }

    return XMLDIFF_OK;

}

int XmlDiff::compareValues(QDomElement node1, QDomElement node2, QString *errorMsg)
{
    QStringList node1ValuesList = getAttrbitesValuesPair(node1).second;
    QStringList node2ValuesList = getAttrbitesValuesPair(node2).second;

    if(node1ValuesList != node2ValuesList){
        return XMLDIFF_DIFFERENT_VALUES;
    }

    return XMLDIFF_OK;
}


QString XmlDiff::elementToPath(QDomElement element){
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

QPair<QStringList, QStringList> XmlDiff::getAttrbitesValuesPair(QDomElement element)
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
