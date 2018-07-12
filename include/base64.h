
#define BLACK_SQUARE        "▪"
#define WHITE_SQUARE        "▫"

void hexDump(const char *desc, void *addr, int len, int offset);
std::string B64Encode(unsigned char const *data, int dataSize);
std::string B64Decode(unsigned char const *data, int dataSize);
