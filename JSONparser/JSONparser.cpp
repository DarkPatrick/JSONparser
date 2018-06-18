#include "JSONparser.h"


namespace JSONparser {
    using namespace std;


    __stdcall JsonParser::JsonParser() {
    }


    __stdcall JsonParser::JsonParser(const char* file_name) {
        parseFileJSON(file_name);
    }


    __stdcall JsonParser::JsonParser(const wchar_t* file_name) {
        parseFileJSON(file_name);
    }


    __stdcall JsonParser::~JsonParser() {
    }


    uint32_t __stdcall JsonParser::parseStringJSON(const char* str_name) {
        name_tree.name = u8"";

        vector<uint32_t> unnamed_ids;
        vector<uint32_t> bracket;
        vector<string> path;
        string param_name = u8"", probably_name = u8"";
        uint32_t esc = 0, name_entering = 0;

        bracket.push_back(0);
        for (auto &ch : string(str_name)) {
            if ((ch == u8'\"') && (!esc)) {
                if (bracket.back() == 2) {
                    name_entering = 0;
                    path.push_back(param_name);
                    addWordToTree(path);
                    path.pop_back();
                    bracket.pop_back();
                } else {
                    name_entering = 1;
                    bracket.push_back(2);
                    param_name = u8"";
                }
                probably_name = u8"";
            } else {
                if (name_entering) {
                    param_name += ch;
                } else {
                    string acceptable_ch = u8"abcdefghijklmnopqrstuvwxyz0123456789+-.";

                    if (acceptable_ch.find(ch) != string::npos) {
                        probably_name += ch;
                    }
                }
            }
            if (!name_entering) {
                if (ch == u8'[') {
                    unnamed_ids.push_back(0);
                    param_name = "0";
                    path.push_back(param_name);
                    addWordToTree(path);
                    bracket.push_back(1);
                }
                if (ch == u8'{') {
                    bracket.push_back(0);
                }
                if ((ch == u8']') || (ch == u8'}') || (ch == u8',')) {
                    if (probably_name != u8"") {
                        path.push_back(probably_name);
                        addWordToTree(path);
                        path.pop_back();
                        probably_name = u8"";
                    }
                    path.pop_back();
                }
                if (ch == u8']') {
                    bracket.pop_back();
                    unnamed_ids.pop_back();
                }
                if (ch == u8'}') {
                    bracket.pop_back();
                }
                if (ch == u8',') {
                    if (bracket.back() == 1) {
                        unnamed_ids.back()++;
                        param_name = to_string(unnamed_ids.back());
                        path.push_back(param_name);
                        addWordToTree(path);
                    }
                }
                if (ch == u8':') {
                    path.push_back(param_name);
                }
            }
            if (ch == u8'\\') {
                esc = esc ? 0 : 1;
            } else {
                esc = 0;
            }
        }

        return 1;
    }


    uint32_t __stdcall JsonParser::parseFileJSON(const char* file_name) {
        ifstream h_file(file_name);

        if (h_file.is_open()) {
            string whole_file = u8"";
            string line;

            while (!h_file.eof()) {
                getline(h_file, line);
                whole_file.append(line);
            }
            h_file.close();

            return parseStringJSON(whole_file.c_str());
        } else {
            return 0;
        }
    }


    uint32_t __stdcall JsonParser::parseFileJSON(const wchar_t* file_name) {
        ifstream h_file(file_name);

        if (h_file.is_open()) {
            string whole_file = u8"";
            string line;

            while (!h_file.eof()) {
                getline(h_file, line);
                whole_file.append(line);
            }
            h_file.close();

            return parseStringJSON(whole_file.c_str());
        } else {
            return 0;
        }
    }


    char* __stdcall JsonParser::getVal(char** path) {
        TreeNode *p = &name_tree;
        uint32_t find = 0;
        vector<string> vec_path;

        while ((*path != nullptr) && (**path != '\0')) {
            vec_path.push_back(string(*path++));
        }

        for (auto word : vec_path) {
            find = 0;
            for (auto &branch : p->children) {
                if (word == branch.name) {
                    p = &branch;
                    find = 1;
                    break;
                }
            }

            if (!find) {
                break;
            }
        }

        return (find && p->children.size()) ? p->children.at(0).name.c_str() : u8"";
    }


    void __stdcall JsonParser::getTree(void* start_from, int rec_lvl) {
        TreeNode *p;

        if (start_from == nullptr) {
            p = &name_tree;
        } else {
            p = reinterpret_cast<TreeNode*>(start_from);
        }

        for (auto &branch : p->children) {
            if (branch.name.size()) {
                for (auto tab = 0; tab < rec_lvl; ++tab) {
                    std::cout << "    ";
                }
                std::cout << branch.name << std::endl;
                getTree(reinterpret_cast<void*>(&branch), rec_lvl + 1);
            }
        }

        if ((p->name.size()) && (p->children.size() == 0)) {
            std::cout << std::endl;
        }
    }


    uint32_t JsonParser::addWordToTree(const vector<string> &path) {
        TreeNode *p = &name_tree;
        uint32_t find = 0;

        for (auto word : path) {
            find = 0;
            for (auto &&branch : p->children) {
                if (word == branch.name) {
                    find = 1;
                    p = &branch;
                    break;
                }
            }

            if (!find) {
                TreeNode new_node;

                new_node.name = word;
                p->children.push_back(new_node);
            }
        }

        return 1 - find;
    }
}
