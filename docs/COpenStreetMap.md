/*many of the structs and functions in XMLBusSystem.cpp were explained in CBusSystem.md, so here I will document the functions I used.*/



struct COpenStreetMap::SImplementation{
    
    //used to find a specified start tag, return true if success, return false if failure
    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
    }

    //used to find a specified end tag, return true if success, return false if failure
    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
    }

    //reads the source and the entity that utilizes the source to detect nodes and ways in the function to convert as objects
    bool ParseOSM(std::shared_ptr<CXMLReader> src){

    }

    //calls ParseOSM and gives it source to use
    SImplementation(std::shared_ptr<CXMLReader> src){
        
    }

}