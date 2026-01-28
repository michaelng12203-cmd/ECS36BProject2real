#include "XMLReader.h"
#include <expat.h>
#include <queue>

struct CXMLReader::SImplementation{
    std::shared_ptr<CDataSource> Source;
    std::queue<SXMLEntity> EntityQueue;
    XML_Parser Parser;

    static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts){
        SImplementation *Implementation = (SImplementation *)userData;
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;

        for(int i = 0; atts[i] != nullptr; i += 2){
            entity.DAttributes.push_back({atts[i], atts[i+1]});
        }
        Implementation->EntityQueue.push(entity);
    }

    static void EndElementHandler(void *userData, const XML_Char *name){
        SImplementation *Implementation = (SImplementation *)userData;
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;

        Implementation->EntityQueue.push(entity);
    }
    static void CharacterDataHandler(void *userData, const XML_Char *s, int len){
        SImplementation *Implementation = (SImplementation *)userData;
        SXMLEntity entity;
        
        entity.DType = SXMLEntity::EType::CharData;
        std::string char_data(s);
        entity.DNameData = char_data;
        entity.DNameData += "\0";

        Implementation->EntityQueue.push(entity);
    }

    SImplementation(std::shared_ptr< CDataSource > src){
        Source = src;
        Parser = XML_ParserCreate(NULL);

        XML_SetStartElementHandler(Parser, StartElementHandler);
        XML_SetEndElementHandler(Parser, EndElementHandler);
        XML_SetCharacterDataHandler(Parser, CharacterDataHandler);
    }

    ~SImplementation(){
        XML_ParserFree(Parser);
    }
};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique<SImplementation>(src);
    
}

CXMLReader::~CXMLReader(){

}

bool CXMLReader::End() const{
    return DImplementation->EntityQueue.empty();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){

}
