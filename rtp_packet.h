//
// Created by hijiang on 2019/8/10.
//

#ifndef GB28181_HIKVISION_JAIN_SIP_RTP_PACKET_H
#define GB28181_HIKVISION_JAIN_SIP_RTP_PACKET_H
#include <memory>
#include <vector>
#include "../utils/stream_util.h"
//https://tools.ietf.org/html/rfc3550 @page 11
#define NAL_RTP_PACKET_SIZE 1400

//RTP Header Extension @page 19

class RtpHeaderExtention {
public:
    uint16_t profile;
    uint16_t length;
    std::unique_ptr<char[]> header_extention;
    virtual size_t size()
    {
        return 4 + length;
    }
    //todo
    virtual int encode(std::shared_ptr<StreamUtil> stream)
    {
        return 0;
    }

    virtual int decode(std::shared_ptr<StreamUtil> stream)
    {
        return 0;
    }
};

class RtpHeader {
public:
    RtpHeader();
    // little endian
    uint8_t csrc = 0;//4bit
    uint8_t extension  = 0;//1bit
    uint8_t padding = 0;//1bit
    uint8_t version = 2;//2bit

    uint8_t pt;
    uint8_t marker;

    uint16_t seqnum;
    uint32_t timestamp;
    uint32_t ssrc;
    std::vector<uint32_t> csrcs;
    std::shared_ptr<RtpHeaderExtention> rtp_header_extention = nullptr;//exit if extension bit is set
public:
    int encode(std::shared_ptr<StreamUtil> stream);
    int decode(std::shared_ptr<StreamUtil> stream);
    size_t size();
};

class  RtpPacket {
public:
    RtpPacket();
    virtual ~RtpPacket();
public:
    int encode(std::shared_ptr<StreamUtil> stream);
    int decode(std::shared_ptr<StreamUtil> stream);
    size_t size();
    uint16_t getSeqNum();
private:
    RtpHeader header_;
    char *payload_;
    size_t payload_len_;
    std::shared_ptr<StreamUtil> stream_;
    friend class RtpPacker;
};

#endif //GB28181_HIKVISION_JAIN_SIP_RTP_PACKET_H
