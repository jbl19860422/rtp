//
// Created by hijiang on 2019/8/8.
//
#include <assert.h>
#include "rtp_packer.h"



RtpPacker::RtpPacker()
{

}

uint16_t RtpPacker::getSequenceNum()
{
    if (++rtp_seqnum_ >= 65535)
    {
        rtp_seqnum_ = 0;
    }
    return rtp_seqnum_;
}

std::vector<std::shared_ptr<RtpPacket>> RtpPacker::pack(std::unique_ptr<char[]> data, size_t len, uint8_t pt, int ssrc, int64_t pts)
{
    assert(len > 0);
    data_ = std::move(data);//hold the data
    len_ = len;
    std::vector<std::shared_ptr<RtpPacket>> pkts;
    size_t left_len = len;
    int curr_pos = 0;
    while(left_len > 0)
    {
        int consume_count = 0;
        std::shared_ptr<RtpPacket> pkt;
        if(left_len < NAL_RTP_PACKET_SIZE)
        {
            pkt = std::make_shared<RtpPacket>();
            pkt->header_.pt = pt;
            pkt->header_.timestamp = pts;
            pkt->header_.marker = 1;
            pkt->header_.seqnum = getSequenceNum();
            pkt->header_.ssrc = ssrc;
            pkt->payload_ = data_.get() + curr_pos;
            pkt->payload_len_ = left_len;
            consume_count = left_len;
        }
        else
        {
            pkt = std::make_shared<RtpPacket>();
            pkt->header_.pt = pt;
            pkt->header_.timestamp = pts;
            pkt->header_.marker = 0;
            pkt->header_.seqnum = getSequenceNum();
            pkt->header_.ssrc = ssrc;
            pkt->payload_ = data_.get() + curr_pos;
            pkt->payload_len_ = NAL_RTP_PACKET_SIZE;
            consume_count = NAL_RTP_PACKET_SIZE;
        }
        pkts.push_back(pkt);
        curr_pos += consume_count;
        left_len -= consume_count;
    }
    return pkts;
}

RtpPacker::~RtpPacker()
{

}