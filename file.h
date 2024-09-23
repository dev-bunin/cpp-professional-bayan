#ifndef FILE_H
#define FILE_H

#include <boost/filesystem.hpp>
#include <fstream>
#include <memory>

class HasherInterface;

class File
{
public:
	File(boost::filesystem::directory_entry entry,
		 std::shared_ptr<HasherInterface> hasher,
		 size_t block_size);

	boost::filesystem::directory_entry getEntry();

	bool operator==(const File &other) const;

private:
	std::shared_ptr<HasherInterface> m_hasher;
	size_t m_blockSize;

	boost::filesystem::directory_entry m_entry;
	size_t m_size;
	int m_blockCount;
	mutable std::map<int, uint32_t> m_blocks;

	uint32_t getBlock(int id) const;
};

template <typename Hasher, int block_size>
using FileList = std::vector<File>;

#endif // FILE_H
