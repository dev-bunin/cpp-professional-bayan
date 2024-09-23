#include "dublicatefinder.h"
#include "hasher.h"
#include "dirtraveler.h"
#include "file.h"

#include <iostream>

using namespace std;

DublicateFinder::DublicateFinder(std::unique_ptr<HasherInterface> hasher,
								 std::unique_ptr<TravelerInterface> traveler,
								 size_t blockSize)
	: m_hasher(move(hasher)), m_traveler(move(traveler)), m_blockSize(blockSize)
{

}

void DublicateFinder::printDublicates() const
{
	for (auto dublicateList : getDublicates()) {
		for (auto file : dublicateList) {
			cout << file.path().filename() << " ";
		}
		cout << '\n';
	}
}

std::vector<DublicateFinder::Dublicates> DublicateFinder::getDublicates() const
{
	vector<Dublicates> result;
	auto entrys = m_traveler->getFiles();

	if (entrys.size() <= 2) {
		return result;
	}

	// Переводим в удобную для работы структуру
	vector<File> files;
	files.reserve(entrys.size());
	for (auto &entry : entrys) {
		files.push_back({entry, m_hasher, m_blockSize});
	}

	for (auto it = files.begin(); it != files.end(); ++it) {
		bool hasDublicates = false;
		Dublicates dublicates;
		for (auto it2 = next(it); it2 != files.end();) {
			if (*it == *it2) {
				hasDublicates = true;
				dublicates.push_back(it2->getEntry());
				it2 = files.erase(it2);
			} else {
				++it2;
			}
		}
		if (hasDublicates) {
			dublicates.push_back(it->getEntry());
			result.push_back(dublicates);
		}
	}

	return result;
}
