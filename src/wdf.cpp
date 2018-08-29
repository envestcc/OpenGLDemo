#include"wdf.h"
#include<iostream>
#include<fstream>
#include<cstring>
#include<algorithm>

uint32_t RGB565to888(uint16_t color, uint8_t alpha)
{
    unsigned int r = (color >> 11) & 0x1f;
    unsigned int g = (color >> 5) & 0x3f;
    unsigned int b = (color) & 0x1f;
    uint32_t R, G, B, A;
    A = alpha;
    R = (r << 3) | (r >> 2);
    G = (g << 2) | (g >> 4);
    B = (b << 3) | (b >> 2);
    return   A << 24 | (B << 16) | (G << 8) | R;
}

// 16bit 565Type Alpha mix
uint16_t Alpha565(uint16_t Src, uint16_t Des, uint8_t Alpha)
{
    uint16_t Result;
    // after mix = ( ( A-B ) * Alpha ) >> 5 + B
    // after mix = ( A * Alpha + B * ( 32-Alpha ) ) / 32

    unsigned short R_Src, G_Src, B_Src;
    R_Src = G_Src = B_Src = 0;

    R_Src = Src & 0xF800;
    G_Src = Src & 0x07E0;
    B_Src = Src & 0x001F;

    R_Src = R_Src >> 11;
    G_Src = G_Src >> 5;

    unsigned short R_Des, G_Des, B_Des;
    R_Des = G_Des = B_Des = 0;

    R_Des = Des & 0xF800;
    G_Des = Des & 0x07E0;
    B_Des = Des & 0x001F;

    R_Des = R_Des >> 11;
    G_Des = G_Des >> 5;

    unsigned short R_Res, G_Res, B_Res;
    R_Res = G_Res = B_Res = 0;

    R_Res = (((R_Src - R_Des)*Alpha) >> 5) + R_Des;
    G_Res = (((G_Src - G_Des)*Alpha) >> 5) + G_Des;
    B_Res = (((B_Src - B_Des)*Alpha) >> 5) + B_Des;

    R_Res = R_Res << 11;
    G_Res = G_Res << 5;

    Result = R_Res | G_Res | B_Res;
    return Result;
}

