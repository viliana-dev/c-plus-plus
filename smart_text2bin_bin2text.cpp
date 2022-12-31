#include <iostream>
#include <fstream> 
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <cstring>

using namespace std;


int text_to_bin(string input_path, string output_path) { 
    ifstream file(input_path);
    ofstream file_bin(output_path, ios::binary);
    unsigned int a, b, v;
    vector<int> vertex;
    unordered_map<unsigned int, vector<pair<unsigned int, unsigned char>>> edges;
    while(!file.eof()){ 
        file >> a >> b >> v;
        if (a > b) {
            int tmp = a;
            a = b;
            b = tmp;
        }
        edges[a].push_back(std::make_pair<unsigned int, unsigned char>(static_cast<unsigned int>(b), static_cast<unsigned char>(v)));
    }
    for (auto p : edges) {
        vertex.push_back(p.first);
    }
    unsigned int len = vertex.size();
    file_bin.write((char *)&len, 4);
    file_bin.write((char*)vertex.data(), 4*len);
    for (auto v : vertex) {
        const auto& cur = edges[v];
        unsigned int cur_len = cur.size();
        file_bin.write((char *)&cur_len, 4);
        for (auto dst : cur) {
            int vv = dst.first;
            char ww = dst.second;
            file_bin.write((char *)&vv, 4);
            file_bin.write((char *)&ww, 1);
        }
    }
    file.close(); 
    file_bin.close();
    return 0; 
}


int bin_to_text(string input_path, string output_path) { 
    ofstream file(output_path);
    ifstream file_bin(input_path, ios::binary);
    unsigned int a, b, N;
    unsigned char val;
    vector<unsigned int> vertex;
    file_bin.read((char *)&N, 4);
    for (int i = 0; i < N; i++) {
        file_bin.read((char *)&a, 4);
        vertex.push_back(a);
    }
    unsigned int count;
    for (auto v : vertex) {
        file_bin.read((char *)&count, 4);
        for (unsigned int i = 0; i < count; i++) {
            file_bin.read((char *)&b, 4);
            file_bin.read((char *)&val, 1);
            file << v << "\t" << b << "\t" << int((unsigned char) val) << endl;
        }
    }
    file.close(); 
    file_bin.close();
    return 0; 
}


int parse_args(int argc, char* argv[], string &input_path, string &output_path, bool  &text2bin) {
    if (argc == 6) {
        for(int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "-i") == 0) {
                input_path = argv[i + 1];
            } 
            if (strcmp(argv[i], "-o") == 0) {
                output_path = argv[i + 1];
            } 
            if (strcmp(argv[i], "-d") == 0) {
                text2bin = false;
            } 
        }
    }
    if (argc < 6) {
        cout << "Not Enought Command Line Arguments Passed" << endl;
        return -1;
    }
    if (argc > 6) {
        cout << "Too Much Command Line Arguments Passed" << endl;
        return -1;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    bool text2bin = true;
    string input_path;
    string output_path;
    if (parse_args(argc, argv, input_path, output_path, text2bin) == -1)
        return -1;
    if (text2bin) {
        text_to_bin(input_path, output_path);
    } else {
        bin_to_text(input_path, output_path);
    }
    return 0;
}

