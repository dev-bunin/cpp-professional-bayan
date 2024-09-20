#ifndef DIRTRAVELER_H
#define DIRTRAVELER_H

#include <boost/filesystem.hpp>
#include <iostream>

class TravelerInterface {
protected:
public:
	virtual ~TravelerInterface() = default;

	virtual void getFiles() = 0;
};

template <typename Iterator>
class DirTraveler : public TravelerInterface {
	using FilterFunction = std::function<bool (Iterator)>;
public:
	DirTraveler(const boost::filesystem::path& dir) : m_path(dir) {}

	void addFilter(FilterFunction filter_function);
	void getFiles() override;

private:
	boost::filesystem::path m_path;
	std::vector<FilterFunction> m_filters;
};

template<typename Type>
void printer(Type value) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<typename Iterator>
void DirTraveler<Iterator>::addFilter(FilterFunction filter_function)
{
	m_filters.push_back(filter_function);
}

template<typename Iterator>
void DirTraveler<Iterator>::getFiles()
{
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
		auto path = it->path();
		if (boost::filesystem::is_directory(path)) {
			continue;
		}
		std::cout << path << '\t' << boost::filesystem::file_size(path) << std::endl;
	}
}

#endif // DIRTRAVELER_H
