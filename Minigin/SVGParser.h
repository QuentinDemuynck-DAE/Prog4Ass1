#pragma once

#include <fstream>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include "Globals.h"

namespace dae
{
    typedef glm::vec2 Vec2;

    struct TileInfo {
        bool bottomBun = false;
        bool cheese = false;
        bool floor = false;
        bool ladder = false;
        bool ladderExit = false;
        bool meat = false;
        bool plate = false;
        bool salad = false;
        bool topBun = false;
        bool wall = false;
        bool tomato = false;
        bool playerSpawn = false;
        bool sausage = false;
        bool pickle = false;
        bool egg = false;
        glm::vec2 position;
    };
    

    struct Map {
        int columns = 0;
        int rows = 0;
        Vec2 tileSize = Vec2(0.0f);        
        std::vector<TileInfo> tiles;
        Vec2 margin;
    };


    // Used help for this
    inline Map loadMapFromTiledJSON(const std::string& filename, const Vec2& offset = Vec2(0.0f)) {

    	std::string actualPath = "../Data/" + filename;
        std::ifstream in(actualPath);
        if (!in.is_open()) {
            throw std::runtime_error("Failed to open map file: " + actualPath);
        }

        nlohmann::json json;
        try {
            in >> json;
        }
        catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error(std::string("JSON parse error in file ") + actualPath + ": " + e.what());
        }

        // Validate required fields
        for (auto field : { "imagewidth", "imageheight", "tilewidth", "margin", "spacing", "tilecount", "columns" }) {
            if (!json.contains(field)) {
                throw std::runtime_error(std::string("Missing required JSON field: ") + field + " in file " + actualPath);
            }
        }

        int tileWidth = json["tilewidth"];
        int margin = json["margin"];
        int spacing = json["spacing"];
        int tileCount = json["tilecount"];
        int columns = json["columns"];
        int rows = tileCount / columns;
        int tileHeight = json["tileheight"];


        Vec2 totalOffset = offset + Vec2(static_cast<float>(margin), static_cast<float>(margin));
        Vec2 tileStep = Vec2(static_cast<float>(tileWidth + spacing), static_cast<float>(tileHeight + spacing));

        Map map;
        map.margin.x = float(margin);
        map.margin.y = float(margin);

        map.columns = columns;
        map.rows = rows;
        map.tileSize = Vec2(static_cast<float>(tileWidth), static_cast<float>(tileHeight));
        map.tiles.assign(columns * rows, TileInfo{});

        std::unordered_map<int, TileInfo> defs;

        for (const auto& tile : json["tiles"]) {
            int localId = tile.at("id");
            TileInfo info{};
            if (tile.contains("properties")) {
                for (const auto& p : tile["properties"]) {
                    const std::string name = p.at("name");
                    bool val = p.at("value").get<bool>();
                    if (!val) continue;
                    if (name == "bottomBun")  info.bottomBun = true;
                    else if (name == "cheese")     info.cheese = true;
                    else if (name == "floor")      info.floor = true;
                    else if (name == "ladder")     info.ladder = true;
                    else if (name == "ladderExit") info.ladderExit = true;
                    else if (name == "meat")       info.meat = true;
                    else if (name == "plate")      info.plate = true;
                    else if (name == "salad")      info.salad = true;
                    else if (name == "topBun")     info.topBun = true;
                    else if (name == "wall")       info.wall = true;
                    else if (name == "tomato")     info.tomato = true;
                    else if (name == "playerSpawn")info.playerSpawn = true;
                    else if (name == "sausage")    info.sausage = true;
                    else if (name == "pickle")     info.pickle = true;
                    else if (name == "egg")        info.egg = true;
                }
            }
            defs.emplace(localId, info);
        }


        // Compute spawn positions
        for (int row = 0; row < rows; ++row)              
        {
            for (int col = 0; col < columns; ++col)        
            {
                int index = row * columns + col;
                if (index >= 0) {
                    map.tiles[index] = defs[index];
                }
                TileInfo& t = map.tiles[row * columns + col];

                Vec2 worldPos = totalOffset
                    + Vec2{ float(col), float(row) } *tileStep;
                t.position = worldPos;
            }
        }

        return map;
    }




	//! ONLY WORKS FOR INKSCAPE SVGS don't use it anymore realised I can use a tile map
    //static std::vector<Rectangle> parseRectsInLayer(
    //    const std::string& svgPath,
    //    const std::string& layerName)
    //{
    //    pugi::xml_document doc;
    //
    //    std::string fullPath = std::string("../Data/") + svgPath;
    //    if (!doc.load_file(fullPath.c_str()))
    //    {
    //        std::cout << "Failed to load SVG" << std::endl;
    //        return {};
    //    }
    //
    //    std::vector<Rectangle> rects;
    //
    //    for (auto g : doc.child("svg").children("g"))
    //    {
    //        if (std::string(g.attribute("inkscape:label").as_string()) == layerName)
    //        {
    //            // Within that layer, grab all <rect> nodes
    //            for (auto r : g.children("rect"))
    //            {
    //                Rectangle rc;
    //                rc.x = r.attribute("x").as_float();
    //                rc.y = r.attribute("y").as_float();
    //                rc.columns = r.attribute("columns").as_float();
    //                rc.rows = r.attribute("rows").as_float();
    //                rects.push_back(rc);
    //            }
    //            break;
    //        }
    //    }
    //    return rects;
    //}
}
