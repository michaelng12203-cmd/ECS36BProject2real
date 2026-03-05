#include "XMLBusSystem.h"
#include <vector>
#include <unordered_map>
#include <iostream>
using std::cout;
using std::endl;

struct CXMLBusSystem::SImplementation{
    const std::string DBusSystemTag = "bussystem";
    const std::string DStopsTag = "stops";
    const std::string DStopTag = "stop";
    const std::string DStopIDAttr = "id";
    const std::string DStopNodeAttr = "node";
    const std::string DStopDescAttr = "description";

    const std::string DRoutesTag = "routes";
    const std::string DRouteTag = "route";
    const std::string DRouteNameAttr = "name";
    const std::string DRouteStopTag = "routestop";
    const std::string DRouteStopAttr = "stop";
    const std::string DScheduleNameAttr = "schedule";

    const std::string DPathsTag = "paths";
    const std::string DPathTag = "path";
    const std::string DPathSrcAttr = "source";
    const std::string DPathDestAttr = "destination";
    const std::string DPathNodeTag = "node";
    const std::string DPathNodeIDAttr = "id";

    //all SStop objects have access to whats in here
    struct SStop : public CBusSystem::SStop{
        TStopID DID;
        CStreetMap::TNodeID DNodeID;
        std::string DDescription;

        SStop(TStopID id, CStreetMap::TNodeID nodeid, const std::string &description){
            DID = id;
            DNodeID = nodeid;
            DDescription = description;
        }
        ~SStop(){};

        TStopID ID() const noexcept override{
            return DID;
        }

        CStreetMap::TNodeID NodeID() const noexcept override{
            return DNodeID;
        }

        std::string Description() const noexcept override{
            return DDescription;
        }

        std::string Description(const std::string &description) noexcept override{
            DDescription = description;
            return DDescription;
        }
    };

    //all SRoute objects have access to this
    struct SRoute : public CBusSystem::SRoute{
        std::string DName;
        std::vector<TStopID> DGetStopID;

        SRoute(std::string Name, std::vector<TStopID> GetStopID){
            DName = Name;
            DGetStopID = GetStopID;
        }

        ~SRoute(){};

        std::string Name() const noexcept override{
            return DName;

        }
        std::size_t StopCount() const noexcept override{
            return DGetStopID.size();
        
        }

        std::size_t TripCount() const noexcept override{
            return 3;

        }
        TStopID GetStopID(std::size_t index) const noexcept override{
            if(index >= DGetStopID.size()){
                return 0;
            }
            return DGetStopID[index];

        }
        TStopTime GetStopTime(std::size_t stopindex, std::size_t tripindex) const noexcept override{
            return TStopTime(std::chrono::seconds(0));

        }


    };

    //all SPath objects have access to this info
    struct SPath : public CBusSystem::SPath{
        CStreetMap::TNodeID DStartNodeID;
        CStreetMap::TNodeID DEndNodeID;
        std::vector<CStreetMap::TNodeID> DGetNodeID;

        SPath(CStreetMap::TNodeID StartNodeID, CStreetMap::TNodeID EndNodeID, std::vector<CStreetMap::TNodeID> GetNodeID){
            DStartNodeID = StartNodeID;
            DEndNodeID = EndNodeID;
            DGetNodeID = GetNodeID;
        }

        ~SPath(){};

        CStreetMap::TNodeID StartNodeID() const noexcept override{
            return DStartNodeID;

        }
        CStreetMap::TNodeID EndNodeID() const noexcept override{
            return DEndNodeID;

        }
        std::size_t NodeCount() const noexcept override{
            return DGetNodeID.size();

        }
        CStreetMap::TNodeID GetNodeID(std::size_t index) const noexcept override{
            if(index >= DGetNodeID.size()){
                return 0;
            }
            return DGetNodeID[index];

        }
    };


