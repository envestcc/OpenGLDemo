/****************************************************************

WDF File

Author: cc

****************************************************************/


#ifndef WDF_H
#define WDF_H

#include<string>
#include<vector>
#include<map>


class Was 
{
public:
    struct Header
    {
        // SP(0x53,0x50)
        uint16_t magic;
        // size of washeader
        uint16_t size;
        // direction count of img
        uint16_t imgDirCnt;
        // frame count of img
        uint16_t imgFrameCnt;
        // img width
        uint16_t imgWidth;
        // img height
        uint16_t imgHeight;
        // position of the key point
        uint16_t imgKpX;
        uint16_t imgKpY;
    };

    struct FrameHeader
    {
        int32_t key_x;
        int32_t key_y;
        int32_t width;
        int32_t height;
    };

    Header header;
    uint16_t palette16[256];
    uint32_t palette[256];
    std::vector<uint32_t> frameIndexs;
    std::vector<FrameHeader> frameHeaders;
    std::vector<std::vector<uint32_t>> frameLines;
    std::vector<std::vector<uint32_t>> frames;

};

class Wdf
{

public:

    struct Header
    {
        // wdf file magic, should be PFDW(0x50,0x46,0x44,0x57)
        uint32_t magic;
        // number of file contained
        uint32_t number;
        // files contained index table position from start of the file
        uint32_t offset;
    };

    struct Index
    {
        // was file uuid
        uint32_t uid;
        // was file position from start of the file
        uint32_t offset;
        // size of the was file
        uint32_t size;
        // TBC...
        uint32_t space;
    };

    struct WasIndex
    {
        // was index
        int id;
        // was pointer
        Was *was;

        WasIndex():id(-1),was(nullptr){}
    };

    Wdf(std::string filename);
    ~Wdf();
    Was* LoadWas(uint32_t uuid);

    std::string filename;
    Header header;
    std::vector<Index*> wasHeaderIndexs;
    std::map<uint32_t, WasIndex*> uid2IndexMap;

};


#endif