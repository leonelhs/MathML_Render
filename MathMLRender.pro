
#Refurbished MathML Render QtMmlWidget 4 over Qt 6.2.0

QT += core gui xml widgets

SOURCES += \
        Helpers.cpp \
        MmlDocument.cpp \
        MmlEntityTable.cpp \
        MmlNodeSpec.cpp \
        OperSpecSearchResult.cpp \
        main.cpp \
        Nodes/MmlNode.cpp \
        Nodes/MmlTokenNode.cpp \
        Nodes/MmlRootBaseNode.cpp \
        Nodes/MmlSpacingNode.cpp \
        Nodes/MmlSubsupBaseNode.cpp \
        Nodes/MmlTableBaseNode.cpp \
        Nodes/XmlNodes/MmlTextNode.cpp \
        Nodes/XmlNodes/MmlMtdNode.cpp \
        Nodes/XmlNodes/MmlMsubNode.cpp \
        Nodes/XmlNodes/MmlMsupNode.cpp \
        Nodes/XmlNodes/MmlMfracNode.cpp \
        Nodes/XmlNodes/MmlMoverNode.cpp \
        Nodes/XmlNodes/MmlMtableNode.cpp \
        Nodes/XmlNodes/MmlMunderNode.cpp \
        Nodes/XmlNodes/MmlMpaddedNode.cpp \
        Nodes/XmlNodes/MmlMsubsupNode.cpp \
        Nodes/XmlNodes/MmlMunderoverNode.cpp \
        Nodes/XmlNodes/MmlMtrNode.cpp \
        Nodes/XmlNodes/MmlMoNode.cpp

HEADERS += \
        Device.h \
        Helpers.h \
        MmlDocument.h \
        MmlEntityTable.h \
        MmlFont.h \
        MmlNodeSpec.h \
        MmlOperSpec.h \
        OperSpecSearchResult.h \
        Nodes/Mml.h \
        Nodes/MmlNode.h \
        Nodes/MmlRootBaseNode.h \
        Nodes/MmlSpacingNode.h \
        Nodes/MmlSubsupBaseNode.h \
        Nodes/MmlTableBaseNode.h \
        Nodes/MmlTokenNode.h \
        Nodes/XmlNodes/MmlMfracNode.h \
        Nodes/XmlNodes/MmlMiNode.h \
        Nodes/XmlNodes/MmlMnNode.h \
        Nodes/XmlNodes/MmlMrowNode.h \
        Nodes/XmlNodes/MmlMsqrtNode.h \
        Nodes/XmlNodes/MmlMrootNode.h \
        Nodes/XmlNodes/MmlMsupNode.h \
        Nodes/XmlNodes/MmlMsubNode.h \
        Nodes/XmlNodes/MmlMsubsupNode.h \
        Nodes/XmlNodes/MmlMoNode.h \
        Nodes/XmlNodes/MmlMstyleNode.h \
        Nodes/XmlNodes/MmlTextNode.h \
        Nodes/XmlNodes/MmlMphantomNode.h \
        Nodes/XmlNodes/MmlMfencedNode.h \
        Nodes/XmlNodes/MmlMtableNode.h \
        Nodes/XmlNodes/MmlMtrNode.h \
        Nodes/XmlNodes/MmlMtdNode.h \
        Nodes/XmlNodes/MmlMoverNode.h \
        Nodes/XmlNodes/MmlMunderNode.h \
        Nodes/XmlNodes/MmlMunderoverNode.h \
        Nodes/XmlNodes/MmlMalignMarkNode.h \
        Nodes/XmlNodes/MmlMerrorNode.h \
        Nodes/XmlNodes/MmlMtextNode.h \
        Nodes/XmlNodes/MmlMpaddedNode.h \
        Nodes/XmlNodes/MmlMspaceNode.h \
        Nodes/XmlNodes/MmlUnknownNode.h




