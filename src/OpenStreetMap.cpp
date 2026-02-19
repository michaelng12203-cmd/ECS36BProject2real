#include "OpenStreetMap.h"
#include <unordered_map>
#include <iostream>
using std::cout;
using std::endl;

struct COpenStreetMap::SImplementation{
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";

    const std::string DWayTag = "way";
    const std::string DWayIDAttr = "id";
    const std::string DWayNode = "nd";
    const std::string DWayNodeAttr = "ref";


    struct SNode: public CStreetMap::SNode{
        const std::string DNodeIDAttr = "id";
        const std::string DNodeLatAttr = "lat";
        const std::string DNodeLonAttr = "lon";
        TNodeID DID;
        SLocation DLocation;
        //SXMLEntity DEntity;

        SNode(const SXMLEntity &entity){
            // cout<<std::stoull(entity.AttributeValue(DNodeIDAttr))<<endl;
            // cout<<std::stod(entity.AttributeValue(DNodeLatAttr))<<endl;
            // cout<<std::stod(entity.AttributeValue(DNodeLonAttr))<<endl;

            auto NodeID = std::stoull(entity.AttributeValue(DNodeIDAttr));
            auto NodeLat = std::stod(entity.AttributeValue(DNodeLatAttr));
            auto NodeLon = std::stod(entity.AttributeValue(DNodeLonAttr));

            cout<<NodeID<<endl;
            cout<<NodeLat<<endl;
            cout<<NodeLon<<endl;

            DID = NodeID;
            DLocation = SLocation{NodeLat,NodeLon};
            //DEntity = entity;
            
        }
        ~SNode(){

        }

        TNodeID ID() const noexcept override{
            return DID;
        }
        
        SLocation Location() const noexcept override{
            return DLocation;
        }
        
        std::size_t AttributeCount() const noexcept override{
            SXMLEntity TempEntity;
            //int count = 0;
            /*for(auto &Attribute : TempEntity.DAttributes){
                count++;
            }*/
           /*for (int i = 0; i < TempEntity.DAttributes.size(); i++){
                count++;
           }*/
            return TempEntity.DAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            SXMLEntity TempEntity;
            if (index < AttributeCount()){
                return std::get<0>(TempEntity.DAttributes[index]);
            }
            return std::string();
        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            SXMLEntity TempEntity;
            return TempEntity.AttributeExists(key);
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            SXMLEntity TempEntity;
            if (HasAttribute(key)){
                return TempEntity.AttributeValue(key);
            }
            return std::string();
        }
        
    };

    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;

    struct SWay: public CStreetMap::SWay{
        const std::string DWayIDAttr = "id";

        std::vector<TNodeID> DGetNodeID;

        TWayID DID;
        //SXMLEntity DEntity;

        SWay(const SXMLEntity &entity, std::vector<TNodeID> WayNodeIDs){
            //cout<<std::stoull(entity.AttributeValue(DWayIDAttr))<<endl;
            auto WayID = std::stoull(entity.AttributeValue(DWayIDAttr));

            DID = WayID;

            cout<<DID<<endl;

            DGetNodeID = WayNodeIDs;
            //DEntity = entity;
        }

        ~SWay(){

        }

        TWayID ID() const noexcept override{
            return DID;
        }
        
        std::size_t NodeCount() const noexcept override{
            return DGetNodeID.size();
        }
        
        TNodeID GetNodeID(std::size_t index) const noexcept override{
            /*if (index < NodeCount()){
                return DNodesByIndex[index];
            }
            return InvalidNodeID;*/
            return DGetNodeID[index];
        }
        
        std::size_t AttributeCount() const noexcept override{
            SXMLEntity TempEntity;
            //int count = 0;
            /*for(auto &Attribute : TempEntity.DAttributes){
                count++;
            }*/
            /*for (int i = 0; i < TempEntity.DAttributes.size(); i++){
                    count++;
            }*/
            return TempEntity.DAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            SXMLEntity TempEntity;
            if (index < AttributeCount()){
                return std::get<0>(TempEntity.DAttributes[index]);
            }
            return std::string();
        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            SXMLEntity TempEntity;
            return TempEntity.AttributeExists(key);
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            SXMLEntity TempEntity;
            if (HasAttribute(key)){
                return TempEntity.AttributeValue(key);
            }
            return std::string();
        }
        
    };

    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::StartElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::EndElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    void ParseNode(std::shared_ptr< CXMLReader > src, const SXMLEntity &entity){
        auto NewNode = std::make_shared<SNode>(entity);
        DNodesByIndex.push_back(NewNode);
        DNodesByID[NewNode->ID()] = NewNode;
        FindEndTag(src,DNodeTag);
    }

