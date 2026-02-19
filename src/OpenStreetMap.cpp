#include "OpenStreetMap.h"
#include <unordered_map>
#include <iostream>
using std::cout;
using std::endl;

using TAttribute = std::pair< std::string, std::string >;
using TAttributes = std::vector< TAttribute >;

struct COpenStreetMap::SImplementation{
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";

    const std::string DWayTag = "way";
    const std::string DWayTagTag = "tag";
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

            cout<<"Node ID: "<<NodeID<<endl;
            cout<<"Node Latitude: "<<NodeLat<<endl;
            cout<<"Node Longitude: "<<NodeLon<<endl;

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

        TAttributes DGetAttributes;

        TWayID DID;
        //SXMLEntity DEntity;

        SWay(std::vector<TNodeID> WayNodeIDs, TWayID WayID, TAttributes Attributes){

            DID = WayID;

            cout<<"SWay ID: "<<DID<<endl;

            DGetNodeID = WayNodeIDs;
            DGetAttributes = Attributes;
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
            return DGetNodeID[index];
        }
        
        std::size_t AttributeCount() const noexcept override{
            return DGetAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if (index < AttributeCount()){
                return std::get<0>(DGetAttributes[index]);
            }
            return std::string();
        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            for(auto &Attribute : DGetAttributes){
                if(std::get<0>(Attribute) == key){
                    return true;
                }
            }
            return false;
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            for(auto &Attribute : DGetAttributes){
                if(std::get<0>(Attribute) == key){
                    return std::get<1>(Attribute);
                }
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

    std::vector<std::shared_ptr<SWay> > DWaysByIndex;
    std::unordered_map<TWayID,std::shared_ptr<SWay> > DWaysByID;

    bool ParseOSM(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;

        if(!FindStartTag(src,DOSMTag)){
            cout<<"Start tag osm not found"<<endl;
            return false;
        }

        std::vector<TNodeID> WayNodeIDs;
        TAttributes TagAttributes;

        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                auto NewNode = std::make_shared<SNode>(TempEntity);
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NewNode->ID()] = NewNode;
                FindEndTag(src,DNodeTag);
            }

            /*if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DWayNode)){
                cout<<"oioioi"<<endl;
                TNodeID NodeID = std::stoull(TempEntity.AttributeValue(DWayNodeAttr));
                WayNodeIDs.push_back(NodeID);
            }*/

            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DWayTag){
                cout<<std::stoull(TempEntity.AttributeValue(DWayIDAttr))<<endl;
                auto WayID = std::stoull(TempEntity.AttributeValue(DWayIDAttr));

                do{
                    if(!src->ReadEntity(TempEntity,true)){
                        return true;
                    }
                    //cout<<int(TempEntity.DType)<<" '"<<TempEntity.DNameData<<"'"<<endl;
                    if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DWayNode)){
                        cout<<std::stoull(TempEntity.AttributeValue(DWayNodeAttr))<<endl;
                        TNodeID NodeID = std::stoull(TempEntity.AttributeValue(DWayNodeAttr));
                        WayNodeIDs.push_back(NodeID);
                    }

                    if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DWayTagTag)){
                        TAttribute attribute;

                        std::string key = "k";
                        std::string value = TempEntity.AttributeValue("k");

                        attribute = {key, value};

                        TAttribute attribute2;

                        std::string key2 = "v";
                        std::string value2 = TempEntity.AttributeValue("v");

                        attribute2 = {key2, value2};

                        TagAttributes.push_back(attribute);
                        TagAttributes.push_back(attribute2);
                    }

                }while((TempEntity.DType != SXMLEntity::EType::EndElement)||(TempEntity.DNameData != DWayTag));

                auto NewWay = std::make_shared<SWay>(WayNodeIDs, WayID, TagAttributes);
                DWaysByIndex.push_back(NewWay);
                DWaysByID[NewWay->ID()] = NewWay;
                FindEndTag(src,DWayTag);
            }
        }

        /*if(!FindStartTag(src,DWayTag)){
            cout<<"Start tag way not found"<<endl;
            return false;
        }*/

        

        /*while(src->ReadEntity(TempEntity)){

            
        }*/
       return true;
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