#include "file.h"

#include "hasher.h"


File::File(boost::filesystem::directory_entry entry,
							   std::shared_ptr<HasherInterface> hasher,
							   size_t blockSize)
	: m_entry(entry), m_size(boost::filesystem::file_size(entry)),
	  m_hasher(hasher), m_blockSize(blockSize)
{
	m_blockCount = m_size / blockSize;
	if ((m_size % blockSize) != 0) {
		m_blockCount += 1;
	}
}

boost::filesystem::directory_entry File::getEntry()
{
	return m_entry;
}

bool File::operator==(const File &other) const
{
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

uint32_t File::getBlock(int id) const
{
	if (m_blocks.count(id)) {
		return m_blocks[id];
	}
	std::ifstream fs;
	fs.open(m_entry.path());
	if (!fs.is_open()) {
		return -1;
	}

	char data[m_blockSize];
	fs.seekg(m_blockSize * id);
	fs.read(data, m_blockSize);
	fs.close();
	m_blocks[id] = m_hasher->getChecksum(data, m_blockSize);

	return m_blocks[id];
}
