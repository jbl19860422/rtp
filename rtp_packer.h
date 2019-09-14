//
// Created by hijiang on 2019/8/8.
//

#ifndef GB28181_HIKVISION_JAIN_SIP_RTP_PACKER_H
#define GB28181_HIKVISION_JAIN_SIP_RTP_PACKER_H
//c
#include <stdint.h>
//stl
#include <memory>
#include <atomic>
#include <vector>
#include <mutex>

#include "../utils/stream_util.h"
#include "rtp_packet.h"

class RtpPacker {
public:
    RtpPacker();
    virtual ~RtpPacker();
    std::vector<std::shared_ptr<RtpPacket>> pack(std::unique_ptr<char[]> data, size_t len, uint8_t pt, int ssrc, int64_t pts);
public:
    uint16_t getSequenceNum();
    std::atomic<bool> init_;
    std::mutex seqnum_mtx_;
    uint16_t rtp_seqnum_ = 0;
    std::unique_ptr<char[]> data_;
    size_t len_;
};

#endif //GB28181_HIKVISION_JAIN_SIP_RTP_PACKER_H
