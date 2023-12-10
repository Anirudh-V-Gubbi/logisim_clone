#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>

enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

std::vector<std::pair<short int, short int>> stringToList(std::string str)
{
    std::vector<std::pair<short int, short int>> list;
    
    std::stringstream ss(str);
    std::string offset;
    while(std::getline(ss, offset, ')')) {
        std::stringstream ssoffset(offset);
        short int x, y;
        
        while(ssoffset.peek() == ' ' || ssoffset.peek() == '(' || ssoffset.peek() == ',') {
            ssoffset.ignore();
            }
        ssoffset >> x;
            
        while(ssoffset.peek() == ' ' || ssoffset.peek() == ',') {
            ssoffset.ignore();
        }
        ssoffset >> y;
        
        list.push_back(std::make_pair(x, y));
    }
    
    return list;
}

Direction stringToDirection(std::string str)
{
    if(str == "NORTH") {
        return Direction::NORTH;
    }
    if(str == "SOUTH") {
        return Direction::SOUTH;
    }
    if(str == "WEST") {
        return Direction::WEST;
    }
    
    return Direction::EAST;
}

std::string getResourceFullPath(std::string path)
{
    std::string fullPath(RESOURCE_PATH);
    fullPath += path;
    
    return fullPath;
}

std::string getShaderFullPath(std::string path)
{
    std::string fullPath(RESOURCE_PATH);
    fullPath += "shaders/";
    fullPath += path;

    return fullPath;
}

std::string getTextureFullPath(std::string path)
{
    std::string fullPath(RESOURCE_PATH);
    fullPath += "images/";
    fullPath += path;
    
    return fullPath;
}

#endif
