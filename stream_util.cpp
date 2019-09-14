//
// Created by hijiang on 2019/8/4.
//
#include <assert.h>
#include "stream_util.h"

using namespace std;
StreamUtil::StreamUtil()
{
    p = bytes = NULL;
    nb_bytes = 0;

    // TODO: support both little and big endian.
//    assert(srs_is_little_endian());
}

StreamUtil::~StreamUtil()
{
}

int StreamUtil::initialize(char* b, int nb)
{
    int ret = 0;

    if (!b) {
        ret = -1;
        return ret;
    }

    if (nb <= 0) {
        ret = -2;
        return ret;
    }

    nb_bytes = nb;
    p = bytes = b;
    return ret;
}

char* StreamUtil::data()
{
    return bytes;
}

int StreamUtil::size()
{
    return nb_bytes;
}

int StreamUtil::pos()
{
    return (int)(p - bytes);
}

int StreamUtil::left()
{
    return nb_bytes - (int)(p-bytes);
}

bool StreamUtil::empty()
{
    return !bytes || (p >= bytes + nb_bytes);
}

bool StreamUtil::require(int required_size)
{
    return required_size <= nb_bytes - (p - bytes);
}

void StreamUtil::skip(int size)
{
    p += size;
}

int8_t StreamUtil::read_1bytes()
{
    return (int8_t)*p++;
}

int16_t StreamUtil::read_2bytes()
{
    int16_t value;
    char* pp = (char*)&value;
    pp[1] = *p++;
    pp[0] = *p++;

    return value;
}

int32_t StreamUtil::read_3bytes()
{
    int32_t value = 0x00;
    char* pp = (char*)&value;
    pp[2] = *p++;
    pp[1] = *p++;
    pp[0] = *p++;

    return value;
}

int32_t StreamUtil::read_4bytes()
{
    int32_t value;
    char* pp = (char*)&value;
    pp[3] = *p++;
    pp[2] = *p++;
    pp[1] = *p++;
    pp[0] = *p++;

    return value;
}

int64_t StreamUtil::read_8bytes()
{
    int64_t value;
    char* pp = (char*)&value;
    pp[7] = *p++;
    pp[6] = *p++;
    pp[5] = *p++;
    pp[4] = *p++;
    pp[3] = *p++;
    pp[2] = *p++;
    pp[1] = *p++;
    pp[0] = *p++;

    return value;
}

string StreamUtil::read_string(int len)
{
    std::string value;
    value.append(p, len);

    p += len;

    return value;
}

void StreamUtil::read_bytes(char* data, int size)
{
    memcpy(data, p, size);

    p += size;
}

void StreamUtil::write_1bytes(int8_t value)
{
    assert(require(1));

    *p++ = value;
}

void StreamUtil::write_2bytes(int16_t value)
{
    assert(require(2));

    char* pp = (char*)&value;
    *p++ = pp[1];
    *p++ = pp[0];
}

void StreamUtil::write_4bytes(int32_t value)
{
    assert(require(4));

    char* pp = (char*)&value;
    *p++ = pp[3];
    *p++ = pp[2];
    *p++ = pp[1];
    *p++ = pp[0];
}

void StreamUtil::write_3bytes(int32_t value)
{
    assert(require(3));

    char* pp = (char*)&value;
    *p++ = pp[2];
    *p++ = pp[1];
    *p++ = pp[0];
}

void StreamUtil::write_8bytes(int64_t value)
{
    assert(require(8));

    char* pp = (char*)&value;
    *p++ = pp[7];
    *p++ = pp[6];
    *p++ = pp[5];
    *p++ = pp[4];
    *p++ = pp[3];
    *p++ = pp[2];
    *p++ = pp[1];
    *p++ = pp[0];
}

void StreamUtil::write_string(string value)
{
    assert(require((int)value.length()));

    memcpy(p, value.data(), value.length());
    p += value.length();
}

void StreamUtil::write_bytes(char* data, int size)
{
    assert(require(size));

    memcpy(p, data, size);
    p += size;
}