#include <iostream>

#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <type_traits>

#include "dirtraveler.h"

using namespace std;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

template <typename Iterator>
unique_ptr<TravelerInterface> createTraveler(const po::variables_map &params) {
	unique_ptr<DirTraveler<Iterator>> result(new DirTraveler<Iterator>(params["dir"].as<fs::path>()));

	if (params.count("ignore")) {
		result->addFilter([&params](Iterator it) {
			if constexpr (std::is_same<Iterator, fs::recursive_directory_iterator>::value) {
				if (fs::is_directory(*it) &&
						it->path().filename() == params["ignore"].as<string>()) {
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

	if (params.count("minsize")) {
		result->addFilter([&params](Iterator it) {
			if (!fs::is_regular_file(*it)) {
				return true;
			}
			static size_t minFileSize = params["minsize"].as<size_t>();
			return fs::file_size(*it) >= minFileSize;
		});
	}

	if (params.count("mask")) {
		result->addFilter([&params](Iterator it) {
			if (!fs::is_regular_file(*it)) {
				return true;
			}
//			static const boost::regex mask("^.*\\.txt$");
			static const boost::regex mask(params["mask"].as<string>());
			return boost::regex_match(it->path().filename().string(), mask);
		});
	}

	return result;
}

int main(int argc, const char *argv[])
{
	size_t blockSize;

	po::options_description desc{"Options"};
	desc.add_options()
			("help,h", "Показать эту справку и выйти")
			("dir", po::value<fs::path>()->default_value("./"), "Папка для сканирования")
			("ignore,I", po::value<string>(), "Папаки исключения")
			("recursive,R", "Рекурсивно показывать каталоги")
			("minsize", po::value<size_t>()->default_value(1),"Минимальный размер файла")
			("mask", po::value<string>(), "Маска разрешенных имен файлов")
			("block-size", po::value<int>()->default_value(5), "Размер блока (S)")
			("algoritm", po::value<string>()->default_value("crc32"), "Алгоритм сравнения crc32/md5");
	po::variables_map vm;
	po::store(parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << endl;
		return 0;
	}

	if (!fs::is_directory(vm["dir"].as<fs::path>())) {
		cerr << "Неверно задана дирректория для сканирования" << endl;
		return 1;
	}

	std::unique_ptr<TravelerInterface> treveler;
	if (vm.count("recursive")) {
		treveler = createTraveler<fs::recursive_directory_iterator>(vm);
	} else {
		treveler = createTraveler<fs::directory_iterator>(vm);
	}
	if (!treveler) {
		cerr << __LINE__ << endl;
		return 1;
	}

	//	if (vm.count("ignore")) {
	// Добавляем фильтр на игнорирование папки
	// Подумать как сделать
	//		ptr->addFilter([](fs::directory_entry itr) {
	//			if (fs::is_directory(itr)) {
	//				cout << itr << endl;
	//				fs::
	//			}
	//			return true;
	//		});
	//	}

	//	if (vm.count("minSize")) {
	//		size_t minSize = vm["minSize"].as<size_t>();
	//		ptr->addFilter([minSize](fs::directory_entry it) {
	//			if (fs::is_directory(it)) {
	//				return true;
	//			}
	//			return fs::file_size(it) >= minSize;
	//		});
	//	}

	//	if (vm.count("ignore")) {
	//		cout << "Ignore " << vm["ignore"].as<string>() << endl;
	//	}

	treveler->getFiles();

	return 0;
}
