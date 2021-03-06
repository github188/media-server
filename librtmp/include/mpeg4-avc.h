#ifndef _mpeg4_avc_h_
#define _mpeg4_avc_h_

#include <stdint.h>

struct mpeg4_avc_t
{
//	uint8_t version; // 1-only
	uint8_t profile;
	uint8_t compatibility; // constraint_set[0-5]_flag
	uint8_t level;
	uint8_t nalu; // NALUnitLength, default 0x03

	uint8_t nb_sps;
	uint8_t nb_pps;

	struct mpeg4_avc_sps_t
	{
		uint16_t bytes;
		uint8_t data[64];
	}  sps[32]; // [0-31]

	struct mpeg4_avc_pps_t
	{
		uint16_t bytes;
		uint8_t data[64];
	} pps[256];

	// extension
	uint8_t chroma_format_idc;
	uint8_t bit_depth_luma_minus8;
	uint8_t bit_depth_chroma_minus8;
};

int mpeg4_avc_decoder_configuration_record_load(const uint8_t* data, size_t bytes, struct mpeg4_avc_t* avc);

int mpeg4_avc_decoder_configuration_record_save(const struct mpeg4_avc_t* avc, uint8_t* data, size_t bytes);

int mpeg4_avc_to_nalu(const struct mpeg4_avc_t* avc, uint8_t* data, size_t bytes);

#endif /* !_mpeg4_avc_h_ */
