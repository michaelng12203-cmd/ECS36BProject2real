#include <gtest/gtest.h>
#include "SVGWriter.h"
#include "StringDataSink.h"

TEST(SVGWriterTest, CreateDestroyTest){
  std::shared_ptr<CStringDataSink> Sink = std::make_shared<CStringDataSink>();

    {CSVGWriter Writer(Sink,100,100);}
    EXPECT_EQ(Sink->String(),"<?xml version=\"1.0\" encoding=\"UTF-8\"?><svg width=\"100\" height=\"100\" xmlns=\"http://www.w3.org/2000/svg\"></svg>");
}

TEST(SVGWriterTest, CircleTest){

    std::shared_ptr<CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    TAttributes attrs = {{"fill", "none"}, {"stroke", "green"}, {"stroke-width", "2"}};
    SSVGPoint center = {50,50};
    {
        CSVGWriter Writer(Sink,100,100);
        Writer.Circle(center, 45, attrs);
    }
    
    EXPECT_NE(Sink->String().find("<circle"), std::string::npos);
    EXPECT_NE(Sink->String().find("cx=\"50.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("cy=\"50.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("r=\"45.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("style=\"fill:none;"), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke:green;"), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke-width:2"), std::string::npos);

}

TEST(SVGWriterTest, RectangleTest){
    std::shared_ptr<CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    {
        CSVGWriter Writer(Sink,100,100);
        TAttributes attrs = {{"fill", "red"}, {"stroke", "blue"}, {"stroke-width", "11"}};
        SSVGPoint topleft = {50,50};
        SSVGSize size = {30,40};

        Writer.Rectangle(topleft, size, attrs);
    }
    
    EXPECT_NE(Sink->String().find("<rect"), std::string::npos);
    EXPECT_NE(Sink->String().find("x=\"50.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y=\"50.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("width=\"30.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("height=\"40.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("style=\"fill:red;"), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke:blue;"), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke-width:11"), std::string::npos);
}

TEST(SVGWriterTest, LineTest){
    std::shared_ptr<CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    {
        CSVGWriter Writer(Sink,100,100);
        TAttributes attrs = {{"stroke", "white"}, {"stroke-width", "8"}};
        SSVGPoint start = {20,20};
        SSVGPoint end = {80,80};

        Writer.Line(start, end, attrs);
    }

    EXPECT_NE(Sink->String().find("<line"), std::string::npos);
    EXPECT_NE(Sink->String().find("x1=\"20.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y1=\"20.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("x2=\"80.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y2=\"80.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke:white;"), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke-width:8"), std::string::npos);
}

TEST(SVGWriterTest, SimplePathTest){
    std::shared_ptr<CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    std::vector<SSVGPoint> points = {{30, 30}, {67, 67}, {50, 50}, {90, 20}, {10, 45}, {35, 70}};
    TAttributes attrs = {{"stroke", "black"}, {"stroke-width", "4"}};
    {
        CSVGWriter Writer(Sink,100,100);
        Writer.SimplePath(points, attrs);
    }

    EXPECT_NE(Sink->String().find("<line"), std::string::npos);
    EXPECT_NE(Sink->String().find("x1=\"30.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y1=\"30.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("x2=\"67.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y2=\"67.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke:black;"), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke-width:4"), std::string::npos);

    EXPECT_NE(Sink->String().find("<line"), std::string::npos);
    EXPECT_NE(Sink->String().find("x1=\"50.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y1=\"50.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("x2=\"90.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y2=\"20.000000\""), std::string::npos);

    EXPECT_NE(Sink->String().find("<line"), std::string::npos);
    EXPECT_NE(Sink->String().find("x1=\"10.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y1=\"45.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("x2=\"35.000000\""), std::string::npos);
    EXPECT_NE(Sink->String().find("y2=\"70.000000\""), std::string::npos);
}

TEST(SVGWriterTest, GroupTest){

    std::shared_ptr<CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    TAttributes attrs = {{"stroke", "pink"}, {"stroke-width", "33"}};
    {
        CSVGWriter Writer(Sink,100,100);
        Writer.GroupBegin(attrs);
        Writer.GroupEnd();
    }
    EXPECT_NE(Sink->String().find("stroke:pink;"), std::string::npos);
    EXPECT_NE(Sink->String().find("stroke-width:33"), std::string::npos);
    EXPECT_NE(Sink->String().find("/g"), std::string::npos);

}

class CFailingSink : public CDataSink{
    public:
        int DValidCalls = 0;
        virtual ~CFailingSink(){};
        bool Put(const char &ch) noexcept override{
            if(DValidCalls){
                DValidCalls--;
                return true;
            }
            return false;
        }

        bool Write(const std::vector<char> &buf) noexcept override{
            if(DValidCalls){
                DValidCalls--;
                return true;
            }
            return false;
        }
};

TEST(SVGWriterTest, ErrorTests){
    std::shared_ptr<CFailingSink> Sink = std::make_shared<CFailingSink>();

    TAttributes attrs_1 = {{"fill", "none"}, {"stroke", "green"}, {"stroke-width", "2"}};
    TAttributes attrs2 = {{"fill", "red"}, {"stroke", "blue"}, {"stroke-width", "11"}};
    TAttributes attrs3 = {{"stroke", "white"}, {"stroke-width", "8"}};
    TAttributes attrs4 = {{"stroke", "black"}, {"stroke-width", "4"}};
    TAttributes attrs5 = {{"stroke", "pink"}, {"stroke-width", "33"}};

        SSVGPoint center = {50,50};
        SSVGPoint topleft = {50,50};
        SSVGSize size = {30,40};
        SSVGPoint start = {20,20};
        SSVGPoint end = {80,80};
    std::vector<SSVGPoint> points = {{30, 30}, {67, 67}, {50, 50}, {90, 20}, {10, 45}, {35, 70}};
    {
        CSVGWriter Writer(Sink,100,100);
        EXPECT_FALSE(Writer.Circle(center, 45, attrs_1));
        EXPECT_FALSE(Writer.Rectangle(topleft, size, attrs2));
        EXPECT_FALSE(Writer.Line(start, end, attrs3));
        EXPECT_FALSE(Writer.SimplePath(points, attrs4));
        EXPECT_FALSE(Writer.GroupBegin(attrs5));
        EXPECT_FALSE(Writer.GroupEnd());
    }
}
