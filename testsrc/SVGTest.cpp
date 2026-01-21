#include "svg.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Helper structure to capture written SVG strings
struct STestOutput{
    std::vector<std::string> DLines;
    bool DDestroyed = false;

    std::string JoinOutput(){
        std::string Result;

        for (const auto& line : DLines){
            Result += line;
        }
        return Result;
    }
};

// Callback to capture SVG output
svg_return_t write_callback(svg_user_context_ptr user, const char* text){
    if(!user || !text){
        return SVG_ERR_NULL;
    }
    STestOutput* OutPtr = static_cast<STestOutput*>(user);
    OutPtr->DLines.push_back(text);
    return SVG_OK;
}

// Cleanup callback (just returns OK for testing)
svg_return_t cleanup_callback(svg_user_context_ptr user){
    if(!user){
        return SVG_ERR_NULL;
    }
    STestOutput* OutPtr = static_cast<STestOutput*>(user);
    if(OutPtr->DDestroyed){
        return SVG_ERR_STATE;
    }
    OutPtr->DDestroyed = true;
    return SVG_OK;
}

// --- TEST FIXTURE ---
class SVGTest : public ::testing::Test{
    protected:
        STestOutput DOutput;
        svg_context_ptr DContext = nullptr;

        void SetUp() override{
            DContext = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
            ASSERT_NE(DContext, nullptr);
        }

        void TearDown() override{
            if(DContext){
                svg_destroy(DContext);
                DContext = nullptr;
            }
        }
};

// --- BASIC CREATION TEST ---
TEST_F(SVGTest, CreateAndDestroy){
    svg_context_ptr context = svg_create(write_callback,
                                         cleanup_callback,
                                         &DOutput,
                                         100,
                                         100);
    EXPECT_NE(context,nullptr);
    EXPECT_EQ(svg_destroy(context),SVG_OK);
}

// --- INVALID INPUT TESTS ---
TEST_F(SVGTest, NullContextFunctions){

   svg_point_t center = {50,50};
    svg_point_t top_left = {50,50};
    svg_size_t size = {30,40};
    const svg_point_t start = {10,10};
    svg_point_t end = {90, 90};

    EXPECT_EQ(svg_circle(NULL, &center, 45, "fill:none; stroke:green; stroke-width:2"), SVG_ERR_NULL);
    EXPECT_EQ(svg_rect(NULL, &top_left, &size, "fill:none; stroke:green; stroke-width:2"), SVG_ERR_NULL);
    EXPECT_EQ(svg_line(NULL, &start, &end, "stroke:green; stroke-width:2"), SVG_ERR_NULL);
}



// --- DRAWING TESTS ---
TEST_F(SVGTest, Circle){

    svg_point_t center = {50,50};
    svg_circle(DContext, &center, 45,"fill:none; stroke:green; stroke-width:2");
    
    std::string Allout = DOutput.JoinOutput();
    EXPECT_NE(Allout.find("<circle"), std::string::npos);
    EXPECT_NE(Allout.find("cx=\"50.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("cy=\"50.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("r=\"45.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("style=\"fill:none; stroke:green; stroke-width:2\""), std::string::npos);
}

TEST_F(SVGTest, Rectangle){
    
    svg_point_t top_left = {50,50};
    svg_size_t size = {30,40};
    svg_rect(DContext, &top_left, &size,"fill:none; stroke:green; stroke-width:2");


    std::string Allout = DOutput.JoinOutput();
    EXPECT_NE(Allout.find("<rect"), std::string::npos);
    EXPECT_NE(Allout.find("x=\"50.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("y=\"50.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("width=\"30.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("height=\"40.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("style=\"fill:none; stroke:green; stroke-width:2\""), std::string::npos);
}

TEST_F(SVGTest, Line){
    
    const svg_point_t start = {10,10};
    svg_point_t end = {90, 90};
    svg_line(DContext, &start, &end, "stroke:green; stroke-width:2");

    std::string Allout = DOutput.JoinOutput();
    EXPECT_NE(Allout.find("<line"), std::string::npos);
    EXPECT_NE(Allout.find("x1=\"10.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("y1=\"10.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("x2=\"90.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("y2=\"90.000000\""), std::string::npos);
    EXPECT_NE(Allout.find("style=\"stroke:green; stroke-width:2\""), std::string::npos);
}

// --- GROUPING TEST ---
TEST_F(SVGTest, Grouping){
    
    svg_group_begin(DContext, "stroke:blue; stroke-width:2>");
    svg_group_end(DContext);

    std::string Allout = DOutput.JoinOutput();
    EXPECT_NE(Allout.find("<g"), std::string::npos);
    EXPECT_NE(Allout.find("stroke:blue; stroke-width:2>"), std::string::npos);
    EXPECT_NE(Allout.find("</g>"), std::string::npos);

}





// --- EDGE CASES ---
TEST_F(SVGTest, ZeroDimensions){
    
    svg_point_t center = {50,50};
    svg_point_t top_left = {50,50};
    svg_size_t size = {0,0};

    EXPECT_EQ(svg_circle(DContext, &center, 0, "fill:none; stroke:green; stroke-width:2"), SVG_ERR_INVALID_ARG);
    EXPECT_EQ(svg_rect(DContext, &top_left, &size, "fill:none; stroke:green; stroke-width:2"), SVG_ERR_INVALID_ARG);


}



TEST_F(SVGTest, NullPointPointer){
    
    svg_size_t size = {30,40};

    EXPECT_EQ(svg_circle(DContext, nullptr, 45, "fill:none; stroke:green; stroke-width:2"), SVG_ERR_NULL);
    EXPECT_EQ(svg_rect(DContext, nullptr, &size, "fill:none; stroke:green; stroke-width:2"), SVG_ERR_NULL);
    EXPECT_EQ(svg_line(DContext, nullptr, nullptr, "fill:none; stroke:green; stroke-width:2"), SVG_ERR_NULL);
}



TEST_F(SVGTest, CreateEdgeCases){
    
    EXPECT_EQ(svg_create(write_callback, cleanup_callback, &DOutput, 0, 0), nullptr);
    EXPECT_EQ(svg_create(nullptr, cleanup_callback, &DOutput, 100, 100), nullptr);
}   

TEST_F(SVGTest, DestroyEdgeCases){
    
svg_context_ptr context = svg_create(write_callback,cleanup_callback,&DOutput,100,100);    
svg_return_t destroy = svg_destroy(context);

    std::string Allout = DOutput.JoinOutput();
    EXPECT_NE(Allout.find("</svg>"), std::string::npos);
    EXPECT_EQ(svg_destroy(NULL), SVG_ERR_NULL);

}

// Callback to capture SVG output
svg_return_t write_error_callback(svg_user_context_ptr user, const char* text){
    int *FailureCount = (int *)user;
    if(*FailureCount){
        (*FailureCount)--;
        return SVG_OK;    
    }
    return SVG_ERR_IO;
}

// --- IO Errors ---
TEST_F(SVGTest, IOErrorTest){

    svg_point_t center = {50,50};
    svg_point_t top_left = {50,50};
    svg_size_t size = {30,40};
    const svg_point_t start = {10,10};
    svg_point_t end = {90, 90};

    EXPECT_EQ(svg_circle(DContext, &center, 45, NULL), SVG_ERR_IO);
    EXPECT_EQ(svg_rect(DContext, &top_left, &size, NULL), SVG_ERR_IO);
    EXPECT_EQ(svg_line(DContext, &start, &end, NULL), SVG_ERR_IO);
    EXPECT_EQ(svg_group_begin(DContext, NULL), SVG_ERR_IO);
}
