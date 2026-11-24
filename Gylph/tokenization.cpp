#include "tokenization.h"
#include <iostream>

void Tokenizator::Expect(char c)
{
	if (Match(c))
	{
		Advance();

		return;
	}
	std::cerr << "Failed to consume character: " << c << std::endl;
}