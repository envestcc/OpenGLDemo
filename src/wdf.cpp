#include"wdf.h"
#include<iostream>
#include<fstream>

Wdf::Wdf(std::string filename):
    filename(filename)
{
    std::ifstream in;
    in.open(filename, std::ios::in|std::ios::binary);

    // read header
    in.read((char*)&header, sizeof(header));

    // read indexs
    in.seekg(header.offset);
    wasHeaderIndexs.resize(header.number);
    for(int i=0;i<header.number;i++)
    {
        wasHeaderIndexs[i] = new Index();
        in.read((char*)wasHeaderIndexs[i], sizeof(Index));
        uid2IndexMap[wasHeaderIndexs[i]->uid] = i;
    }
    in.close();
}

Wdf::~Wdf()
{
}

Was Wdf::LoadWas(uint32_t uuid)
{
    int id = uid2IndexMap[uuid];
}
