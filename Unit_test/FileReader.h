//
//  FileReader.h
//  Reverb
//
//  Created by XinquanZhou on 6/3/15.
//
//

#ifndef __Reverb__FileReader__
#define __Reverb__FileReader__

#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>

class FileReader {
    std::string _filename;
    std::string _emptystr;
    size_t _numrow;
    
    FileReader();
    
public:
    FileReader(const char * filename):_numrow(0){
        try {
            _filename = filename;
            std::ifstream file(_filename);
            std::string line;
            while (std::getline(file,line)) {
                raw_content.push_back(line);
                _numrow ++;
            }
        } catch (std::exception &e) {
            abort();
        }
       
    }
    
    ~FileReader(){}
    
    typedef std::vector<std::vector<float>> Content;
    typedef std::vector<std::string> RawContent;
    RawContent raw_content;
    Content content;
    void split(char delim){
        for (int i = 0 ; i < _numrow; i++) {
            std::vector<float> line;
            std::stringstream ss(raw_content[i]);
            std::string item;
            while (std::getline(ss, item, delim)) {
                
                if (item.empty()) {
                    continue;
                }
                line.push_back(std::stof(item));
            }
            content.push_back(line);
        }

    }
};

#endif /* defined(__Reverb__FileReader__) */
