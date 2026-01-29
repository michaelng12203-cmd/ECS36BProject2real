#include "XMLReader.h"
#include <expat.h>
#include <queue>
#include <iostream>

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
                //printf("\n\n\n\nGRRRR\n\n\n");

    }

    static void EndElementHandler(void *userData, const XML_Char *name){
        SImplementation *Implementation = (SImplementation *)userData;
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;

        Implementation->EntityQueue.push(entity);
        //printf("\n\n\n\nGRRRR\n\n\n");
    }
    static void CharacterDataHandler(void *userData, const XML_Char *s, int len){
        SImplementation *Implementation = (SImplementation *)userData;
        SXMLEntity entity;

        entity.DType = SXMLEntity::EType::CharData;
        std::string char_data(s, len);
        entity.DNameData = char_data;

        Implementation->EntityQueue.push(entity);
                //printf("\n\n\n\nGRRRR\n\n\n");

    }

    SImplementation(std::shared_ptr< CDataSource > src){
        Source = src;
        Parser = XML_ParserCreate(NULL);

        XML_SetUserData(Parser, this);
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
    return DImplementation->EntityQueue.empty() && DImplementation->Source->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
   if(CXMLReader::End()){
        return false;
    }
    if(!DImplementation->EntityQueue.empty()){
        entity = DImplementation->EntityQueue.front();
        while(entity.DType == SXMLEntity::EType::CharData && skipcdata == true){
            DImplementation->EntityQueue.pop();
            entity = DImplementation->EntityQueue.front();
        }
        DImplementation->EntityQueue.pop();
        return true;
    }
    while(!DImplementation->Source->End()){
        std::size_t count = 1024;
        std::vector<char> buf;
        if(!DImplementation->Source->Read(buf,count)){
            return false;
        }
        XML_Parse(DImplementation->Parser, buf.data(), buf.size(), DImplementation->Source->End());
    }
        if(!DImplementation->EntityQueue.empty()){
        entity = DImplementation->EntityQueue.front();
        while(entity.DType == SXMLEntity::EType::CharData && skipcdata == true){
            DImplementation->EntityQueue.pop();
            entity = DImplementation->EntityQueue.front();
            if(CXMLReader::End()){
                return false;
            }
        }
        DImplementation->EntityQueue.pop();
        return true;
    }
    return false;
}