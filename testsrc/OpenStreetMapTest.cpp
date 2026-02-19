#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"

TEST(OpenStreetMapTest, SimpleTest){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                                "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
                                                                "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
                                                                "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    ASSERT_EQ(OpenStreetMap.NodeCount(), 2);
    auto Node = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(Node,nullptr);
    EXPECT_EQ(Node->ID(),1);
    auto Location = CStreetMap::SLocation{38.5,-121.7};
    EXPECT_EQ(Node->Location(),Location);
    ASSERT_EQ(OpenStreetMap.WayCount(), 1);
}

TEST(OpenStreetMapTest, NodeByIndex){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                                "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
                                                                "  <node id=\"65\" lat=\"38.9\" lon=\"567.8\"/>\n"
                                                                "  <way id=\"1001\">\n"
                                                                "    <nd ref=\"1\"/>\n"
                                                                "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeByIndex(2), nullptr);

    auto Node = OpenStreetMap.NodeByIndex(1);
    ASSERT_NE(Node,nullptr);
    EXPECT_EQ(Node->ID(),65);
    auto Location = CStreetMap::SLocation{38.9,567.8};
    EXPECT_EQ(Node->Location(),Location);
}

TEST(OpenStreetMapTest, NodeByID){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                                "  <node id=\"67\" lat=\"42.0\" lon=\"6.9\"/>\n"
                                                                "  <node id=\"500\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"1002\">\n"
                                                                "    <nd ref=\"1\"/>\n"
                                                                "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeByID(0), nullptr);
    
    auto Node = OpenStreetMap.NodeByID(67);
    ASSERT_NE(Node,nullptr);
    EXPECT_EQ(Node->ID(),67);
    auto Location = CStreetMap::SLocation{42.0,6.9};
    EXPECT_EQ(Node->Location(),Location);
}

TEST(OpenStreetMapTest, WayByID){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                                "  <node id=\"67\" lat=\"42.0\" lon=\"6.9\"/>\n"
                                                                "  <node id=\"123\" lat=\"17.38\" lon=\"-2.2222\"/>\n"
                                                                "  <way id=\"1003\">\n"
                                                                "    <nd ref=\"1234\"/>\n"
                                                                "    <nd ref=\"5678\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.WayByID(0), nullptr);
    
    auto Way = OpenStreetMap.WayByID(1003);
    ASSERT_NE(Way,nullptr);
    EXPECT_EQ(Way->ID(),1003);
    auto NodeID = Way->GetNodeID(0);
    EXPECT_EQ(NodeID,1234);
}

/*TEST(OpenStreetMapTest, WayByID){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                                "  <node id=\"67\" lat=\"42.0\" lon=\"6.9\"/>\n"
                                                                "  <node id=\"123\" lat=\"17.38\" lon=\"-2.2222\"/>\n"
                                                                "  <way id=\"1003\">\n"
                                                                "    <nd ref=\"1234\"/>\n"
                                                                "    <nd ref=\"5678\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.WayByID(0), nullptr);
    
    auto Way = OpenStreetMap.WayByID(1003);
    ASSERT_NE(Way,nullptr);
    EXPECT_EQ(Way->ID(),1003);
    auto NodeID = Way->GetNodeID(0);
    EXPECT_EQ(NodeID,1234);
}*/

TEST(OpenStreetMapTest, TagTest){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                                "  <node id=\"67\" lat=\"42.0\" lon=\"6.9\"/>\n"
                                                                "  <node id=\"123\" lat=\"17.38\" lon=\"-2.2222\"/>\n"
                                                                "  <way id=\"1003\">\n"
                                                                "    <nd ref=\"1234\"/>\n"
                                                                "    <nd ref=\"5678\"/>\n"
                                                                "    <tag k=\"supdude\" v=\"dingdongjohn\"/>\n"
                                                                "    <tag k=\"balls\" v=\"nai\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto Way = OpenStreetMap.WayByID(1003);

    std::cout<<Way->AttributeCount()<<std::endl;

    auto badAttribute = Way->GetAttribute("g");
    EXPECT_EQ(badAttribute, std::string());

    auto badAttribute2 = Way->GetAttributeKey(6);
    EXPECT_EQ(badAttribute2, std::string());
    
    auto attribute = Way->GetAttribute("k");
    EXPECT_EQ(attribute,"supdude");

    auto attribute2 = Way->GetAttributeKey(1);
    EXPECT_EQ(attribute2,"v");

    auto attribute3 = Way->GetAttributeKey(2);
    EXPECT_EQ(attribute3,"k");
}


// test tags
// super long ids
// crazy longitude latitudes
// bad node id
// bad node index