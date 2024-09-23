#include "hasher.h"

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

using namespace boost::uuids::detail;

uint32_t MD5Hasher::getChecksum(char *data, size_t size) const
{
	md5 hash;
	md5::digest_type digest;

	hash.process_bytes(data, size);
	hash.get_digest(digest);

	return reinterpret_cast<uint32_t>(*digest);
}

uint32_t Crc32Hasher::getChecksum(char *data, size_t size) const
{
	boost::crc_32_type hash;
	hash.process_bytes(data, size);
	return hash.checksum();
}
