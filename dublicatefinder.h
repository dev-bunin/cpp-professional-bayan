#ifndef DUBLICATEFINDER_H
#define DUBLICATEFINDER_H

#include <memory>
#include <vector>
#include <boost/filesystem.hpp>

class HasherInterface;
class TravelerInterface;

class DublicateFinder
{
public:
	DublicateFinder(std::unique_ptr<HasherInterface> hasher,
					std::unique_ptr<TravelerInterface> traveler,
					size_t blockSize);

	void printDublicates() const;

private:
	using Dublicates = std::vector<boost::filesystem::directory_entry>;

	std::shared_ptr<HasherInterface> m_hasher;
	std::shared_ptr<TravelerInterface> m_traveler;
	size_t m_blockSize = 5;

	std::vector<Dublicates> getDublicates() const;
};

#endif // DUBLICATEFINDER_H
