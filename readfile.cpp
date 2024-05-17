#include "readfile.h"
#include <fstream>

std::vector<char>	readFile(const std::string& filename) {
	std::ifstream	file(filename, std::ios::ate | std::ios::binary);
	size_t			fileSize;

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	fileSize = (size_t)file.tellg();
	std::vector<char>	buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
