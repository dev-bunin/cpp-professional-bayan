#ifndef HASHER_H
#define HASHER_H

#include <cstdint>
#include <cstddef>

class HasherInterface
{
public:
	virtual uint32_t getChecksum(char *data, size_t size) const = 0;
};

class MD5Hasher {
public:
	MD5Hasher() = default;

	static uint32_t getChecksum(char *data, size_t size);
};

class Crc32Hasher {
public:
	Crc32Hasher() = default;

	static uint32_t getChecksum(char *data, size_t size);
};

#endif // HASHER_H
