#include <iostream>

#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <type_traits>
#include <unordered_set>

#include "dublicatefinder.h"
#include "dirtraveler.h"
#include "hasher.h"

using namespace std;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

template <typename Iterator>
unique_ptr<TravelerInterface> createTraveler(const po::variables_map &params) {
	unique_ptr<DirTraveler<Iterator>> result(new DirTraveler<Iterator>(params["dir"].as<vector<fs::path>>()));

	if (params.count("exeption")) {
		result->addFilter([&params](Iterator it) {
			if (!fs::is_directory(*it)) {
				return true;
			}
			if constexpr (std::is_same<Iterator, fs::recursive_directory_iterator>::value) {
				// Для быстрого поиска
				static unordered_set<string> exeptionDir = [&params]() {
					auto exeptionDirVector = params["exeption"].as<vector<string>>();
					return unordered_set<string>(exeptionDirVector.begin(), exeptionDirVector.end());
				}();
				if (exeptionDir.count(it->path().filename().string())) {
					it.disable_recursion_pending();
					return false;
				}
				return true;
			} else {
				// Рекурсивный поиск выключен
				return true;
			}
		});
	}

	size_t minSize = 1;
	if (params.count("minsize")) {
		minSize = params["minsize"].as<size_t>();
	}
	result->addFilter([minSize](Iterator it) {
		if (!fs::is_regular_file(*it)) {
			return true;
		}
		static size_t minFileSize = minSize;
		return fs::file_size(*it) >= minFileSize;
	});

	if (params.count("mask")) {
		result->addFilter([&params](Iterator it) {
			if (!fs::is_regular_file(*it)) {
				return true;
			}
			static const boost::regex mask(params["mask"].as<string>());
			return boost::regex_match(it->path().filename().string(), mask);
		});
	}

	return result;
}

bool getOptions(int argc, const char *argv[], po::variables_map &map) {
	// ToDo Добавить валидацию пльзовательского ввода
	po::options_description desc{"Parameters"};
	desc.add_options()
			("help,h", "Show help and close")
			("dir,d", po::value<vector<fs::path>>()->multitoken(), "Dir for search")
			("exeption,e", po::value<vector<string>>()->multitoken(), "Exception dir")
			("recursive,r", "Recursive search")
			("minsize,s", po::value<size_t>()->default_value(1),"Min mile size")
			("mask,m", po::value<string>(), "Filter mask")
			("block-size",po::value<size_t>()->default_value(5), "BlockSize")
			("algoritm", po::value<string>()->default_value("crc32"), "Hash algoritm (src32, md5)");
	po::store(parse_command_line(argc, argv, desc), map);
	po::notify(map);

	if (map.count("help")) {
		cout << desc << endl;
		return false;
	}

	return true;
}

int main(int argc, const char *argv[])
{
	po::variables_map options;
	if (!getOptions(argc, argv, options)) {
		return 0;
	}

	std::unique_ptr<TravelerInterface> treveler;
	if (options.count("recursive")) {
		treveler = createTraveler<fs::recursive_directory_iterator>(options);
	} else {
		treveler = createTraveler<fs::directory_iterator>(options);
	}

	std::unique_ptr<HasherInterface> hasher;
	if (options["algoritm"].as<string>() == "md5") {
		hasher.reset(new MD5Hasher());
	} else {
		hasher.reset(new Crc32Hasher());
	}

	if (!treveler || !hasher) {
		return 1;
	}

	DublicateFinder finder(move(hasher), move(treveler), options["block-size"].as<size_t>());

	finder.printDublicates();

	return 0;
}
