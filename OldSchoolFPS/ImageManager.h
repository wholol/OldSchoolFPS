#pragma once
#include <string>
#include <unordered_map>
#include <SFML\Graphics.hpp>

class ImageManager
{
public:
	static ImageManager& GetInstanace()	//get the instace	
	{
		static ImageManager Instance;
		return Instance;
	}

	ImageManager(const ImageManager&) = delete;
	void addImage(const std::string& imageName);		
	sf::Image& getImage(const std::string& imageName);

private:
	ImageManager() {};
	sf::Image img;
	std::unordered_map<std::string, sf::Image> map;
	
};