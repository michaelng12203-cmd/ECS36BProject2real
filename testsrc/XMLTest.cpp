#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringDataSource.h"

TEST(XMLReaderTest, SimpleTest){
    std::string str = "<Genshin>Hello</Genshin>";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    {
    CXMLReader Reader(Source);
    SXMLEntity entity;

    EXPECT_TRUE(Reader.ReadEntity(entity, true));
    EXPECT_EQ(entity.DNameData, "Genshin");
    }
}

TEST(XMLReaderTest, ElementTest){
    std::string str = "<Isagi number=\"11\" color=\"blue\" rank=\"1\">Striker</Isagi>";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    {
    CXMLReader Reader(Source);
    SXMLEntity entity;
    Reader.ReadEntity(entity, true);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);

    
    EXPECT_EQ(entity.AttributeValue("color"), "blue");

    SXMLEntity entity2;
    Reader.ReadEntity(entity2, true);
    EXPECT_EQ(entity2.DType, SXMLEntity::EType::EndElement);
    }
}

TEST(XMLReaderTest, CDataTest){
    std::string str = "<Kaiser>FrierentheSlayer</Kaiser>";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    {CXMLReader Reader(Source);
    SXMLEntity entity;
    Reader.ReadEntity(entity, false);
    
    SXMLEntity entity2;
    Reader.ReadEntity(entity2, false);
    EXPECT_EQ(entity2.DNameData, "FrierentheSlayer");
    }
}

TEST(XMLReaderTest, LongCDataTest){
    std::string str = "<Kaiser>GinGagamaruIkkiNikoOliverAikuImsoBritishAryuStylishRedPantherNesstheMagicianHioriYoBarouKingShoeiShidouRyuseiPinkDemonMeguruBachiraMonsterYellowTechnicianKingDribblerReoMikageCopyChameleonsukunawoncuzofplotfrfr</Kaiser>";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    {CXMLReader Reader(Source);
    SXMLEntity entity;
    Reader.ReadEntity(entity, false);
    
    SXMLEntity entity2;
    Reader.ReadEntity(entity2, false);
    EXPECT_EQ(entity2.DNameData, "GinGagamaruIkkiNikoOliverAikuImsoBritishAryuStylishRedPantherNesstheMagicianHioriYoBarouKingShoeiShidouRyuseiPinkDemonMeguruBachiraMonsterYellowTechnicianKingDribblerReoMikageCopyChameleonsukunawoncuzofplotfrfr");
    }
}
TEST(XMLReaderTest, SpecialCharacterTest){
    std::string str = "<Special>\'\"()+-/%</Special>";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    {
        CXMLReader Reader(Source);
        SXMLEntity entity;
        Reader.ReadEntity(entity, false);
        SXMLEntity entity2;
        Reader.ReadEntity(entity2, false);
        EXPECT_EQ(entity2.DNameData, "\'\"()+-/%");
    }
}

    TEST(XMLReaderTest, InvalidXMLTest){
    std::string str = "";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    std::string str2 = "HowDareYouCallThisBro<IamSoSad>";
    std::shared_ptr<CStringDataSource> Source2 = std::make_shared<CStringDataSource>(str2);

    {
        CXMLReader Reader(Source);
        SXMLEntity entity;

        EXPECT_FALSE(Reader.ReadEntity(entity, true));

        CXMLReader Reader2(Source2);
        SXMLEntity entity2;

        EXPECT_FALSE(Reader2.ReadEntity(entity2, true));
    }
}

TEST(XMLReaderTest, LongCharDataCrosses512Boundary){
    std::string str = "<SupDude>iuhfhuf83hjfui32hfuih23if32f3ifheuuuuuuuuuuuuuuuuuuufiuhewfiuwehiuhfhuf83hjfui32hfuih23if32f3ifheuuuuuuuuuuuuuuuuuuufiuhewfiuwehiuhfhuf83hjfui32hfuih23if32f3ifheuuuuuuuuuuuuuuuuuuufiuhewfiuwehiuhfhuf83hjfui32hfuih23if32f3ifheuuuuuuuuuuuuuuuuuuufiuhewfiuwehiuhfhuf83hjfui32hfuih23if32f3ifheuuuuuuuuuuuuuuuuuuufiuhewfiuwehiuhfhuf83hjfui32hfuih23if32f3ifheuuuuuuuuuuuuuuuuuuufiuhewfiuwehiuhfhuf83hjfui32hfuih23if32f3ifheuuuuuuuuuuuuuuuuuuufiuhewfiuwehiuhfhuf83hjfui32hfuih23if32f3ifheuuuuuuuuuuuuuuuuuuufiuhewfiuweh</SupDude>";
    std::shared_ptr<CStringDataSource> Source = std::make_shared<CStringDataSource>(str);

    {
        CXMLReader Reader(Source);
        SXMLEntity entity;
        Reader.ReadEntity(entity, false);

        SXMLEntity entity2;
        Reader.ReadEntity(entity2, false);

        EXPECT_EQ(entity2.DNameData.size(), 512);
    }
}
