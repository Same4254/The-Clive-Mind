#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/istreamwrapper.h"

#include <iostream>
#include <fstream>
#include <istream>

int main() {
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    double* s = (double*) calloc(12690, sizeof(double));

    rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("Math", "50", allocator);
	object.AddMember("Science", "70", allocator);
	object.AddMember("English", "50", allocator);
	object.AddMember("Social Science", "70", allocator);

    rapidjson::Value array(rapidjson::kArrayType);
    for(int i = 0; i < 12690; i++) {
        array.PushBack(s[i], allocator);
    }

    object.AddMember("state", array, allocator);
	document.AddMember("Marks", object, allocator);

    std::ofstream ofs("output.json");
    rapidjson::OStreamWrapper osw(ofs);
    
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);

    document.Accept(writer);

    ofs.close();

    std::ifstream ifs("output.json");
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document d;
    d.ParseStream(isw);

    for (rapidjson::Value::ConstMemberIterator itr2 = d.MemberBegin(); itr2 != d.MemberEnd(); ++itr2) {
        const rapidjson::Value& state = itr2->value["state"];

        int i = 0; 
        for (auto& v : state.GetArray()) {
            s[i] = v.GetDouble();
            i++;
        }
    }
}