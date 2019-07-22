/* harppi.cpp v1.1
 * Updated: August 2, 2016
 * 
 * LICENSE: GPL v3
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 * 
 * UPDATES:
 * Changed the error messaging when calling the getd, geti, gets, or getb
 * functions to be more informative. Now it should let users know if they
 * are simply using the wrong get function or if they are calling a 
 * parameter that does not exist.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "harppi.hpp"

harppi::harppi(char *file) {
    harppi::readParams(file);
}

bool harppi::assignParams(std::string type, std::string key, std::string val) {
    if (type == "string") {
        harppi::strings.insert(std::pair<std::string, std::string>(key,val));
    } else if (type == "int") {
        harppi::ints.insert(std::pair<std::string, int>(key,atof(val.c_str())));
    } else if (type == "double") {
        harppi::doubles.insert(std::pair<std::string, double>(key,atof(val.c_str())));
    } else if (type == "bool") {
        bool b;
        std::istringstream(val) >> std::boolalpha >> b;
        harppi::bools.insert(std::pair<std::string, bool>(key, b));
    } else if (type == "vector<double>") {
        std::vector<double> vec;
        std::istringstream iss(val);
        std::string s;
        while (std::getline(iss, s, ',')) {
            vec.push_back(atof(s.c_str()));
        }
        harppi::dvectors.insert(std::pair<std::string, std::vector<double> >(key, vec));
    } else if (type == "vector<int>") {
        std::vector<int> vec;
        std::istringstream iss(val);
        std::string s;
        while (std::getline(iss, s, ',')) {
            vec.push_back(atof(s.c_str()));
        }
        harppi::ivectors.insert(std::pair<std::string, std::vector<int> >(key, vec));
    } else if (type == "vector<string>") {
        std::vector<std::string> vec;
        std::istringstream iss(val);
        std::string s;
        while (std::getline(iss, s, ',')) {
            vec.push_back(s);
        }
        harppi::svectors.insert(std::pair<std::string, std::vector<std::string> >(key, vec));
    } else if (type == "vector<bool>") {
        std::vector<bool> vec;
        std::istringstream iss(val);
        std::string s;
        while (std::getline(iss, s, ',')) {
            bool b;
            std::istringstream(s) >> std::boolalpha >> b;
            vec.push_back(b);
        }
        harppi::bvectors.insert(std::pair<std::string, std::vector<bool> >(key, vec));
    } else {
        std::cout << "WARNING: Unrecognized type specified in parameter file.\n";
        std::cout << "    Type " << type << " is not currently supported.\n";
        std::cout << "    Currently supported types are:\n";
        std::cout << "        string, int, double, bool, vector<string>\n";
        std::cout << "        vector<double>, and vector<int>" << std::endl;
        return false;
    }
    
    return true;
}

void harppi::readParams(char *file) {
    std::ifstream fin;
    std::string line, type, key, equal, val;
    bool check = true;
    
    fin.open(file, std::ios::in);
    while (std::getline(fin, line) && check) {
        std::istringstream iss(line);
        iss >> type >> key >> equal >> val;
        if (type != "#") {
            check = harppi::assignParams(type, key, val);
        } 
    }
    fin.close();
    
    if (!check) {
        std::stringstream message;
        message << "ERROR: All parameters have not been assigned" << std::endl;
        throw std::runtime_error(message.str());
    }
}

void harppi::print() {
    std::map<std::string, std::string>::iterator it_string = harppi::strings.begin();
    std::map<std::string, int>::iterator it_int = harppi::ints.begin();
    std::map<std::string, double>::iterator it_double = harppi::doubles.begin();
    std::map<std::string, bool>::iterator it_bool = harppi::bools.begin();
    std::map<std::string, std::vector<double> >::iterator it_dvectors = 
                                                        harppi::dvectors.begin();
    std::map<std::string, std::vector<int> >::iterator it_ivectors = 
                                                        harppi::ivectors.begin();
    std::map<std::string, std::vector<std::string> >::iterator it_svectors =
                                                        harppi::svectors.begin();
    std::map<std::string, std::vector<bool> >::iterator it_bvectors = 
                                                        harppi::bvectors.begin();
    
    for (it_string = harppi::strings.begin(); it_string != harppi::strings.end(); ++it_string)
        std::cout << "string " << it_string->first << " = " << it_string->second << std::endl;
    for (it_int = harppi::ints.begin(); it_int != harppi::ints.end(); ++it_int)
        std::cout << "int " << it_int->first << " = " << it_int->second << std::endl;
    for (it_double = harppi::doubles.begin(); it_double != harppi::doubles.end(); ++it_double)
        std::cout << "double " << it_double->first << " = " << it_double->second << std::endl;
    for (it_bool = harppi::bools.begin(); it_bool != harppi::bools.end(); ++it_bool)
        std::cout << "bool " << it_bool->first << " = " << std::boolalpha << it_bool->second << std::endl;
    for (it_dvectors = harppi::dvectors.begin(); it_dvectors != harppi::dvectors.end(); ++it_dvectors) {
        int numVals = harppi::dvectors[it_dvectors->first].size();
        std::cout << "vector<double> " << it_dvectors->first << " = ";
        for (int i = 0; i < numVals; ++i) {
            std::cout << harppi::dvectors[it_dvectors->first][i];
            if (i != numVals-1) std::cout << ",";
        }
        std::cout << std::endl;
    }
    for (it_ivectors = harppi::ivectors.begin(); it_ivectors != harppi::ivectors.end(); ++it_ivectors) {
        int numVals = harppi::ivectors[it_ivectors->first].size();
        std::cout << "vector<int> " << it_ivectors->first << " = ";
        for (int i = 0; i < numVals; ++i) {
            std::cout << harppi::ivectors[it_ivectors->first][i];
            if (i != numVals-1) std::cout << ",";
        }
        std::cout << std::endl;
    }
    for (it_svectors = harppi::svectors.begin(); it_svectors != harppi::svectors.end(); ++it_svectors) {
        int numVals = harppi::svectors[it_svectors->first].size();
        std::cout << "vector<string> " << it_svectors->first << " = ";
        for (int i = 0; i < numVals; ++i) {
            std::cout << harppi::svectors[it_svectors->first][i];
            if (i != numVals-1) std::cout << ",";
        }
        std::cout << std::endl;
    }
    for (it_bvectors = harppi::bvectors.begin(); it_bvectors != harppi::bvectors.end(); ++it_bvectors) {
        int numVals = harppi::bvectors[it_bvectors->first].size();
        std::cout << "vector<bool> " << it_bvectors->first << " = ";
        for (int i = 0; i < numVals; ++i) {
            std::cout << std::boolalpha << harppi::bvectors[it_bvectors->first][i];
            if (i != numVals-1) std::cout << ",";
        }
        std::cout << std::endl;
    }
}

void harppi::check_min(std::vector<typekey> neededParams) {
    int minNum = neededParams.size();
    int count = 0;
                                                        
    for (int i = 0; i < minNum; ++i) {
        if (neededParams[i].type == "int") {
            if (harppi::ints.count(neededParams[i].key) == 1) ++count;
            else std::cout << neededParams[i].type << " " << neededParams[i].key << " not found." << std::endl;
        } else if (neededParams[i].type == "double") {
            if (harppi::doubles.count(neededParams[i].key) == 1) ++count;
            else std::cout << neededParams[i].type << " " << neededParams[i].key << " not found." << std::endl;
        } else if (neededParams[i].type == "string") {
            if (harppi::strings.count(neededParams[i].key) == 1) ++count;
            else std::cout << neededParams[i].type << " " << neededParams[i].key << " not found." << std::endl;
        } else if (neededParams[i].type == "bool") {
            if (harppi::bools.count(neededParams[i].key) == 1) ++count;
            else std::cout << neededParams[i].type << " " << neededParams[i].key << " not found." << std::endl;
        } else if (neededParams[i].type == "vector<double>") {
            if (harppi::dvectors.count(neededParams[i].key) == 1) ++count;
            else std::cout << neededParams[i].type << " " << neededParams[i].key << " not found." << std::endl;
        } else if (neededParams[i].type == "vector<int>") {
            if (harppi::ivectors.count(neededParams[i].key) == 1) ++count;
            else std::cout << neededParams[i].type << " " << neededParams[i].key << " not found." << std::endl;
        } else if (neededParams[i].type == "vector<string>") {
            if (harppi::svectors.count(neededParams[i].key) == 1) ++count;
            else std::cout << neededParams[i].type << " " << neededParams[i].key << " not found." << std::endl;
        } else if (neededParams[i].type == "vector<bool>") {
            if (harppi::bvectors.count(neededParams[i].key) == 1) ++count;
            else std::cout << neededParams[i].type << " " << neededParams[i].key << " not found." << std::endl;
        }
    }
    
    if (count != minNum) {
        std::stringstream message;
        message << "ERROR: Minimum parameters not found." << std::endl;
        throw std::runtime_error(message.str());
    }
}

double harppi::getd(std::string key, int element) {
    if (harppi::ints.count(key) == 1) {
        return double(harppi::ints[key]);
    } else if (harppi::doubles.count(key) == 1) {
        return harppi::doubles[key];
    } else if (harppi::dvectors.count(key) == 1) {
        return harppi::dvectors[key][element];
    } else if (harppi::ivectors.count(key) == 1) {
        return double(harppi::ivectors[key][element]);
    } else if (harppi::strings.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a string not a numeric type.\n";
        message << "Use the gets() function instead of getd()." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::bools.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a boolean not a numeric type.\n";
        message << "Use the getb() function instead of getd()." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::svectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a vector<string> not a numeric type.\n";
        message << "Use the gets() function instead of getd()." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::bvectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a vector<bool> not a numeric type." << std::endl;
        throw std::runtime_error(message.str());
    } else {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " does not exist in the parameter file.\n";
        message << "Add " << key << " to your parameter file as a numeric type." << std::endl;
        throw std::runtime_error(message.str());
    }
}

int harppi::geti(std::string key, int element) {
    if (harppi::ints.count(key) == 1) {
        return harppi::ints[key];
    } else if (harppi::doubles.count(key) == 1) {
        return int(harppi::doubles[key]);
    } else if (harppi::dvectors.count(key) == 1) {
        return int(harppi::dvectors[key][element]);
    } else if (harppi::ivectors.count(key) == 1) {
        return harppi::ivectors[key][element];
    } else if (harppi::strings.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a string not a numeric type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::bools.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a boolean not a numeric type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::svectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a vector<string> not a numeric type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::bvectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a vector<bool> not a numeric type." << std::endl;
        throw std::runtime_error(message.str());
    } else {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " does not exist in the parameter file." << std::endl;
        throw std::runtime_error(message.str());
    }
}

bool harppi::getb(std::string key, int element) {
    if (harppi::bools.count(key) == 1) {
        return harppi::bools[key];
    } else if (harppi::bvectors.count(key) == 1) {
        return harppi::bvectors[key][element];
    } else if (harppi::ints.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is an int not a boolean type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::doubles.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is an double not a boolean type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::dvectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is an vector<double> not a boolean type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::ivectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is an vector<int> not a boolean type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::strings.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a string not a boolean type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::svectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a vector<string> not a boolean type." << std::endl;
        throw std::runtime_error(message.str());
    } else {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " does not exist in the parameter file." << std::endl;
        throw std::runtime_error(message.str());
    }
}

std::string harppi::gets(std::string key, int element) {
    if (harppi::strings.count(key) == 1) {
        return harppi::strings[key];
    } else if (harppi::svectors.count(key) == 1) {
        return harppi::svectors[key][element];
    } else if (harppi::ints.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is an int not a string type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::doubles.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a double not a string type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::dvectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a vector<double> not a string type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::ivectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a vector<int> not a string type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::bools.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a boolean not a string type." << std::endl;
        throw std::runtime_error(message.str());
    } else if (harppi::bvectors.count(key) == 1) {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " is a vector<bool> not a string type." << std::endl;
        throw std::runtime_error(message.str());
    } else {
        std::stringstream message;
        message << "ERROR: Parameter " << key << " does not exist in the parameter file." << std::endl;
        throw std::runtime_error(message.str());
    }
}

bool harppi::checkParam(std::string key) {
    if (harppi::strings.count(key) == 1) {
        return true;
    } else if (harppi::ints.count(key) == 1) {
        return true;
    } else if (harppi::doubles.count(key) == 1) { 
        return true;
    } else if (harppi::bools.count(key) == 1) {
        return true;
    } else if (harppi::dvectors.count(key) == 1) {
        return true;
    } else if (harppi::ivectors.count(key) == 1) {
        return true;
    } else if (harppi::svectors.count(key) == 1) {
        return true;
    } else if (harppi::bvectors.count(key) == 1) {
        return true;
    } else {
        return false;
    }
}
