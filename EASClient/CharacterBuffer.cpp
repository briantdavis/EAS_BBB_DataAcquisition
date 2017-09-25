#include "CharacterBuffer.hpp"


CharacterBuffer::CharacterBuffer::CharacterBuffer(int newBufferSize)
{
	this->bufferSize = newBufferSize;
	this->bufferInputPosition = 0;
	this->bufferOutputPosition = 0;
	this->buffer = new char[this->bufferSize];
  for(int i = 0; i< this->bufferSize; i++)
  {
    this->buffer[i] = 0;
  }
	this->bufferStatusCode = BUFFER_UNDERFLOW;
}

CharacterBuffer::CharacterBuffer::~CharacterBuffer()
{
	delete [] this->buffer;
}

int CharacterBuffer::CharacterBuffer::push(char input)
{
	if(this->bufferStatusCode != BUFFER_OVERFLOW_IMMINENT && this->bufferStatusCode != BUFFER_OVERFLOW)
	{
		this->buffer[this->bufferInputPosition++] = input;
		this->bufferInputPosition %= this->bufferSize;
		
		if(this->bufferInputPosition == this->bufferOutputPosition)
		{
			return (this->bufferStatusCode = BUFFER_OVERFLOW_IMMINENT);
		}
		else
		{
			return (this->bufferStatusCode = BUFFER_SAFE);
		}
	}
	else
	{
		return (this->bufferStatusCode = BUFFER_OVERFLOW);
	}
}

int CharacterBuffer::CharacterBuffer::pushOver(char input)
{
	this->buffer[this->bufferInputPosition++] = input;
	this->bufferInputPosition %= this->bufferSize;
	if(this->bufferInputPosition > this->bufferOutputPosition
	   || (this->bufferInputPosition == 0 && this->bufferOutputPosition == (this->bufferSize - 1)))
	{
		this->bufferOutputPosition = this->bufferInputPosition;
		return (this->bufferStatusCode = BUFFER_OVERFLOW);
	}
	else if(this->bufferInputPosition == this->bufferOutputPosition)
	{
		return (this->bufferStatusCode = BUFFER_OVERFLOW_IMMINENT);
	}
	else
	{
		return (this->bufferStatusCode = BUFFER_SAFE);
	}
}

int CharacterBuffer::CharacterBuffer::pull(char& output)
{
  output = 0;
	if(this->bufferStatusCode != BUFFER_UNDERFLOW_IMMINENT && this->bufferStatusCode != BUFFER_UNDERFLOW)
	{
		output = this->buffer[bufferOutputPosition];
    this->buffer[bufferOutputPosition] = 0;
    bufferOutputPosition++;
		this->bufferOutputPosition = this->bufferOutputPosition % this->bufferSize;
		if(this->bufferInputPosition == this->bufferOutputPosition)
		{
			return (this->bufferStatusCode = BUFFER_UNDERFLOW_IMMINENT);
		}
		else
		{
			return (this->bufferStatusCode = BUFFER_SAFE);
		}
	}
	else
	{
		return (this->bufferStatusCode = BUFFER_UNDERFLOW);
	}
}

int CharacterBuffer::CharacterBuffer::peek(char& output, int positionFromHead){
      int truePosition = (bufferOutputPosition + positionFromHead) % bufferSize;
	  
      bool caseOutputPosPreceedsInputPos = bufferInputPosition > bufferOutputPosition
										   && truePosition < bufferInputPosition
										   && truePosition >= bufferOutputPosition;
	  
	  bool caseInputPosPreceedsOutputPos = bufferInputPosition < bufferOutputPosition
			&& !(truePosition < bufferOutputPosition && truePosition >= bufferInputPosition);
	  
	  bool caseInputPosEqualsOutputPos = bufferInputPosition == bufferOutputPosition
										 // truePosition == bufferOutputPosition
										 && bufferStatusCode != BUFFER_UNDERFLOW;
	  
      if(caseOutputPosPreceedsInputPos || caseInputPosPreceedsOutputPos || caseInputPosEqualsOutputPos){
        output = buffer[truePosition];
        return bufferStatusCode;        
      }                  
      else{
        output = 0;
        return bufferStatusCode;        
      }                                    
      
    }
    
int CharacterBuffer::CharacterBuffer::trash()
{
  char trash;
  return pull(trash);
}
    
int CharacterBuffer::CharacterBuffer::readFromUntil(std::string& output, char from, char until)
{
  output = "";
  char nextChar;
  int status = BUFFER_SAFE;
  do
  {
    this->pull(nextChar);
  } while( nextChar != from && nextChar != 0);
  
  if(nextChar == 0)
  {
    return this->bufferStatusCode;
  }
  else
  {
    output.push_back(nextChar);
    do
    {
      this->peek(nextChar);
      if(nextChar != until && nextChar != 0)
      {
        output.push_back(nextChar);
        this->trash();
      }
    } while( nextChar != until && nextChar != 0);
    
    return this->bufferStatusCode;
  }
}

int CharacterBuffer::CharacterBuffer::readBetween(std::string& output, char delimiter)
{
  return this->readFromUntil(output, delimiter, delimiter);
}
	
std::string CharacterBuffer::CharacterBuffer::str()
{
	std::string output = "[";
	
	for(int i = 0; i < this->bufferSize; i++)
	{
		output += "'" + std::string(1, this->buffer[i]) + "'";
		if(i != (this->bufferSize - 1))
		{
			output += ", ";
		}
	}
	output += "]";
	return output;
}

std::string CharacterBuffer::CharacterBuffer::loc()
{
	std::string output = "[";
	
	for(int i = 0; i < this->bufferSize; i++)
	{
		std::string insert = " ";
		if(i == this->bufferInputPosition
		   && i == this->bufferOutputPosition)
		{
			insert = "b";
		}
		else if(i == this->bufferInputPosition)
		{
			insert = "i";
		}
		else if(i == this->bufferOutputPosition)
		{
			insert = "o";
		}
		output += " " + insert + " ";
		if(i != (this->bufferSize - 1))
		{
			output += ", ";
		}
	}
	output += "]";
	return output;
}