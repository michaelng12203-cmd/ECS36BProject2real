#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringDataSource.h"

TEST(XMLReaderTest, SimpleTest){
    std::string str = "<Genshin>Hello</Impact>";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    {CXMLReader Reader(Source);
    SXMLEntity entity;

    EXPECT_TRUE(Reader.ReadEntity(entity, true));
    EXPECT_EQ(entity.DNameData, "Genshin");

    }
}

TEST(XMLReaderTest, ElementTest){

}

TEST(XMLReaderTest, CDataTest){
    /*std::string str = "<rootUmamusume>Hello</root>";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    {
        CXMLReader Reader(Source);
        SXMLEntity entity;
        Reader.ReadEntity(entity, true);
        SXMLEntity entity2;
        Reader.ReadEntity(entity2, false);

        //std::cout<<entity.DType<<std::endl;
        std::cout<<entity2.DNameData<<std::endl;
        //std::cout<<entity.DAttributes<<std::endl;


    }*/
}

TEST(XMLReaderTest, LongCDataTest){

}

TEST(XMLReaderTest, SpecialCharacterTest){

}

TEST(XMLReaderTest, InvalidXMLTest){

}

TEST(XMLReaderTest, LongCharDataCrosses512Boundary){

}
