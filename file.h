#ifndef FILE_H
#define FILE_H

#include <boost/filesystem.hpp>
#include <fstream>

#include <iostream>

template <typename Hasher, int block_size>
class File
{
public:
	File(boost::filesystem::directory_entry entry)
		: m_entry(entry), m_size(boost::filesystem::file_size(entry))
	{
		m_blockCount = m_size / block_size;
		if ((m_size % block_size) != 0) {
			m_blockCount + 1;
		}
	}

	void print() { std::cout << m_entry.path() << std::endl; };

	uint32_t getBlock(int id) const {
		if (m_blocks.count(id)) {
			return m_blocks[id];
		}
		std::ifstream fs;
		fs.open(m_entry.path());
		if (!fs.is_open()) {
			return -1;
		}

		char data[block_size];
		fs.seekg(block_size * id);
		fs.read(data, block_size);
		fs.close();
		m_blocks[id] = Hasher::getChecksum(data, block_size);

		return m_blocks[id];
	}

	bool operator==(const File &other) {
		if (m_size != other.m_size) {
			return false;
		}

		for (int i = 0; i < m_blockCount; ++i) {
			// Сравниваем по блочно, пока не найдем различные
			if (getBlock(i) != other.getBlock(i)) {
				return false;
			}
		}
		return true;
	}

private:
	boost::filesystem::directory_entry m_entry;

	size_t m_size;
	int m_blockCount;

	mutable std::map<int, uint32_t> m_blocks;
};

template <typename Hasher, int block_size>
using FileList = std::vector<File<Hasher, block_size>>;

#endif // FILE_H
