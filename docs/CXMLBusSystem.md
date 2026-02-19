//many of the structs and functions in XMLBusSystem.cpp were explained in CBusSystem.md, so here I will document the functions I used.

struct CXMLBusSystem::SImplementation{

    //used to find a specified start tag, return true if success, return false if failure
    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
    }

    //used to find a specified end tag, return true if success, return false if failure
    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
    }

    
    //this function handles a singular stop
    void ParseStop(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &stop){
    }

    //this function handles all stops, it calls ParseStop for every stop it encounters
    void ParseStops(std::shared_ptr< CXMLReader > systemsource){
    }

    //this function handles a singular route by handling all routestops within the route
    void ParseRoute(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &name){
    }

    //this function handles all routes, it calls PraseRoute for every route it encounters
    void ParseRoutes(std::shared_ptr< CXMLReader > systemsource){
    }

    //this function handles a single path and all node IDs it encounters
    void ParsePath(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &points){
    }   
    
    //this function handles all paths, it calls ParsePath to handle every path it encounters 
    void ParsePaths(std::shared_ptr< CXMLReader > systemsource){
    }

    //this function begins the whole process, calling start tags and Parse functions
    void ParseBusSystem(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){
    }

    //calls BusSystem and gives it source to use
    SImplementation(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){        
    }
}
    