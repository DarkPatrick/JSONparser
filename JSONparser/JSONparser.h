#pragma once


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>


namespace JSONparser {
    /// <summary>Класс для выгрузки целиком JSON файла, его автоматического парсинга и дальнейшей работы с ним.</summary>
    class JsonParser {
        private:
        /// <summary>Структура дерева для хранения идентификаторов полей JSON файла и быстрого доступа к ним.</summary>
        struct TreeNode {
            std::string name;
            std::vector<TreeNode> children;
        };

        public:
        __declspec(dllexport) __stdcall
        JsonParser();
        /// <summary>Создание JSON объекта из указанного файла.</summary>
        __declspec(dllexport) __stdcall
        JsonParser(const char* file_name);
        __declspec(dllexport) __stdcall
        ~JsonParser();
        /// <summary>Парсинг JSON строки.</summary>
        /// <param name='str_name'>Имя переменной, содержащей JSON файла.</param>
        __declspec(dllexport)
        uint32_t __stdcall parseStringJSON(const char* str_name);
        /// <summary>Загрузка локального JSON файла и его парсинг.</summary>
        /// <param name='file_name'>Имя загружаемого JSON файла.</param>
        __declspec(dllexport)
        uint32_t __stdcall parseFileJSON(const char* file_name);
        /// <summary>Возвращает значение поля.</summary>
        /// <param name='path'>Полный путь в виде вектора из строковых имён полей</param>
        __declspec(dllexport)
        char* __stdcall getVal(char** path);

        private:
        uint32_t addWordToTree(const std::vector<std::string> &path);

        private:
        TreeNode name_tree;
    };
}
