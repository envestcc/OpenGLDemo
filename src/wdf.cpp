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
        uid2IndexMap[wasHeaderIndexs[i]->uid] = new WasIndex();
        uid2IndexMap[wasHeaderIndexs[i]->uid]->id = i;
    }
    in.close();
}

Wdf::~Wdf()
{
}

Was* Wdf::LoadWas(uint32_t uuid)
{
    if (uid2IndexMap.count(uuid) == 0)
    {
        std::cout << "" << std::endl;
        char msg[256];
        sprintf(msg, "Not find was %d in %s", uuid, filename.c_str());
        throw msg;
    }
    WasIndex* it = uid2IndexMap[uuid];
    if (it->was != nullptr)
        return it->was;
    it->was = new Was();
    // read was
    std::ifstream in;
    in.open(filename, std::ios::in|std::ios::binary);
    in.seekg(wasHeaderIndexs[it->id]->offset);
    in.read((char*)&(it->was->header), sizeof(Was::Header));
    in.read((char*)(it->was->palette), (512*2));
    in.close();

}
