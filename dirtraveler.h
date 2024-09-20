#ifndef DIRTRAVELER_H
#define DIRTRAVELER_H

#include <boost/filesystem.hpp>


class TravelerInterface {
protected:
	using EntryList = std::vector<boost::filesystem::directory_entry>;
public:
	virtual ~TravelerInterface() = default;

	virtual EntryList getFiles() = 0;
};

template <typename Iterator>
class DirTraveler : public TravelerInterface {
	using FilterFunction = std::function<bool (Iterator)>;
public:
	DirTraveler(const boost::filesystem::path& dir) : m_path(dir) {}

	/*
	 * Попытка сделать универсальную работу с фильтрами
	 * - Директории для исключения
	 * - Минимальный размер файла
	 * - Фильтрация по маске
	 */
	void addFilter(FilterFunction filter_function);
	EntryList getFiles() override;

private:
	boost::filesystem::path m_path;
	std::vector<FilterFunction> m_filters;
};

template<typename Iterator>
void DirTraveler<Iterator>::addFilter(FilterFunction filter_function)
{
	m_filters.push_back(filter_function);
}

template<typename Iterator>
TravelerInterface::EntryList DirTraveler<Iterator>::getFiles()
{
	EntryList result;
	auto begin = Iterator(m_path);
	auto end = boost::filesystem::end(begin);
	for (auto it = begin; it != end; ++it) {
		bool filtersPass = std::all_of(m_filters.begin(), m_filters.end(),
									   [it](FilterFunction filter) {
				return filter(it);;
	});
		if (!filtersPass) {
			continue;
		}
		if (!boost::filesystem::is_regular_file(*it)) {
			continue;
		}
		result.push_back(*it);
	}

	return result;
}

#endif // DIRTRAVELER_H
