#include "functionLibrary.hpp"

std::vector<std::string> split(std::string splitee, char delimiter)
{
  std::vector<std::string> output;
  std::string buffer = "";
  for(int i = 0; i < splitee.size(); i++)
  {
    if(splitee.at(i) == delimiter)
    {
      output.push_back(buffer);
      buffer = "";
    }
    else
    {
      buffer += std::string(1,splitee.at(i));
    }
  }
  output.push_back(buffer);
  return output;
}

int numEmptyStrings(std::vector<std::string> stringVector)
{
  int output = 0;
  for(int  i = 0; i < stringVector.size(); i++)
  {
    if(stringVector.at(i) == "")
    {
      output++;
    }
  }
  return output;
}

void parselatlong(std::string latlong, float& latlongval){
  int beginMinutes = latlong.find('.')-2;
  std::string minutes = latlong.substr(beginMinutes);
  std::string degrees = latlong.substr(0,beginMinutes);
  latlongval = std::stof(degrees) +  std::stof(minutes) / 60.0f;
}

void parsetime(std::string time, unsigned int& hours, unsigned int& minutes, float& seconds){
  hours = (unsigned int)std::stoi(time.substr(0,2));
  minutes = (unsigned int)std::stoi(time.substr(2,2));
  seconds = std::stof(time.substr(4));
}
