#include "headers/translator.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#include <cairo/cairo-pdf.h>
#include <freetype2/ft2build.h>
#include <freetype2/freetype/freetype.h>
#include <freetype2/freetype/ftsnames.h>

std::string text;
cairo_t *context;
int y=50;

void getLatin(char* source, char* destination){
    getSource(source);
    char* address=(char*)"support/SyneMono-Regular.ttf";
    writeToDestination(destination, address, false);
}

void getRunic(char* source, char* destination){
    getSource(source);
    char* address=(char*)"support/futhark.ttf";
    writeToDestination(destination, address, true);
}

void getSource(char* source){
    FILE* f;
    char* part= strtok(source,"\0");
    f= fopen(part,"r");
    fseek(f, 0, SEEK_END);
    int length= ftell(f);
    fseek(f, 0, SEEK_SET);
    char buf[length];
    fread(buf, length, sizeof(char), f);
    fclose(f);
    text=buf;
}

void writeToDestination(char* destination, char* fontAddress, bool toRunic){
    char* part= strtok(destination,"\0");
    cairo_surface_t *surface;

    int height=648;

    if(text.length()>35*55){
        int dif= text.length()- 35*55;
        dif/=55;
        dif*=15;
        height+=dif;
    }

    surface= cairo_pdf_surface_create(part, 504, height);
    context= cairo_create(surface);

    FT_Face face;
    FT_Library library;
    cairo_font_face_t *ff;
    FT_Init_FreeType(&library);
    FT_New_Face(library, fontAddress, 0, &face);
    ff=cairo_ft_font_face_create_for_ft_face(face, 0);
    cairo_set_font_face(context, ff);
    cairo_set_font_size(context, 8.f);
    cairo_set_source_rgb(context, 0, 0, 0);
    cairo_move_to(context, 50, y);

    if(toRunic)
        textCorrection();
    else
        runicCorrection();

    if(text.length()>55){
        while (text.length()>0){
            int limit= text.length()>=55?55:text.length();
            
            cairo_show_text(context, text.substr(0, limit).c_str());
            cairo_stroke(context);
            y+=15;
            cairo_move_to(context, 50, y);

            text.replace(0, limit, ""); 
        }
        cairo_show_page(context);
    }else{
        cairo_show_text(context, text.c_str());
        cairo_show_page(context);
    }
          
    cairo_surface_destroy(surface);
    cairo_destroy(context);

}

void textCorrection(){

    size_t pos;
    while((pos=text.find("th"))!=std::string::npos){
        text.replace(pos, 2, ":");
    }

    while((pos=text.find("ng"))!=std::string::npos){
        text.replace(pos, 2, ";");
    }

    while((pos=text.find("x"))!=std::string::npos){
        text.replace(pos, 1, "cs");
    }

    while((pos=text.find("q"))!=std::string::npos){
        text.replace(pos, 1, "cj");
    }

}

void runicCorrection(){

    size_t pos;
    while((pos=text.find(":"))!=std::string::npos){
        text.replace(pos, 1, "th");
    }

    while((pos=text.find(";"))!=std::string::npos){
        text.replace(pos, 1, "ng");
    }

    while((pos=text.find("cs"))!=std::string::npos){
        text.replace(pos, 2, "x");
    }

    while((pos=text.find("cj"))!=std::string::npos){
        text.replace(pos, 2, "q");
    }

}