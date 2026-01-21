/**
 * @file svg.c
 * @brief Implementation of simple SVG drawing interface.
 *
 * Implements the basic functions for creating SVG documents.
 */
#include "svg.h"
#include <stdlib.h>
#include <stdio.h>


/**
 * @brief Opaque SVG drawing context.
 *
 * Holds the necessary data to implement functions.
 */
struct SVG_CONTEXT{
    svg_write_fn write_fn;
    svg_cleanup_fn cleanup_fn;
    svg_user_context_ptr user;
};



svg_context_ptr svg_create(svg_write_fn write_fn, 
                           svg_cleanup_fn cleanup_fn, 
                           svg_user_context_ptr user, 
                           svg_px_t width, 
                           svg_px_t height){
    svg_context_ptr context = (svg_context_ptr)malloc(sizeof(svg_context_t));
    context->write_fn = write_fn;
    context->cleanup_fn = cleanup_fn;
    context->user = user;

     if(width <= 0 || height <= 0){
        return NULL;
    }
    if(write_fn == NULL){
        return NULL;    
    }

    if(user == NULL){
        return NULL;    
    }

    int size = snprintf(NULL, 0,"<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">",
                        width, height);
    char *buffer = (char *)malloc(size + 1);
    snprintf(buffer, size, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">",
                        width, height);

    write_fn(user, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    write_fn(user, "\n");
    write_fn(user, buffer);
    free(buffer);
    write_fn(user, ">\n");

    return context;

}

svg_return_t svg_destroy(svg_context_ptr context){

    if(context){
        context->write_fn(context->user, "</svg>");
        free(context);
        return SVG_OK;
    }
    return SVG_ERR_NULL;
}


svg_return_t svg_circle(svg_context_ptr context,
                        const svg_point_t *center,
                        svg_real_t radius,
                        const char *style){

    if(style == NULL){
        return SVG_ERR_IO;
    }
    if(radius <= 0){
        return SVG_ERR_INVALID_ARG;
    }
    if(center == NULL){
        return SVG_ERR_NULL;
    }
    if(context == NULL){
        return SVG_ERR_NULL;
    }
    int size = snprintf(NULL, 0,"<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>",
                        center->x,center->y,radius, style);
    char *buffer = (char *)malloc(size + 1);
    snprintf(buffer, size, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>",
                        center->x,center->y,radius, style);
    svg_return_t circle = context->write_fn(context->user, buffer);
    free(buffer);
    context->write_fn(context->user, ">\n");

    return circle;
}


svg_return_t svg_rect(svg_context_ptr context,
                      const svg_point_t *top_left,
                      const svg_size_t *size,
                      const char* style){

    if(style == NULL){
        return SVG_ERR_IO;
    }
    if(size->width <= 0||size->height <= 0){
        return SVG_ERR_INVALID_ARG;
    }
    if(top_left == NULL){
        return SVG_ERR_NULL;
    }   
    if(context == NULL){
        return SVG_ERR_NULL;
    }
    int space = snprintf(NULL, 0, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"%s\"/>",
                        top_left->x, top_left->y, size->width, size->height, style);
    char *buffer = (char *)malloc(space + 1);
    snprintf(buffer, space,"<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"%s\"/>",
                        top_left->x, top_left->y, size->width, size->height, style);
    svg_return_t rect = context->write_fn(context->user, buffer);
    free(buffer);
    context->write_fn(context->user, ">\n");

    return rect;
}

svg_return_t svg_line(svg_context_ptr context,
                      const svg_point_t *start,
                      const svg_point_t *end,
                      const char* style){
    if(style == NULL){
        return SVG_ERR_IO;
    }
    if(start == NULL || end == NULL){
        return SVG_ERR_NULL;
    }
    if(context == NULL){
        return SVG_ERR_NULL;
    }
    int size = snprintf(NULL, 0, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"%s\"/>",
                        start->x, start->y, end->x, end->y, style);
    char *buffer = (char *)malloc(size + 1);
    snprintf(buffer, size, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"%s\"/>",
                        start->x, start->y, end->x, end->y, style);
    svg_return_t line = context->write_fn(context->user, buffer);
    free(buffer);
    context->write_fn(context->user, ">\n");

    return line;
}


svg_return_t svg_group_begin(svg_context_ptr context, 
                             const char* attrs){
    if(attrs == NULL){
        return SVG_ERR_IO;
    }
    int size = snprintf(NULL, 0, "<g %s>", attrs);
    char *buffer = (char *)malloc(size + 1);
    snprintf(buffer, size, "<g %s>", attrs);
    svg_return_t group_begin = context->write_fn(context->user, buffer);
    free(buffer);
    return group_begin;
}

svg_return_t svg_group_end(svg_context_ptr context){
    
    return context->write_fn(context->user, "</g>");
}
