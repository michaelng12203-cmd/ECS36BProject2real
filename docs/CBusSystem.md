/*Every member function in this class is a pure virtual function. They don't have implementation, that will be created and overriden in subclasses.*/

class CBusSystem{
    public:
        using TStopID = uint64_t; //TStopID is now equivalent to uint64_t

        inline static constexpr TStopID InvalidStopID = std::numeric_limits<TStopID>::max(); /*represents an invalid or missing stop ID*/

        //this struct contains the information that every SStop object will have
        struct SStop{
            virtual ~SStop(){}; //destructor
            virtual TStopID ID() const noexcept = 0; //the ID of the stop
            virtual CStreetMap::TNodeID NodeID() const noexcept = 0;//the node ID of the stop
            virtual std::string Description() const noexcept = 0;//the description of the stop
            virtual std::string Description(const std::string &description) noexcept = 0;/*also the description of the stop, but is set equal to the given parameter*/
        };

        //this struct contains the information every SRoute object will have
        struct SRoute{
            virtual ~SRoute(){}; //destructor
            virtual std::string Name() const noexcept = 0;//the name of the route
            virtual std::size_t StopCount() const noexcept = 0;//How many stops are in the route
            virtual TStopID GetStopID(std::size_t index) const noexcept = 0;/* gets a certain stop of the route, specified by index*/
        };

        //this struct contains the information every SPath object will have
        struct SPath{
            virtual ~SPath(){};//destructor
            virtual CStreetMap::TNodeID StartNodeID() const noexcept = 0;//the start node ID for the path
            virtual CStreetMap::TNodeID EndNodeID() const noexcept = 0;//the end node ID for the path
            virtual std::size_t NodeCount() const noexcept = 0;//how many nodes there are in the path
            virtual CStreetMap::TNodeID GetNodeID(std::size_t index) const noexcept = 0;/*gets a certain node in the path, specified by index*/
        };

        virtual ~CBusSystem(){};

        virtual std::size_t StopCount() const noexcept = 0;//gets stopcount
        virtual std::size_t RouteCount() const noexcept = 0;//gets routecount
        virtual std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept = 0;//gets stop at a certain index
        virtual std::shared_ptr<SStop> StopByID(TStopID id) const noexcept = 0;//gets a stop at a certain ID
        virtual std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept = 0;//gets a route by a certain diex
        virtual std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept = 0;//gets a route by a name
        virtual std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const noexcept = 0;/*gets a path by certain //start and end IDs*/
};