void DataHandler(uint8_t *pData, uint32_t* pBmpStart, int pixelOffset, int pixelLen, int y, bool& copyline, uint16_t* m_Palette16, uint32_t* m_Palette32)
{
    uint32_t Pixels = pixelOffset;
    uint32_t PixelLen = pixelLen;
    uint16_t AlphaPixel = 0;

    while (pData && *pData != 0) // {00000000} 表示像素行结束，如有剩余像素用透明色代替
    {
        uint8_t style = 0;
        uint8_t Level = 0; // Alpha层数
        uint8_t Repeat = 0; // 重复次数
        style = (*pData & 0xc0) >> 6;  // 取字节的前两个比特
        switch (style)
        {
        case 0: // {00******}
            if (copyline&&y == 1)
            {
                copyline = false;
            }
            if (*pData & 0x20) // {001*****} 表示带有Alpha通道的单个像素
            {
                // {001 +5bit Alpha}+{1Byte Index}, 表示带有Alpha通道的单个像素。
                // {001 +0~31层Alpha通道}+{1~255个调色板引索}
                Level = (*pData) & 0x1f; // 0x1f=(11111) 获得Alpha通道的值
                pData++; // 下一个字节
                if (Pixels < PixelLen)
                {
                    AlphaPixel = Alpha565(m_Palette16[(uint8_t)(*pData)], 0, Level);  // 混合
                    *pBmpStart++ = RGB565to888(AlphaPixel, Level * 8);
                    Pixels++;
                    pData++;
                }
            }
            else // {000*****} 表示重复n次带有Alpha通道的像素
            {
                // {000 +5bit Times}+{1Byte Alpha}+{1Byte Index}, 表示重复n次带有Alpha通道的像素。
                // {000 +重复1~31次}+{0~255层Alpha通道}+{1~255个调色板引索}
                // 注: 这里的{00000000} 保留给像素行结束使用，所以只可以重复1~31次。
                Repeat = (*pData) & 0x1f; // 获得重复的次数
                pData++;
                Level = *pData; // 获得Alpha通道值
                pData++;
                for (int i = 1; i <= Repeat; i++)
                {
                    if (Pixels < PixelLen)
                    {
                        AlphaPixel = Alpha565(m_Palette16[(uint8_t)*pData], 0, Level); // ???
                        *pBmpStart++ = RGB565to888(AlphaPixel, Level * 8);
                        Pixels++;
                    }
                }
                pData++;
            }
            break;
        case 1:
            // {01******} 表示不带Alpha通道不重复的n个像素组成的数据段
            // {01  +6bit Times}+{nByte Datas},表示不带Alpha通道不重复的n个像素组成的数据段。
            // {01  +1~63个长度}+{n个字节的数据},{01000000}保留。
            if (copyline&&y == 1)
            {
                copyline = false;
            }
            Repeat = (*pData) & 0x3f; // 获得数据组中的长度
            pData++;
            for (int i = 1; i <= Repeat; i++)
            {
                if (Pixels < PixelLen)
                {
                    *pBmpStart++ = m_Palette32[(uint8_t)*pData];
                    Pixels++;
                    pData++;
                }
            }
            break;
        case 2:
            // {10******} 表示重复n次像素
            // {10  +6bit Times}+{1Byte Index}, 表示重复n次像素。
            // {10  +重复1~63次}+{0~255个调色板引索},{10000000}保留。
            if (copyline&&y == 1)
            {
                copyline = false;
            }
            Repeat = (*pData) & 0x3f; // 获得重复的次数
            pData++;
            for (int i = 1; i <= Repeat; i++)
            {
                if (Pixels < PixelLen)
                {
                    *pBmpStart++ = m_Palette32[(uint8_t)*pData];
                    Pixels++;
                }
            }
            pData++;
            break;
        case 3:
            // {11******} 表示跳过n个像素，跳过的像素用透明色代替
            // {11  +6bit Times}, 表示跳过n个像素，跳过的像素用透明色代替。
            // {11  +跳过1~63个像素},{11000000}保留。
            Repeat = (*pData) & 0x3f; // 获得重复次数
            for (int i = 1; i <= Repeat; i++)
            {
                if (Pixels < PixelLen)
                {
                    pBmpStart++;
                    Pixels++;
                }
            }
            pData++;
            break;
        default: // 一般不存在这种情况
            std::cerr << "Error!" << std::endl;
            break;
        }
    }
    if (*pData == 0 && PixelLen > Pixels)
    {
        uint32_t Repeat = 0;
        Repeat = PixelLen - Pixels;
        for (uint32_t i = 0; i < Repeat; i++)
        {
            if (Pixels < PixelLen)
            {
                pBmpStart++;
                Pixels++;
            }
        }
    }
}

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
        wasHeaderIndexs[i] = *new Index();
        in.read((char*)&wasHeaderIndexs[i], sizeof(Index));
        uid2IndexMap[wasHeaderIndexs[i].uid] = *(new WasIndex());
        uid2IndexMap[wasHeaderIndexs[i].uid].id = i;
    }
    in.close();
}

Wdf::~Wdf()
{
    // for(int i=0;i<wasHeaderIndexs.size();i++)
    //     delete &(wasHeaderIndexs[i]);
    wasHeaderIndexs.clear();
    // std::map<uint32_t, WasIndex>::iterator iter;
    // for(iter = uid2IndexMap.begin();iter != uid2IndexMap.end(); iter++)
    // {
    //     delete &(iter->second);
    // }
    uid2IndexMap.clear();
        
}

