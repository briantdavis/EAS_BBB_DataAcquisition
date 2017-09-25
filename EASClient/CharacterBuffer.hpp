#ifndef CHARACTERBUFFER_HPP
#define CHARACTERBUFFER_HPP

#include <string>

namespace CharacterBuffer
{
	static int const BUFFER_OVERFLOW_IMMINENT = -2,
                     BUFFER_OVERFLOW = -1,
                     BUFFER_SAFE = 0,
                     BUFFER_UNDERFLOW=1,
                     BUFFER_UNDERFLOW_IMMINENT=2;
	
	
	class CharacterBuffer
	{
		protected:
			int bufferSize,
				bufferInputPosition,
				bufferOutputPosition,
				bufferStatusCode;
			char* buffer;
		public:
			CharacterBuffer(int);
			~CharacterBuffer();
			int push(char);
			int pushOver(char);
			int pull(char&);
			int peek(char&, int = 0);
      int trash();
      int readFromUntil(std::string&, char, char);
      int readBetween(std::string&, char);
			std::string str();
			std::string loc();
	};
}

#endif //CHARACTERBUFFER_HPP