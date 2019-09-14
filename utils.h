//
// Created by hijiang on 2019/7/30.
//

#ifndef GB28181_HIKVISION_JAIN_SIP_UTILS_H
#define GB28181_HIKVISION_JAIN_SIP_UTILS_H
#include <stdlib.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <libyuv.h>
#ifdef __cplusplus
}
#endif

class Utils {
public:
    static int64_t getCurrentTimeMs();
    static uint32_t crc32(const void* buf, int size);
    static uint32_t mpegts_crc32(const uint8_t *data, int len);
    /* yuv translate fun */
    static void scaleI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint dst_width,
                          jint dst_height, jint mode);
    static void rotateI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint degree);

    static void mirrorI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data);

    static void nv21ToI420(jbyte *src_nv21_data, jint width, jint height, jbyte *src_i420_data);
};
#endif //GB28181_HIKVISION_JAIN_SIP_UTILS_H