    //find a certain start tag
    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::StartElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }
    //find a certain end tag
    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::EndElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    std::vector<std::shared_ptr<SStop> > DStopsByIndex;
    std::unordered_map<TStopID,std::shared_ptr<SStop> > DStopsByID;

    //parses a single stop element
    void ParseStop(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &stop){
        TStopID StopID = std::stoull(stop.AttributeValue(DStopIDAttr));
        CStreetMap::TNodeID NodeID = std::stoull(stop.AttributeValue(DStopNodeAttr));
        auto NewStop = std::make_shared<SStop>(StopID, NodeID, stop.AttributeValue(DStopDescAttr));
        DStopsByIndex.push_back(NewStop);
        DStopsByID[StopID] = NewStop;
        FindEndTag(systemsource,DStopTag);
    }
    //is in charge of parsing all stop elements
    void ParseStops(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;

        do{
            if(!systemsource->ReadEntity(TempEntity,true)){
                return;
            }
            //cout<<int(TempEntity.DType)<<" '"<<TempEntity.DNameData<<"'"<<endl;
            if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DStopTag)){
                ParseStop(systemsource,TempEntity);
            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement)||(TempEntity.DNameData != DStopsTag));
    }

    std::vector<std::shared_ptr<SRoute>> DRouteByIndex;
    std::unordered_map<std::string, std::shared_ptr<SRoute> > DRouteByName;

    //parses a single route element and its routestop elements
    void ParseRoute(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &name){
        SXMLEntity TempEntity;
        std::string routeName = std::string(name.AttributeValue(DRouteNameAttr));
        std::vector<TStopID> RouteStopIDs;
        std::string schedule = std::string(name.AttributeValue(DScheduleNameAttr));
        std::cout<<schedule<<std::endl;
        
        do{
            //cout<<"GRRRRRRR\n\n\n"<<endl;
            if(!systemsource->ReadEntity(TempEntity,true)){
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DRouteStopTag)){
                TStopID StopID = std::stoull(TempEntity.AttributeValue(DRouteStopAttr));
                RouteStopIDs.push_back(StopID);
            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement) || (TempEntity.DNameData != DRouteTag));

        auto newRoute = std::make_shared<SRoute>(routeName, RouteStopIDs);
        DRouteByIndex.push_back(newRoute);
        DRouteByName[routeName] = newRoute;
    }

    //in charge of parsing all route elements
    void ParseRoutes(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;

        do{
            if(!systemsource->ReadEntity(TempEntity, true)){

                return;
            }
            //cout<<
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DRouteTag)){
                ParseRoute(systemsource, TempEntity);

            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement)||(TempEntity.DNameData != DRoutesTag));
    }

    std::unordered_map<std::string, std::shared_ptr<SPath> > DPathByStopIDs;

    //parses a single path, its source and destination, and its node IDs
    void ParsePath(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &points){
        SXMLEntity TempEntity;
        CStreetMap::TNodeID srcPath = std::stoull(points.AttributeValue(DPathSrcAttr));
        CStreetMap::TNodeID destPath = std::stoull(points.AttributeValue(DPathDestAttr));
        std::vector<CStreetMap::TNodeID> PathStopIDs;

        do{
            if(!systemsource->ReadEntity(TempEntity,true)){
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DPathNodeTag)){
                CStreetMap::TNodeID nodeID = std::stoull(TempEntity.AttributeValue(DPathNodeIDAttr));
                PathStopIDs.push_back(nodeID);
            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement) || (TempEntity.DNameData != DPathTag));

        auto newPath = std::make_shared<SPath>(srcPath, destPath, PathStopIDs);
        std::string key = std::to_string(srcPath) + "," + std::to_string(destPath);
        DPathByStopIDs[key] = newPath;
    }   
    
    //in charge of parsing all paths
    void ParsePaths(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;

        do{
            if(!systemsource->ReadEntity(TempEntity,true)){
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DPathTag)){
                ParsePath(systemsource, TempEntity);
            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement)||(TempEntity.DNameData != DPathsTag));


    }
    //the main "control center", this function will call other functions to parse elements
    bool ParseBusSystem(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){
        SXMLEntity TempEntity;
        if(!FindStartTag(systemsource,DBusSystemTag)){
            return false;
        }
        if(!FindStartTag(systemsource,DStopsTag)){
            return false;
        }
        ParseStops(systemsource);

        if(!FindStartTag(systemsource,DRoutesTag)){
            return false;
        }
        ParseRoutes(systemsource);

        if(!FindStartTag(pathsource,DPathsTag)){
            return false;
        }
        ParsePaths(pathsource);

        return true;
    }
    //gives ParseBusSystem source to work with
    SImplementation(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){
        ParseBusSystem(systemsource, pathsource);
        
    }

    std::size_t StopCount() const noexcept{
        return DStopsByIndex.size();
    }

    std::size_t RouteCount() const noexcept{
        return DRouteByIndex.size();
    }
    
    std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept{
        if(index >= DStopsByIndex.size()){
            return nullptr;
        }
        return DStopsByIndex[index];
    }
    
    std::shared_ptr<SStop> StopByID(TStopID id) const noexcept{ 
        auto StopID = DStopsByID.find(id);
        if(StopID == DStopsByID.end()){
            return nullptr;
        } else{
            return StopID->second;
        }
    }
    
    std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept{
        if(index >= DRouteByIndex.size()){
            return nullptr;
        }
        return DRouteByIndex[index];
    }
    
    std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept{
        auto RouteName = DRouteByName.find(name);
        if(RouteName == DRouteByName.end()){
            return nullptr;
        } else{
            return RouteName->second;
        }
    }
    
    std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const noexcept{
        std::string key = std::to_string(start) + "," + std::to_string(end);
        auto PathStopIDs = DPathByStopIDs.find(key);
        if(PathStopIDs == DPathByStopIDs.end()){
            return nullptr;
        } else{
            return PathStopIDs->second;
        }
    }
    
};



//creates DImplementation, allows access to functions in the struct above
CXMLBusSystem::CXMLBusSystem(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){
    DImplementation = std::make_unique<SImplementation>(systemsource,pathsource);
}
    
CXMLBusSystem::~CXMLBusSystem(){

}
    
std::size_t CXMLBusSystem::StopCount() const noexcept{
    return DImplementation->StopCount();
}
    
std::size_t CXMLBusSystem::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CXMLBusSystem::StopByIndex(std::size_t index) const noexcept{
    return DImplementation->StopByIndex(index);
}

std::shared_ptr<CBusSystem::SStop> CXMLBusSystem::StopByID(TStopID id) const noexcept{
    return DImplementation->StopByID(id);
}

std::shared_ptr<CBusSystem::SRoute> CXMLBusSystem::RouteByIndex(std::size_t index) const noexcept{
    return DImplementation->RouteByIndex(index);
}

std::shared_ptr<CBusSystem::SRoute> CXMLBusSystem::RouteByName(const std::string &name) const noexcept{
    return DImplementation->RouteByName(name);
}

std::shared_ptr<CBusSystem::SPath> CXMLBusSystem::PathByStopIDs(TStopID start, TStopID end) const noexcept{
    return DImplementation->PathByStopIDs(start, end);
}
