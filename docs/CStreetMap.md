/*Many member functions in this class are pure virtual functions. They don't have implementation, that will be created and overridden in subclasses.*/

class CStreetMap{
    public:
        using TNodeID = uint64_t;//TNodeID is now equivalent to uint64_t
        using TWayID = uint64_t;//TWayID is now equivalent to uint64_t

        //This struct
        struct SLocation{
            double DLatitude;
            double DLongitude;  
            SLocation() = default;
            SLocation(const CStreetMap::SLocation &loc) = default;
            SLocation(double latitude, double longitude): DLatitude(latitude), DLongitude(longitude){};
            bool operator==(const SLocation &loc) const{
                return DLatitude == loc.DLatitude && DLongitude == loc.DLongitude;
            };
        };

        inline static constexpr TNodeID InvalidNodeID = std::numeric_limits<TNodeID>::max(); //Represents an invalid Node ID
        inline static constexpr TWayID InvalidWayID = std::numeric_limits<TWayID>::max();//Represents an invalid Way ID

        //this struct contains information every SNode object will have
        struct SNode{
            virtual ~SNode(){};
            virtual TNodeID ID() const noexcept = 0;
            virtual SLocation Location() const noexcept = 0;
            virtual std::size_t AttributeCount() const noexcept = 0;
            virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;
            virtual bool HasAttribute(const std::string &key) const noexcept = 0;
            virtual std::string GetAttribute(const std::string &key) const noexcept = 0;
        };

        //this struct contains information every SWay object will have
        struct SWay{
            virtual ~SWay(){};
            virtual TWayID ID() const noexcept = 0;
            virtual std::size_t NodeCount() const noexcept = 0;
            virtual TNodeID GetNodeID(std::size_t index) const noexcept = 0;
            virtual std::size_t AttributeCount() const noexcept = 0;
            virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;
            virtual bool HasAttribute(const std::string &key) const noexcept = 0;
            virtual std::string GetAttribute(const std::string &key) const noexcept = 0;
        };

        virtual ~CStreetMap(){};//destructor

        virtual std::size_t NodeCount() const noexcept = 0;//gives the number of nodes
        virtual std::size_t WayCount() const noexcept = 0;//gives the number of ways
        virtual std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept = 0;//gets a node at a certain index
        virtual std::shared_ptr<SNode> NodeByID(TNodeID id) const noexcept = 0;// gets a node by a certain ID
        virtual std::shared_ptr<SWay> WayByIndex(std::size_t index) const noexcept = 0;//gets a way by a certain index
        virtual std::shared_ptr<SWay> WayByID(TWayID id) const noexcept = 0;//gets a way by a certain ID
};