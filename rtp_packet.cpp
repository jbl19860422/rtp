//
// Created by hijiang on 2019/8/10.
//
#include "rtp_packet.h"
RtpHeader::RtpHeader()
{

}

size_t RtpHeader::size()
{
    size_t sz = 12;
    sz += csrcs.size()*4;
    if(extension)
    {
        if(rtp_header_extention)
        {
            sz += rtp_header_extention->size();
        }
    }
    return sz;
}

int RtpHeader::encode(std::shared_ptr <StreamUtil> stream)
{
//    assert(csrc ==  csrcs.size());
//    assert(extension?rtp_header_extention!=nullptr:rtp_header_extention== nullptr);

    uint8_t tmpv1 = 0;
    tmpv1 |= (csrc & 0x0F) << 0;
    tmpv1 |= (extension & 0x01) << 4;
    tmpv1 |= (padding & 0x01) << 5;
    tmpv1 |= (version & 0x03) << 6;
    stream->write_1bytes(tmpv1);

    uint8_t tmpv2 = 0;
    tmpv2 |= (marker & 0x01) << 0;
    tmpv2 |= (pt & 0x7F) << 1;
    stream->write_1bytes(tmpv2);

    stream->write_2bytes(seqnum);
    stream->write_4bytes(timestamp);
    stream->write_4bytes(ssrc);

    for(const auto & c : csrcs)
    {
        stream->write_4bytes(c);
    }

    if(rtp_header_extention)
    {
        rtp_header_extention->encode(stream);
    }
    return 0;
}

int RtpHeader::decode(std::shared_ptr<StreamUtil> stream)
{
    if(!stream->require(1))
    {
        return -1;
    }

    uint8_t tmpv1 = stream->read_1bytes();
    csrc = (tmpv1 >> 0) & 0x0F;
    extension = (tmpv1 >> 4) & 0x01;
    padding = (tmpv1 >> 5) & 0x01;
    version = (tmpv1 >> 6) & 0x03;
    if(!stream->require(1))
    {
        return -2;
    }
    uint8_t tmpv2 = stream->read_1bytes();
    marker = (tmpv2 >> 0) & 0x01;
    pt = (tmpv2 >> 1) & 0x7F;
    //seqnum
    if(!stream->require(2))
    {
        return -3;
    }
    seqnum = stream->read_2bytes();
    //timestamp
    if(!stream->require(4))
    {
        return -3;
    }
    timestamp = stream->read_4bytes();
    //ssrc
    if(!stream->require(4))
    {
        return -3;
    }
    ssrc = stream->read_4bytes();

    for(int i = 0; i < csrc; i++)
    {
        if(!stream->require(4))
        {
            auto c = stream->read_4bytes();
            csrcs.push_back(c);
        }
    }

    if(extension)
    {
        rtp_header_extention = std::make_shared<RtpHeaderExtention>();
        if(0 != rtp_header_extention->decode(stream))
        {
            return -4;
        }
    }
}

RtpPacket::RtpPacket()
{

}

RtpPacket::~RtpPacket()
{

}

size_t RtpPacket::size()
{
    size_t sz = 0;
    sz += header_.size();
    sz += payload_len_;
    return sz;
}

uint16_t RtpPacket::getSeqNum()
{
    return header_.seqnum;
}

int RtpPacket::encode(std::shared_ptr <StreamUtil> stream)
{
    header_.encode(stream);
    stream->write_bytes(payload_, payload_len_);
    return 0;
}

int RtpPacket::decode(std::shared_ptr <StreamUtil> stream)
{
    if(0 != header_.decode(stream))
    {
        return -1;
    }

    payload_len_ = stream->left();
    payload_ = stream->data() + stream->pos();
    stream_ = stream;//防止被释放
    return 0;
}