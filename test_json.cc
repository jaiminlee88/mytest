#include<map>
#include<string>
#include<vector>
#include<fstream>
#include<iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

int main () {

    string name = "xiaoming";
    string gender = "boy";
    vector<string> hobby = {"足球","篮球","电影"};
    map<string, double> score = {{"数学",91.5},{"语文", 95.5},{"英语", 96}};

    string lover_name = "xiaohong";
    string love_gender = "girl";
    vector<string> lover_hobby = {"画画","跳舞","唱歌"};
    map<string, double> lover_score = {{"数学", 78.5},{"语文", 89},{"英语", 90}};

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    writer.StartObject();

    // writer.Key("name");
    // writer.String(name.c_str());
    // writer.Key("gender");
    // writer.String(gender.c_str());

    // writer.Key("hobby");
    // writer.StartArray();
    // for(auto &item : hobby){
    //     writer.String(item.c_str());
    // }
    // writer.EndArray();

    // writer.Key("socre");
    // writer.StartObject();
    // for(auto &item : score){
    //     writer.Key((item.first).c_str());
    //     writer.Double(item.second);
    // }
    // writer.EndObject();

    // writer.Key("lover");
    // writer.StartObject();

    // writer.Key("name");
    // writer.String(lover_name.c_str());
    // writer.Key("gender");
    // writer.String(love_gender.c_str());

    // writer.Key("hobby");
    // writer.StartArray();
    // for(auto &item : lover_hobby){
    //     writer.String((item.c_str()));
    // }
    // writer.EndArray();

    // writer.Key("score");
    // writer.StartObject();
    // for(auto &item : lover_score){
    //     writer.Key((item.first).c_str());
    //     writer.Double(item.second);
    // }
    // writer.EndObject();

    // writer.EndObject();

    std::map<string,string> tmp1={{"a","1"},{"b","2"},{"c","3"}};
    std::map<string,string> tmp2={{"d","4"},{"e","5"}};
    writer.Key("data");
    writer.StartArray();
    writer.StartObject();
    for(auto& it : tmp1) {
        writer.Key(it.first.c_str());
        writer.String(it.second.c_str());
    }
    writer.EndObject();
    writer.StartObject();
    for(auto& it : tmp2) {
        writer.Key(it.first.c_str());
        writer.String(it.second.c_str());
    }
    writer.EndObject();
    writer.EndArray();

    writer.EndObject();

    //将生成的json数据写入json1.txt文件中
    std::string outFileName = "json1.txt";
    std::ofstream outfile(outFileName,std::ios::trunc);
    outfile << s.GetString();
    outfile.close();

    std::cout << s.GetString() << std::endl;

}

// {
//     "name":"xiaoming",
//     "gender":"boy",
//     "hobby":["足球","篮球","电影"],
//     "socre":{"数学":91.5,"英语":96.0,"语文":95.5},
//     "lover":{
//         "name":"xiaohong",
//         "gender":"girl",
//         "hobby":["画画","跳舞","唱歌"],
//         "score":{"数学":78.5,"英语":90.0,"语文":89.0}
//         }
// }