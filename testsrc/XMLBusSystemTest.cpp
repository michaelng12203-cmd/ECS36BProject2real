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

    
    std::string Kaiser = StopObj->Description("Emperor");
    EXPECT_EQ(Kaiser, StopObj->Description());
    EXPECT_EQ(Kaiser, "Emperor");
}


TEST(XMLBusSystemTest, InvalidTags){
    auto BusRouteSource1 = std::make_shared<CStringDataSource>(  "<notbussystem>\n"                      
"                                                               </notbussystem>");
    auto BusRouteReader1 = std::make_shared< CXMLReader >(BusRouteSource1);
    auto BusPathSource1 = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader1 = std::make_shared< CXMLReader >(BusPathSource1);
    CXMLBusSystem BusSystem1(BusRouteReader1,BusPathReader1);
    EXPECT_EQ(BusSystem1.StopCount(), 0);
    EXPECT_EQ(BusSystem1.RouteCount(), 0);
    EXPECT_EQ(BusSystem1.PathByStopIDs(3,3), nullptr);

    auto BusRouteSource2 = std::make_shared<CStringDataSource>(  "<bussystem>\n"  
                                                                "<notstops>\n"
                                                                "</notstops>\n"                 
                                                            "</bussystem>");
    auto BusRouteReader2 = std::make_shared< CXMLReader >(BusRouteSource2);
    auto BusPathSource2 = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader2 = std::make_shared< CXMLReader >(BusPathSource2);
    CXMLBusSystem BusSystem2(BusRouteReader2,BusPathReader2);
    EXPECT_EQ(BusSystem2.StopCount(), 0);
    EXPECT_EQ(BusSystem2.RouteCount(), 0);
    EXPECT_EQ(BusSystem2.PathByStopIDs(3,3), nullptr);

    auto BusRouteSource3 = std::make_shared<CStringDataSource>(  "<bussystem>\n"  
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "</stops>\n"  
                                                                "<notroutes>\n"  
                                                                "</notroutes>\n"                                                                         
                                                            "</bussystem>");
    auto BusRouteReader3 = std::make_shared< CXMLReader >(BusRouteSource3);
    auto BusPathSource3 = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader3 = std::make_shared< CXMLReader >(BusPathSource3);
    CXMLBusSystem BusSystem3(BusRouteReader3,BusPathReader3);
    EXPECT_EQ(BusSystem3.RouteCount(), 0);
    EXPECT_EQ(BusSystem3.PathByStopIDs(3,3), nullptr);

    auto BusRouteSource4 = std::make_shared<CStringDataSource>(  "<bussystem>\n"  
                                                                "<stops>\n"
                                                               "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                               "</stops>\n" 
                                                                "<routes>\n"  
                                                                "<route name=\"A\">\n"
                                                                "<routestop stop=\"22061\" />\n"
                                                                "</route>\n"
                                                                "</routes>\n"                                                                          
                                                            "</bussystem>");
    auto BusRouteReader4 = std::make_shared< CXMLReader >(BusRouteSource4);
    auto BusPathSource4 = std::make_shared<CStringDataSource>(  "<notpaths>\n"
                                                                "</notpaths>");
    auto BusPathReader4 = std::make_shared< CXMLReader >(BusPathSource4);
    CXMLBusSystem BusSystem4(BusRouteReader4,BusPathReader4);
    EXPECT_EQ(BusSystem4.PathByStopIDs(3,3), nullptr);
}

TEST(XMLBusSystemTest, IncompleteTags){
    auto BusRouteSource1 = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"\n"
                                                                "</stops>\n"
                                                                "<routes>\n"
                                                                "</bussystem>\n" );
    auto BusRouteReader1 = std::make_shared< CXMLReader >(BusRouteSource1);
    auto BusPathSource1 = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader1 = std::make_shared< CXMLReader >(BusPathSource1);
    CXMLBusSystem BusSystem1(BusRouteReader1,BusPathReader1);
    EXPECT_EQ(BusSystem1.StopCount(), 0);

}

TEST(XMLBusSystemTest, InvalidIndex){

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

    auto RouteObj = BusSystem.RouteByName("B");
    EXPECT_EQ(RouteObj->GetStopID(7), NULL);

    auto PathObj = BusSystem.PathByStopIDs(321,311);
    EXPECT_EQ(PathObj->GetNodeID(8), NULL);


    EXPECT_EQ(BusSystem.StopByIndex(9),nullptr);
    EXPECT_EQ(BusSystem.RouteByIndex(10),nullptr);


    auto BusRouteSource1 = std::make_shared<CStringDataSource>(  "");
    auto BusRouteReader1 = std::make_shared< CXMLReader >(BusRouteSource1);
    auto BusPathSource1 = std::make_shared<CStringDataSource>(  "");
    auto BusPathReader1 = std::make_shared< CXMLReader >(BusPathSource1);
    CXMLBusSystem BusSystem1(BusRouteReader1,BusPathReader1);

        EXPECT_EQ(BusSystem.StopByID(5), nullptr);
        EXPECT_EQ(BusSystem.RouteByName("M"), nullptr);


}

