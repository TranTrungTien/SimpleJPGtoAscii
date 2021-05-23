#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>


/*It based on https://github.com/Muzkaw/JPG2ASCII
and https://www.jonathan-petitcolas.com/2017/12/28/converting-image-to-ascii-art.html */


//std::string ascii = "$@B%8 & WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
std::string ascii = "@%#*+=-:. ";

//you can see more at https://en.wikipedia.org/wiki/Grayscale 
sf::Image Grayscale(const sf::Image& image)
{
	sf::Image img;
	int width = image.getSize().x;
	int height = image.getSize().y;
	img.create(width, height, sf::Color::White);
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			sf::Color color = image.getPixel(x,y);
			int m_color = std::floor((color.r * 0.21) + (color.g * 0.72) + (color.b * 0.07));
			img.setPixel(x, y, sf::Color(m_color, m_color, m_color));
		}
	}
	return img;
}

sf::Image reduceResolution(const sf::Image& img, const sf::Vector2i& resolution)
{
	sf::Image m_img;
	int width = img.getSize().x;
	int height = img.getSize().y;
	m_img.create(width, height, sf::Color(255, 255, 255));

	// it will be like if reduce 1:
	// it redude height / 2. img will be 1080-675
	/*
		|@||@||#|

		|@||@||#|

		|.|| || |
		
		|.|| || |

	grayscale = sum of all pixel in area [number of pixel in this if reduce 1 is 2] / resolution.x * resolution.y
	@@ same grayscale
	.... 
	if reduce = 2 -> width / 2; height / 4;
	540 - 337
	*/
	
	for (int i = 0; i < width - resolution.x; i += resolution.x)
	{
		for (int j = 0; j < height - resolution.y; j += resolution.y)
		{
			int grayscale = 0;
			for (int x = 0; x < resolution.x; ++x)
			{
				for (int y = 0; y < resolution.y; ++y)
				{
					grayscale += img.getPixel(i + x, j + y).r;
				}
			}
			grayscale /= resolution.x * resolution.y;
			for (int x = 0; x < resolution.x; ++x)
			{
				for (int y = 0; y < resolution.y; ++y)
				{
					m_img.setPixel(i + x, j + y, sf::Color(grayscale, grayscale, grayscale));
				}
			}
		}
	}
	return m_img;
}

char getAscii(int grayScale)
{
	return ascii[std::ceil(grayScale / 255.f * (ascii.length() - 1))];
}
 
// see more at https://www.jonathan-petitcolas.com/2017/12/28/converting-image-to-ascii-art.html or at 
// https://medium.com/hd-pro/jpeg-image-scaling-algorithms-913987c9d588#:~:text=JPEG%20Scaling%20Techniques,nearest%20pixel%20in%20the%20output.&text=Bilinear%20%E2%80%94%20This%20interpolates%20pixels%20much%20better%20than%20Nearest%20Neighbor. 
// to resize image 
sf::Vector2u clamDimension(const sf::Vector2u& imageSize, const int& baseWidth)
{
	sf::Vector2u resize;
	if (baseWidth < imageSize.x)
	{
		int reduceHeight = floor(imageSize.y * baseWidth / imageSize.x);
		return sf::Vector2u(baseWidth, reduceHeight);
	}
	return sf::Vector2u(imageSize);
}

int main()
{
	
	std::string inputPath;
	std::string outputPath;
	int reduceResolutions;
	//int resizeWidth;
	std::cout << "Your Image [jpg format]: ";
	std::cin >> inputPath;
	std::cout << "Your Text File Name [txt format] :";
	std::cin >> outputPath;
	std::ofstream os(outputPath);
	std::cout << "Size of the character will be show [pixel] :";
	std::cin >> reduceResolutions;
	sf::Vector2i resolution(reduceResolutions, reduceResolutions * 2); // in notepad : row = 2colums; best if choose reduce = 2. cause in note max column length is 1025.

	sf::Image image;
	if (!image.loadFromFile(inputPath))
	{
		std::cout << "Error to load image from file" << std::endl;
		return -1;
	}


	//std::cout << "Choose Width [pixel] you want to resize. It musrt be <= " << image.getSize().x;
	//std::cin >> resizeWidth;

	//clamDimension(image.getSize(), resizeWidth);
	
	std::cout << "GrayScaling ... ";
	sf::Image img = Grayscale(image);
	img.saveToFile("girls.jpg");
	std::cout << "Done....\n" << "GrayScale Image saved\n" << "Reducing Resolution Image ... \n";
	sf::Image lowImage(reduceResolution(img, resolution));
	std::cout << "Done";
	std::string asciiText = "";
	std::cout << "Converting to Ascii";
	int width = lowImage.getSize().x;
	int height = lowImage.getSize().y;
	for (int y = 0; y < height; y += resolution.y)
	{
		for (int x = 0; x < width; x += resolution.x)
		{
			int grayscale = lowImage.getPixel(x,y).r;
			asciiText += getAscii(grayscale);
		}
		asciiText += '\n';
	}
	std::cout << "Done !!!\n";
	
	if (os)
	{
		os << asciiText;
		std::cout << "File Ascii saved ...\n";
	}
	else std::cout << "Error to saved file Ascii ...\n";
	return 0;
}