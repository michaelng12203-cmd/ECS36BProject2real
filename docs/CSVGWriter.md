#include "SVGWriter.h"
#include "svg.h"
#include <string>
#include <vector>
#include <iostream>



struct CSVGWriter::SImplementation{
std::shared_ptr< CDataSink > DSink; // Takes input from the Writer and creates a string to output
    svg_context_ptr DContext; // The collection of data that holds the output of Writer functions such as Circle, Rectangle, etc.
    
    //Constructor for SImplementation, initializes DContext and DSink, width and height are used as parameters to call svg_create
    SImplementation(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height){
     
    }
    //Destructor for SImplementation, calls svg_destroy to destroy context
    ~SImplementation(){
       
    }
    Takes the attrbutes and converts them into a string style format
    std::string CreateStyleString(const TAttributes &style){

    }
    //Draws a circle using given parameters and style from CreateStyleString, returns true on success, false on failure
    bool Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){
        
    }
    //Draws a rectangle using given parameters and style from CreateStyleString, returns true on success, false on failure
    bool Rectangle(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){

    }
    //Draws a line using given parameters and style from CreateStyleString, returns true on success, false on failure
    bool Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style){

    }
    //Draws a path made up of multiple lines, using given parameters and style from CreateStyleString, returns true on success, false on failure
    bool SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style){

    }
    //Calls svg_group_begin to begin an SVG group using attrs from CreateStyleString, returns true on success, false on failure
    bool GroupBegin(const TAttributes &attrs){

    }
    //Calls svg_group_end to end an SVG group, returns true on success, false on failure
    bool GroupEnd(){

    }

};

//Constructor for SVGWriter, sink is the output that writes the string
CSVGWriter::CSVGWriter(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height){

}

// Destructor for SVGWriter
CSVGWriter::~CSVGWriter(){

}