TEST(XMLBusSystemTest, LargeData){
    auto BusRouteSource = std::make_shared<CStringDataSource>("<bussystem>\n"
    "<stops>\n"
        "<stop id=\"22000\" node=\"95712762\" description=\"Hutchison Dr &amp; Old Davis Road (EB)\" />\n"
        "<stop id=\"22001\" node=\"95712762\" description=\"Hutchison Drive &amp; Old Davis Road (WB)\" />\n"
        "<stop id=\"22002\" node=\"95717425\" description=\"1st St &amp; C St (EB)\" />\n"
        "<stop id=\"22003\" node=\"95717604\" description=\"1st St &amp; D St (WB)\" />\n"
        "<stop id=\"22004\" node=\"95719227\" description=\"Richards Blvd &amp; Olive Dr (SB)\" />\n"
        "<stop id=\"22005\" node=\"2584424865\" description=\"2nd St &amp; B St (EB)\" />\n"
        "<stop id=\"22006\" node=\"95717425\" description=\"C St &amp; 1st St (SB)\" />\n"
        "<stop id=\"22007\" node=\"5674148946\" description=\"2nd St &amp; E St (EB)\" />\n"
        "<stop id=\"22009\" node=\"5274411946\" description=\"2nd St &amp; E St (WB)\" />\n"
        "<stop id=\"22011\" node=\"575319453\" description=\"H St &amp; Amtrak Station (NB)\" />\n"
        "<stop id=\"220121375057318950\" node=\"567442768167148800\" description=\"nuvowvbnwvuorwbnvrwubvowvnjewuignvweivpodmjvisovmoguvnrobr\" />\n"
        "<stop id=\"22013\" node=\"95713485\" description=\"B St &amp; 4th St (NB)\" />\n"
        "<stop id=\"22014\" node=\"95713485\" description=\"B St &amp; 4th St (SB)\" />\n"
        "<stop id=\"22015\" node=\"95711355\" description=\"3rd St &amp; C St (WB)\" />\n"
        "<stop id=\"22016\" node=\"95711355\" description=\"3rd St &amp; C St (EB)\" />\n"
        "<stop id=\"22017\" node=\"5674148942\" description=\"3rd St &amp; E St (WB)\" />\n"
        "<stop id=\"22018\" node=\"5674148941\" />\n"
        "<stop id=\"22019\" node=\"573919278\" />\n"
        "<stop id=\"22020\" node=\"95718594\" />\n"
    "</stops>\n"
    "<routes>\n"
        "<route name=\"A\">\n"
            "<routestop stop=\"22258\" />\n"
        "</route>\n"
        "<route name=\"B\">\n"
            "<routestop stop=\"22274\" />\n"
            "<routestop stop=\"22230\" />\n"
            "<routestop stop=\"22185\" />\n"
            "<routestop stop=\"22189\" />\n"
            "<routestop stop=\"22188\" />\n"
            "<routestop stop=\"22186\" />\n"
            "<routestop stop=\"22191\" />\n"
            "<routestop stop=\"22193\" />\n"
            "<routestop stop=\"22194\" />\n"
            "<routestop stop=\"22195\" />\n"
        "</route>\n"
        "<route name=\"C\">\n"
            "<routestop stop=\"22256\" />\n"
            "<routestop stop=\"22239\" />\n"
            "<routestop stop=\"22361\" />\n"
            "<routestop stop=\"22225\" />\n"
            "<routestop stop=\"22221\" />\n"
            "<routestop stop=\"22226390876391066\" />\n"
            "<routestop stop=\"22227\" />\n"
        "</route>\n"
    "</routes>\n"
"</bussystem>\n");
        auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "<path source=\"260743657890\" destination=\"562412582\">\n"
                                                                "<node id=\"2607436578\" />\n"
                                                                "<node id=\"559373757\" />\n"
                                                                "<node id=\"559748659\" />\n"
                                                                "<node id=\"5674106219\" />\n"
                                                                "<node id=\"2276471063\" />\n"
                                                                "<node id=\"95708764\" />\n"
                                                                "<node id=\"2276471058\" />\n"
                                                                "<node id=\"277606337\" />\n"
                                                                "<node id=\"562412582\" />\n"
                                                                "</path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    ASSERT_EQ(BusSystem.StopCount(),19);
    EXPECT_NE(BusSystem.StopByIndex(0),nullptr);
    EXPECT_NE(BusSystem.StopByIndex(1),nullptr);
    auto StopObj = BusSystem.StopByID(220121375057318950);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),220121375057318950);
    EXPECT_EQ(StopObj->NodeID(),567442768167148800);
    EXPECT_EQ(StopObj->Description(),"nuvowvbnwvuorwbnvrwubvowvnjewuignvweivpodmjvisovmoguvnrobr");

    EXPECT_EQ(BusSystem.RouteCount(),3);
    EXPECT_NE(BusSystem.RouteByIndex(1),nullptr);
    EXPECT_NE(BusSystem.RouteByIndex(2),nullptr);
    auto RouteObj = BusSystem.RouteByName("C");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->Name(),"C");
    EXPECT_EQ(RouteObj->StopCount(), 7);
    EXPECT_EQ(RouteObj->GetStopID(5), 22226390876391066);
    
    auto PathObj = BusSystem.PathByStopIDs(260743657890,562412582);
    ASSERT_NE(PathObj, nullptr);
    EXPECT_EQ(PathObj->StartNodeID(), 260743657890);
    EXPECT_EQ(PathObj->EndNodeID(), 562412582);
    EXPECT_EQ(PathObj->NodeCount(), 9);
    EXPECT_EQ(PathObj->GetNodeID(6), 2276471058);

}