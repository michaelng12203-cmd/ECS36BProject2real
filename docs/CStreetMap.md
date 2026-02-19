/*Many member functions in this class are pure virtual functions. They don't have implementation, that will be created and overridden in subclasses.*/

class CStreetMap{
    public:
        using TNodeID = uint64_t;//TNodeID is now equivalent to uint64_t
        using TWayID = uint64_t;//TWayID is now equivalent to uint64_t

        //this struct is a set of latitude and longitude values
        struct SLocation{
            double DLatitude;
            double DLongitude;
            SLocation() = default;
            SLocation(const CStreetMap::SLocation &loc) = default;//creates a SLocation with given SLocation parameters
            SLocation(double latitude, double longitude): DLatitude(latitude), DLongitude(longitude){};//creates a SLocation with given double parameters
            bool operator==(const SLocation &loc) const{
                return DLatitude == loc.DLatitude && DLongitude == loc.DLongitude; //compares the latitude and longitude of the SLocations
            };
        };

        inline static constexpr TNodeID InvalidNodeID = std::numeric_limits<TNodeID>::max(); //Represents an invalid Node ID
        inline static constexpr TWayID InvalidWayID = std::numeric_limits<TWayID>::max();//Represents an invalid Way ID

        //this struct contains information every SNode object will have
        struct SNode{
            virtual ~SNode(){};//destructor
            virtual TNodeID ID() const noexcept = 0;//the ID of the node
            virtual SLocation Location() const noexcept = 0;//the location of the node
            virtual std::size_t AttributeCount() const noexcept = 0;//how many attributes are in the node
            virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;//gets the key of the node attribute from the node based on the index inputted
            virtual bool HasAttribute(const std::string &key) const noexcept = 0;//returns true if the key has a node attribute that corresponds to it, returns false otherwise
            virtual std::string GetAttribute(const std::string &key) const noexcept = 0;//gets the value of the node attribute with the specified key 
        };

        //this struct contains information every SWay object will have
        struct SWay{
            virtual ~SWay(){};//destructor
            virtual TWayID ID() const noexcept = 0;//the ID of of the way
            virtual std::size_t NodeCount() const noexcept = 0;//the number of nodes in the way
            virtual TNodeID GetNodeID(std::size_t index) const noexcept = 0;//gets the ID of a node at a certain index
            virtual std::size_t AttributeCount() const noexcept = 0;//the number of attributes in the way

            virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;//gets the key of the way attribute from the node based on the index inputted
            virtual bool HasAttribute(const std::string &key) const noexcept = 0;//returns true if the key has a way attribute that corresponds to it, returns false otherwise
            virtual std::string GetAttribute(const std::string &key) const noexcept = 0;//gets the value of the way attribute with the specified key 
        };

        virtual ~CStreetMap(){};//destructor

        virtual std::size_t NodeCount() const noexcept = 0;//gives the number of nodes
        virtual std::size_t WayCount() const noexcept = 0;//gives the number of ways
        virtual std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept = 0;//gets a node at a certain index
        virtual std::shared_ptr<SNode> NodeByID(TNodeID id) const noexcept = 0;// gets a node by a certain ID
        virtual std::shared_ptr<SWay> WayByIndex(std::size_t index) const noexcept = 0;//gets a way by a certain index
        virtual std::shared_ptr<SWay> WayByID(TWayID id) const noexcept = 0;//gets a way by a certain ID
};