    void ParseNodes(std::shared_ptr< CXMLReader > src){
        SXMLEntity TempEntity;

        do{
            if(!src->ReadEntity(TempEntity,true)){
                return;
            }
            //cout<<int(TempEntity.DType)<<" '"<<TempEntity.DNameData<<"'"<<endl;
            if((TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag)){
                ParseNode(src,TempEntity);
            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement)||(TempEntity.DNameData != DOSMTag));
    }

    std::vector<std::shared_ptr<SWay> > DWaysByIndex;
    std::unordered_map<TWayID,std::shared_ptr<SWay> > DWaysByID;

    void ParseWay(std::shared_ptr< CXMLReader > src, const SXMLEntity &entity){
        SXMLEntity TempEntity;
        //std::string WayID = std::string(entity.AttributeValue(DWayIDAttr));

        std::vector<TNodeID> WayNodeIDs;

        do{
            //cout<<"GRRRRRRR\n\n\n"<<endl;
            if(!src->ReadEntity(TempEntity,true)){
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DWayNodeAttr)){
                TNodeID NodeID = std::stoull(TempEntity.AttributeValue(DWayNodeAttr));
                WayNodeIDs.push_back(NodeID);
            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement) || (TempEntity.DNameData != DWayTag));

        auto NewNode = std::make_shared<SNode>(TempEntity);
        DNodesByIndex.push_back(NewNode);
        DNodesByID[NewNode->ID()] = NewNode;
        FindEndTag(src,DNodeTag);
    }

    void ParseWays(std::shared_ptr< CXMLReader > src){
        SXMLEntity TempEntity;

        do{
            if(!src->ReadEntity(TempEntity, true)){
                return;
            }

            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DWayTag)){
                ParseWay(src, TempEntity);
            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement)||(TempEntity.DNameData != DWayTag));
    }

    bool ParseOSM(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;

        if(!FindStartTag(src,DOSMTag)){
            cout<<"Start tag osm not found"<<endl;
            return false;
        }

        //cout<<"awesome"<<endl;
        ParseNodes(src);
        //cout<<"sauce"<<endl;

        /*while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                auto NewNode = std::make_shared<SNode>(TempEntity);
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NewNode->ID()] = NewNode;
                FindEndTag(src,DNodeTag);
            }
        }*/

        /*if(!FindStartTag(src,DWayTag)){
            cout<<"Start tag way not found"<<endl;
            return false;
        }*/

        //cout<<"radical"<<endl;
        ParseWays(src);
        //cout<<"bodcaious"<<endl;

        /*std::vector<TNodeID> WayNodeIDs;

        if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DWayNode)){
            TNodeID NodeID = std::stoull(TempEntity.AttributeValue(DWayNodeAttr));
            WayNodeIDs.push_back(NodeID);
        }

        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DWayTag){
                auto NewWay = std::make_shared<SWay>(TempEntity, WayNodeIDs);
                DWaysByIndex.push_back(NewWay);
                DWaysByID[NewWay->ID()] = NewWay;
                FindEndTag(src,DWayTag);
            }
        }*/
    }

    SImplementation(std::shared_ptr<CXMLReader> src){
        ParseOSM(src);
    }

    std::size_t NodeCount() const noexcept{
        return DNodesByIndex.size();
    }

    std::size_t WayCount() const noexcept{
        return DWaysByIndex.size();
    }

    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        if(index < DNodesByIndex.size()){
            return DNodesByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        auto Search = DNodesByID.find(id);
        if(Search != DNodesByID.end()){
            return Search->second;
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        if(index < DWaysByIndex.size()){
            return DWaysByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        auto Search = DWaysByID.find(id);
        if(Search != DWaysByID.end()){
            return Search->second;
        }
        return nullptr;
    }

};


COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){

}

std::size_t COpenStreetMap::NodeCount() const noexcept{
    return DImplementation->NodeCount();
}

std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}