#include "names.hpp"
#include <set>
#include <string>

static unsigned CHAR_PROB[43][43] = {
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 12, 12, 18, 2, 5, 13, 8, 9, 7, 21, 70, 17, 97, 2, 8, 1, 101, 32, 28, 4, 7, 3, 4, 7, 1,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 1, 0, 2, 15, 0, 0, 0, 5, 0, 0, 2, 0, 0, 9, 0, 0, 9, 1, 1, 0, 0, 0, 0, 1, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 2, 0, 4, 0, 0, 26, 4, 0, 0, 2, 0, 0, 14, 0, 0, 5, 1, 4, 1, 0, 0, 0, 3, 1,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 3, 17, 0, 1, 3, 19, 0, 0, 2, 0, 0, 25, 0, 0, 8, 0, 0, 8, 0, 1, 0, 2, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 2, 3, 8, 19, 5, 6, 4, 4, 0, 5, 41, 4, 38, 3, 3, 0, 55, 23, 16, 2, 7, 0, 6, 5, 1,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 1, 8, 8, 0, 0, 1, 0, 0, 3, 0, 1, 3, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 1, 0, 1, 11, 0, 0, 2, 7, 0, 0, 1, 0, 0, 11, 0, 0, 4, 0, 0, 3, 0, 1, 0, 1, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33, 0, 0, 0, 11, 0, 0, 0, 19, 0, 1, 1, 1, 1, 22, 1, 0, 7, 1, 1, 11, 0, 0, 0, 3, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 5, 7, 15, 4, 7, 3, 1, 7, 4, 9, 24, 8, 48, 13, 5, 0, 22, 36, 12, 2, 8, 0, 6, 0, 4,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 7, 0, 0, 1, 4, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 0, 0, 0, 9, 0, 0, 2, 21, 0, 5, 2, 1, 0, 14, 0, 0, 4, 2, 4, 8, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37, 0, 1, 3, 26, 0, 3, 0, 24, 0, 1, 27, 2, 2, 33, 5, 0, 1, 2, 5, 19, 3, 1, 0, 5, 2,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 6, 0, 0, 13, 0, 0, 0, 19, 0, 0, 1, 4, 0, 16, 1, 0, 2, 1, 0, 5, 1, 0, 0, 4, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, 0, 1, 30, 39, 1, 10, 0, 26, 1, 3, 0, 0, 12, 17, 1, 1, 2, 6, 20, 3, 0, 0, 0, 3, 4,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 3, 7, 1, 1, 3, 3, 12, 0, 5, 20, 6, 70, 19, 5, 0, 60, 33, 21, 11, 5, 4, 4, 3, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 3, 0, 0, 12, 14, 0, 0, 1, 0, 0, 12, 0, 0, 4, 1, 1, 1, 0, 0, 0, 0, 1,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66, 5, 5, 8, 25, 3, 5, 2, 62, 2, 10, 6, 7, 4, 40, 1, 1, 16, 1, 11, 19, 2, 0, 0, 4, 3,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 3, 0, 24, 0, 0, 22, 18, 0, 10, 1, 2, 1, 12, 9, 0, 4, 17, 22, 7, 0, 0, 0, 1, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 1, 0, 25, 0, 0, 40, 19, 0, 1, 1, 0, 0, 31, 0, 0, 14, 3, 6, 5, 0, 0, 0, 3, 1,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 5, 5, 4, 0, 0, 1, 5, 3, 4, 12, 10, 10, 0, 2, 1, 17, 24, 7, 7, 0, 0, 2, 2, 1,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 1, 0, 10, 0, 0, 0, 5, 2, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 5, 1, 0, 0, 1, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 7, 0, 0, 0, 0, 1, 3, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 2, 3, 7, 1, 2, 0, 0, 0, 2, 3, 1, 8, 5, 1, 0, 3, 5, 2, 2, 1, 0, 0, 2, 1,  },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 6, 0, 0, 1, 3, 0, 0, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,  },
};

static std::set<std::string> GENERATED_NAMES;

char choose(const std::string& str, std::default_random_engine& entropy)
{
	std::uniform_int_distribution<> distrib(0, str.length()-1);

	return str[distrib(entropy)];
}

bool has_vowels(const std::string& str, const std::string& vowels)
{
	bool has_vowels = false;
	for (auto c : vowels)
	{
		has_vowels |= (str.find(c) != std::string::npos);
	}

	return has_vowels;
}

unsigned argmax(unsigned* r, size_t len)
{
	unsigned i_max = 0;

	for (unsigned i = 1; i < len; i++)
	{
		if (r[i] > r[i_max]) { i_max = i; } 
	}

	return i_max;
}

std::string ld50::planet_name(std::default_random_engine& entropy)
{
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string vowels = "AEIOU";
	std::uniform_real_distribution<float> uni_real(0, 1);
	std::uniform_int_distribution<> uni_int(3, 10);
  
	unsigned ci = (choose(alphabet, entropy) - '0');
	auto len = uni_int(entropy);
	std::string name = "";

	for (int i = 0; i < len; i++)
	{
		name += (char)(ci + '0');

		if (uni_real(entropy) < 0.25f)
		{
			if (has_vowels(name, vowels))
			{
				ci = (choose(alphabet, entropy) - '0');
			}
			else
			{
				ci = (choose(vowels, entropy) - '0');
			}
		}
		else
		{
			ci = argmax(CHAR_PROB[ci], sizeof(CHAR_PROB[ci]) / sizeof(unsigned));
		}
	}

	GENERATED_NAMES.insert(name);

	return name;
}