Was* Wdf::LoadWas(uint32_t uuid)
{
    if (uid2IndexMap.count(uuid) == 0)
    {
        char msg[256];
        sprintf(msg, "Not find was %d in %s", uuid, filename.c_str());
        throw msg;
    }
    WasIndex* it = &(uid2IndexMap[uuid]);
    if (it->was != nullptr)
        return it->was;
    Was *was = new Was();
    it->was = was;
    // read was header
    std::ifstream in;
    in.open(filename, std::ios::in|std::ios::binary);
    in.seekg(wasHeaderIndexs[it->id].offset);
    in.read((char*)&(was->header), sizeof(Was::Header));
    if (was->header.magic != 0x5053)
    {
        char msg[256];
        sprintf(msg, "was file header magic error:%d, uuid=%d, wdf=%s",was->header.magic, uuid, filename.c_str());
        throw msg;
    }
    // read palette
    uint32_t paletteStart = wasHeaderIndexs[it->id].offset + was->header.size + 4;
    in.seekg(paletteStart);
    memset(was->palette16, 0, sizeof(was->palette16));
    in.read((char*)(was->palette16), sizeof(was->palette16));
    for(int i=0;i<256;i++)
    {
        was->palette[i] = RGB565to888(was->palette16[i], 0xff);
    }
    // read frameindex
    int framecnt = was->header.imgDirCnt * was->header.imgFrameCnt;
    was->frameIndexs.resize(framecnt);
    in.read((char*)was->frameIndexs.data(), framecnt*4);
    // read frameheader
    was->frameHeaders.resize(framecnt);
    was->frameLines.resize(framecnt);
    for(int i=0;i<framecnt;i++)
    {
        in.seekg(paletteStart + was->frameIndexs[i]);
        in.read((char*)(&was->frameHeaders[i]), sizeof(Was::FrameHeader));
        was->frameLines[i].resize(was->frameHeaders[i].height);
        in.read((char*)(was->frameLines[i].data()), sizeof(uint32_t)*(was->frameHeaders[i].height));
    }
    // read frame
    uint32_t max = 500;
    uint8_t* lineData = new uint8_t[max];
    was->frames.resize(framecnt);
    for(int i=0;i<framecnt;i++)
    {
        int width = was->frameHeaders[i].width, height=was->frameHeaders[i].height;
        was->frames[i].resize(width*height);
        uint32_t max2 = *max_element(was->frameLines[i].begin(), was->frameLines[i].end());
        uint32_t min2 = *min_element(was->frameLines[i].begin(), was->frameLines[i].end());

        if (max2-min2 > max)
        {
            delete lineData;
            max = (max2-min2) + 500;
            lineData = new uint8_t[max];
        }
        bool copyLine = true;
        for(int j=0;j<height;j++)
        {
            in.seekg(paletteStart + was->frameIndexs[i] + was->frameLines[i][j]);
            in.read((char*)lineData, max);
            DataHandler(lineData, was->frames[i].data()+width*j, 0, width, j, copyLine, was->palette16, was->palette);
        }
        if (copyLine)
        {
            for(int j=0;j+1<height;j += 2)
            {
                uint32_t* pDst = &(was->frames[i].data()[(j + 1)*width]);
                uint32_t* pSrc = &(was->frames[i].data()[j*width]);
                memcpy((uint8_t*)pDst, (uint8_t*)pSrc, width * 4);
            }
        }
        bool IsBlank = true;
        for (uint32_t pix = 0; pix < width*height; pix++)
        {
            if (was->frames[i].data()[pix] != 0)
            {
                // std::cerr <<frame.src[xx] << std::endl;
                IsBlank = false;
                break;
            }
        }
    }
    delete lineData;
    in.close();
    return was;
}

Was::~Was()
{
    frameIndexs.clear();
    // for(int i=0;i<frameHeaders.size();i++)
    //     delete &frameHeaders[i];
    frameHeaders.clear();
    // for(int i=0;i<frameLines.size();i++)
    //     frameLines[i].clear();
    frameLines.clear();
    // for(int i=0;i<frames.size();i++)
    //     frames[i].clear();
    frames.clear();
}