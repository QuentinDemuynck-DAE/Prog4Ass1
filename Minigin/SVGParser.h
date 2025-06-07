#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "pugixml.hpp"

namespace dae
{
    struct Rectangle
    {
        float x, y, width, height;
    };

    //! ONLY WORKS FOR INKSCAPE SVGS
    static std::vector<Rectangle> parseRectsInLayer(
        const std::string& svgPath,
        const std::string& layerName)
    {
        pugi::xml_document doc;

        std::string fullPath = std::string("../Data/") + svgPath;
        if (!doc.load_file(fullPath.c_str()))
        {
            std::cout << "Failed to load SVG" << std::endl;
            return {};
        }

        std::vector<Rectangle> rects;

        for (auto g : doc.child("svg").children("g"))
        {
            if (std::string(g.attribute("inkscape:label").as_string()) == layerName)
            {
                // Within that layer, grab all <rect> nodes
                for (auto r : g.children("rect"))
                {
                    Rectangle rc;
                    rc.x = r.attribute("x").as_float();
                    rc.y = r.attribute("y").as_float();
                    rc.width = r.attribute("width").as_float();
                    rc.height = r.attribute("height").as_float();
                    rects.push_back(rc);
                }
                break;
            }
        }
        return rects;
    }
}
