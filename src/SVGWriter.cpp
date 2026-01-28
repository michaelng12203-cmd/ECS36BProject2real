#include "SVGWriter.h"
#include "svg.h"
#include <string>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;

struct CSVGWriter::SImplementation{
    std::shared_ptr< CDataSink > DSink;
    svg_context_ptr DContext;

    static svg_return_t WriteFunction(svg_user_context_ptr user, const char *text){
        SImplementation *Implementation = (SImplementation *)user;
        while(*text){
            Implementation->DSink->Put(*text);
            text++;
        }
        return SVG_OK;   
    }


    SImplementation(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height){
        DSink = sink;
        DContext = svg_create(WriteFunction,nullptr,this,width,height);
    }

    ~SImplementation(){
        svg_destroy(DContext);
    }

    std::string CreateStyleString(const TAttributes &style){
        std::string style_string = "";
        for(const TAttribute &Attr : style){
            std::string key = std::get<0>(Attr);
            std::string value = std::get<1>(Attr);

            style_string = style_string + key + ":" + value + ";";
        }
        style_string = style_string.erase(style_string.length()-1);
        return style_string;
    }

    bool Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){
        svg_point_t Center{center.DX,center.DY};
        std::string Style = CreateStyleString(style);
        return svg_circle(DContext,&Center,radius,Style.c_str());
    }

    bool Rectangle(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){
        svg_point_t top_left = {topleft.DX, topleft.DY};
        svg_size_t sizes = {size.DWidth, size.DHeight};
        std::string Style = CreateStyleString(style);
        return svg_rect(DContext, &top_left, &sizes, Style.c_str());

    }
    bool Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style){
        svg_point_t Start = {start.DX, start.DY};
        svg_point_t End = {end.DX, end.DY};
        std::string Style = CreateStyleString(style);
        return svg_line(DContext, &Start, &End, Style.c_str());
    }

    bool SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style){
        std::string Style = CreateStyleString(style);
        for (int i = 0; i < (int) std::size(points); ++i){
            svg_point_t line_start = {points[i].DX,points[i].DY};
            svg_point_t line_end = {points[i+1].DX,points[i+1].DY};
            if (i == ((int) std::size(points) - 1)){
                return svg_line(DContext, &line_start, &line_end, Style.c_str());
            }else{
                svg_line(DContext, &line_start, &line_end, Style.c_str());
            }
        }
    }

    bool GroupBegin(const TAttributes &attrs){
        std::string Attrs = CreateStyleString(attrs);
        return svg_group_begin(DContext, Attrs.c_str());

    }

    bool GroupEnd(){
        return svg_group_end(DContext);
    }

};

CSVGWriter::CSVGWriter(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height){
    DImplementation = std::make_unique<SImplementation>(sink,width,height);
}

CSVGWriter::~CSVGWriter(){

}

bool CSVGWriter::Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){
    return DImplementation->Circle(center, radius,style);
}

bool CSVGWriter::Rectangle(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){
    return DImplementation->Rectangle(topleft,size,style);
}

bool CSVGWriter::Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style){
    return DImplementation->Line(start,end,style);
}

bool CSVGWriter::SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style){
    return DImplementation->SimplePath(points, style);
}

bool CSVGWriter::GroupBegin(const TAttributes &attrs){
    return DImplementation->GroupBegin(attrs);
}

bool CSVGWriter::GroupEnd(){
    return DImplementation->GroupEnd();
}
