#include "mpeg-ps.h"
#include "mpeg-ts.h"
#include <assert.h>
#include <stdio.h>

static void* ts_alloc(void* /*param*/, size_t bytes)
{
	static char s_buffer[188];
	assert(bytes <= sizeof(s_buffer));
	return s_buffer;
}

static void ts_free(void* /*param*/, void* /*packet*/)
{
	return;
}

static void ts_write(void* param, const void* packet, size_t bytes)
{
	fwrite(packet, bytes, 1, (FILE*)param);
}

extern  "C" size_t mpeg_ts_h264(void* h264, size_t bytes);

static void ts_packet(void* param, int avtype, int64_t pts, int64_t dts, void* data, size_t bytes)
{
	mpeg_ts_write(param, avtype, pts, dts, data, bytes);
}

static void mpeg_ts_file(const char* file, void* ts)
{
	unsigned char ptr[188];
	FILE* fp = fopen(file, "rb");
	while (1 == fread(ptr, sizeof(ptr), 1, fp))
	{
		mpeg_ts_packet_dec(ptr, sizeof(ptr), ts_packet, ts);
	}
	fclose(fp);
}

void mpeg_ts_test()
{
	FILE* fp = fopen("test/apple.ts", "wb");
	struct mpeg_ts_func_t tshandler;
	tshandler.alloc = ts_alloc;
	tshandler.write = ts_write;
	tshandler.free = ts_free;
	void* ts = mpeg_ts_create(&tshandler, fp);

	mpeg_ts_file("test/fileSequence0.ts", ts);
	
	mpeg_ts_destroy(ts);
	fclose(fp);
}
