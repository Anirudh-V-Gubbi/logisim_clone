#ifndef ENTITY_PARSER_H
#define ENTITY_PARSER_H

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#define INVALID_ATTRIBUTE Logger::GetInstance()->error(std::string(entityFileName + "[Line " + std::to_string(line) + "]: Invalid attribute").c_str())
#define INVALID_SYNTAX(x) Logger::GetInstance()->error(std::string(entityFileName + "[Invalid syntax]: x").c_str());

const std::string ENTITY_FOLDER = ".//resources/entities/";
const char* START_TAG = "START";
const char* END_TAG = "END";
int line = 1;

void findTag(const char* tag, std::string& str, int& start, int& end)
{
    int i;
    for(i = 0; str[i] != '\0'; i++) {
        if(str[i] == '#') {
            start = i;
            i++;
            int j = 0;
            while(tag[j] != '\0') {
                if(str[i] != tag[j]) break;
                i++;
                j++;
            }
            if(tag[j] == '\0'){
                end = i-1;
                return;
            }
        }
    }
    start = -1;
    end = -1;
}

bool GetAttribute(int& currentIndex, std::string& str, std::string& attribute)
{
    for(; str[currentIndex] != '\0'; currentIndex++) {
        if(str[currentIndex] == '\n') line++;
        
        if(str[currentIndex] == ' ' || str[currentIndex] == '\n' || str[currentIndex] == '\t') continue;
        
        if(str[currentIndex] == '\"'){
            currentIndex++;
            while(str[currentIndex] != '\"') {
                attribute += str[currentIndex];
                currentIndex++;
            }
            currentIndex++;
            if(!attribute.empty()) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    
    return false;
}

bool ParseEntity(std::string entityFileName, std::map<std::string, std::string>& attributeMap)
{
    // read the entity file contents
    // -----------------------------
    std::string fileName = ENTITY_FOLDER + entityFileName;
    std::ifstream file;
    file.open(fileName);
    
    if(!file) {
        Logger::GetInstance()->error(std::string("Could not open file " + fileName).c_str());
        return false;
    }
    
    std::stringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();
    file.close();
    
    // find the #START and #END tags in the file
    // -----------------------------------------
    int start1, start2, end1, end2;
    findTag(START_TAG, content, start1, start2);
    findTag(END_TAG, content, end1, end2);
    
    // enclosing tags not found
    // ------------------------
    if(start1 == -1) {
        INVALID_SYNTAX("No Start tag found");
        return false;
    }
    if(end1 == -1) {
        INVALID_SYNTAX("No End tag found");
        return false;
    }
    
    // map to store the attributes of the entities as key value pair
    // -------------------------------------------------------------
    std::string key, value;
    for(int i = start2 + 1; i < end1;) {
        key = "";
        value = "";
        
        // skip any whitespace / newline
        // -----------------------------
        while(content[i] == ' ' || content[i] == '\n' || content[i] == '\t') {
            if(content[i] == '\n') line++;
            i++;
        }
        if(i >= end1) break;
        
        // get the key part of the attribute
        // ---------------------------------
        if(!GetAttribute(i, content, key)) {
            INVALID_ATTRIBUTE;
            return false;
        }
        
        // match the '=' between key and value of the attribute
        // ----------------------------------------------------
        while(content[i] != '=') {
            if(content[i] == '\n') line++;
            
            if(content[i] == ' ' || content[i] == '\n' || content[i] == '\t') continue;
            
            INVALID_ATTRIBUTE;
            return false;
        }
        i++;
        
        // get the value part of the attribute
        // ---------------------------------
        if(!GetAttribute(i, content, value)) {
            INVALID_ATTRIBUTE;
            return false;
        }
        
        attributeMap[key] = value;
    }
    
    return true;
}
#endif
