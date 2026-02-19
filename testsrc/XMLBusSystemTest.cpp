#include <gtest/gtest.h>
#include "XMLBusSystem.h"
#include "StringDataSource.h"

TEST(XMLBusSystemTest, SimpleTest){
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "<routes>\n"
                                                                "<route name=\"A\">\n"
                                                                "<routestop stop=\"22061\" />\n"
                                                                "<routestop stop=\"92542\" />\n"
                                                                "</route>\n"
                                                                "<route name=\"B\">\n"
                                                                "<routestop stop=\"60683\" />\n"
                                                                "<routestop stop=\"10593\" />\n"
                                                                "<routestop stop=\"11115\" />\n"
                                                                "</route>\n"
                                                                "</routes>\n"
"                                                               </bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    ASSERT_EQ(BusSystem.StopCount(),2);
    EXPECT_NE(BusSystem.StopByIndex(0),nullptr);
    EXPECT_NE(BusSystem.StopByIndex(1),nullptr);
    auto StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),1);
    EXPECT_EQ(StopObj->NodeID(),321);
    EXPECT_EQ(StopObj->Description(),"First");
    StopObj = BusSystem.StopByID(2);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),2);
    EXPECT_EQ(StopObj->NodeID(),311);
    EXPECT_EQ(StopObj->Description(),"second");

    EXPECT_EQ(BusSystem.RouteCount(),2);
    EXPECT_NE(BusSystem.RouteByIndex(0),nullptr);
    EXPECT_NE(BusSystem.RouteByIndex(1),nullptr);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->Name(),"A");
    EXPECT_EQ(RouteObj->StopCount(), 2);
    EXPECT_EQ(RouteObj->GetStopID(0), 22061);
    RouteObj = BusSystem.RouteByName("B");
    ASSERT_NE(RouteObj, nullptr);
    EXPECT_EQ(RouteObj->Name(),"B");
    EXPECT_EQ(RouteObj->StopCount(), 3);
    EXPECT_EQ(RouteObj->GetStopID(2), 11115);
    
    auto PathObj = BusSystem.PathByStopIDs(321,311);
    ASSERT_NE(PathObj, nullptr);
    EXPECT_EQ(PathObj->StartNodeID(), 321);
    EXPECT_EQ(PathObj->EndNodeID(), 311);
    EXPECT_EQ(PathObj->NodeCount(), 3);
    EXPECT_EQ(PathObj->GetNodeID(1), 315);

}


//TEST(XMLBusSystemTest, ){




//}
