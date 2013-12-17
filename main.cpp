#include <QCoreApplication>
#include <QTextStream>
#include <QDomDocument>
#include <QStringList>
#include <QFile>
#include <QDir>

#include <QDebug>

#include "xmldiff.h"


void printUsage (QTextStream *outStream){
    QString usage = "Usage : XmlDiffTool file1.xml file2.xml";
    *outStream << usage << endl;
}

QStringList parseArguments (int argNumber, char *argv[], bool *ok, QString *errorMessage){

    QStringList argumentList;
    *ok = true;

    // Verify argument number
    if(argNumber != 3){
        // Arguent error
        *errorMessage = "Wrong argument!";
        *ok = false;
        return QStringList();
    }

    QString file1Path = argv[1];
    QString file2Path = argv[2];


    // Verify existance
    QFile file1(file1Path);
    if(!file1.exists()){
        *errorMessage = file1Path + " Does not exist!";
        *ok = false;
        return QStringList();
    }
    argumentList.append(file1Path);

    QFile file2(file2Path);
    if(!file2.exists()){
        *errorMessage = file2Path + " Does not exist!";
        *ok = false;
        return QStringList();
    }
    argumentList.append(file2Path);
    return argumentList;

}

QDomDocument getDomDucument (QString filePath, bool *ok, QString *errorMsg){

    *ok = true;
    QFile file( filePath);
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        *errorMsg = "Failed to open file for reading.";
        *ok = false;
        return QDomDocument();
    }


    QDomDocument domDoc;
    if( !domDoc.setContent( &file ) )
    {
        *errorMsg = "Failed to parse the file into a DOM tree.";
        file.close();
        *ok = false;
        return QDomDocument();
    }

    file.close();
    return domDoc;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream outStream(stdout);

    bool ok;
    QString errorMessage;
    QStringList argumentList = parseArguments(argc, argv, &ok, &errorMessage);

    if(!ok){
        // Parse error
        outStream << errorMessage << endl;
        printUsage(&outStream);
        return 1;
    }


    QDomDocument domDoc1 = getDomDucument(argumentList.at(0), &ok, &errorMessage);
    if(!ok){
        // Xml file error
        outStream << errorMessage << endl;
        return 1;
    }

    QDomDocument domDoc2 = getDomDucument(argumentList.at(1), &ok, &errorMessage);
    if(!ok){
        // Xml file error
        outStream << errorMessage << endl;
        return 1;
    }

    int retValue = XmlDiff::compare(domDoc1, domDoc2, &errorMessage);
    switch (retValue) {
    case 0:
        outStream << "The Files are the same" << endl;
        break;
    case 1:
        outStream << "Missing Nodes" << endl;
        break;
    case 2:
        outStream << "Missing Attributes" << endl;

        break;
    case 3:
        outStream << "Values difference" << endl;
        break;
    case 4:
        outStream << "Invalid xml file" << endl;
        break;
    default:
        break;
    }

    return retValue;
}
