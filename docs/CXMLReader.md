struct CXMLReader::SImplementation{
    std::shared_ptr<CDataSource> Source; // The source that reads strings and inputs them into the parser
    std::queue<SXMLEntity> EntityQueue; // The queue of entities
    XML_Parser Parser; // Converts input data into processible output
}

// Constructor for CXMLReader, src specifies the data source
CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique<SImplementation>(src);
}

// Destructor for CXMLReader
CXMLReader::~CXMLReader(){

}

// Returns true when the queue is empty and there is no more input for the source to read
bool CXMLReader::End() const{

}

// Returns true when an entity is successfully read, entity is an SXMLEntity with DType, DNameData, and DAttributes. if skipcdata is true, the cdata in the parsed string is skipped
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){

}
