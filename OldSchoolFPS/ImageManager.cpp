#include "ImageManager.h"

void ImageManager::addImage(const std::string& imageName)
{
	auto find = map.find(imageName);

	if (find != map.end())
	{
		//pass if image is already in the map.
	}
	else {
		
		if (!img.loadFromFile(imageName)) {
			throw std::runtime_error("image name invalid.");
		}
		else {
			map.insert(std::make_pair(imageName, img));
		}
	}
}

sf::Image& ImageManager::getImage(const std::string& imageName)
{
	auto find = map.find(imageName);

	if (find != map.end())
	{
		return find->second;
	}
	else
	{
		throw std::runtime_error("failed to get image from map.");
	}
}
