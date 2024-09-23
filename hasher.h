#ifndef HASHER_H
#define HASHER_H

#include <cstdint>
#include <cstddef>

class HasherInterface {
public:
	virtual uint32_t getChecksum(char *data, size_t size) const = 0;
};

class MD5Hasher : public HasherInterface {
public:
	MD5Hasher() = default;

	uint32_t getChecksum(char *data, size_t size) const override;
};

class Crc32Hasher : public HasherInterface {
public:
	Crc32Hasher() = default;

	uint32_t getChecksum(char *data, size_t size) const override;
};

#endif // HASHER_H
