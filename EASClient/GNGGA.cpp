#include "GNGGA.hpp"

GNGGA::GNGGA::GNGGA(std::string NMEAstring){
  std::vector<std::string> splitGNGGA = split(NMEAstring,',');
  
  std::string timestamp = splitGNGGA.at(1),
              latitudeString = splitGNGGA.at(2),
              latitudeDirString = splitGNGGA.at(3),
              longitudeString = splitGNGGA.at(4),
              longitudeDirString = splitGNGGA.at(5),
              hdopString = splitGNGGA.at(8),
              altitudeString = splitGNGGA.at(9),
              geoidHeightString = splitGNGGA.at(11);
              
  parsetime(timestamp, this->hours, this->minutes, this->seconds);
  parselatlong(latitudeString, this->latitude);
  parselatlong(longitudeString, this->longitude);
  this->hdop = std::stof(hdopString);
  this->altitude = std::stof(altitudeString);
  this->geoidHeight = std::stof(geoidHeightString);
  this->N = (latitudeDirString == "N");
  this->E = (longitudeDirString == "E");
}

float GNGGA::GNGGA::getTime()
{
  return this->hours*3600 + this->minutes*60 + this->seconds;
}

float GNGGA::GNGGA::getLat()
{
  return this->latitude;
}

float GNGGA::GNGGA::getLong()
{
  return this->longitude;
}

float GNGGA::GNGGA::getAlt()
{
  return this->altitude;
}

void GNGGA::GNGGA::print()
{
  std::cout << "GNGGA@ ";
  std::cout << this->hours << ":";
  std::cout << this->minutes << ":";
  std::cout << this->seconds << " UTC, ";
  std::cout << "ALT: " << this->altitude << " M, ";
  std::cout << "LAT: " << this->latitude << " " << ((this->N)?"N":"S") << ", ";
  std::cout << "LONG: " << this->longitude << " " << ((this->E)?"E":"W") << ", " << std::endl;
}
 

bool GNGGA::isGNGGA(std::string NMEAstr)
{
  if(NMEAstr.size() < 4)
  {
    //std::cout << "too small!" << std::endl;
    return false;
  }
  else
  {
    if(NMEAstr.at(0) != '$')
    {
      //std::cout << "no identifier!" << std::endl;
      return false;
    }
    else
    {
      int checksum = NMEAstr.at(1);
      int asterisk = -1;
      for(int i = 2; i < NMEAstr.size() && asterisk == -1; i++)
      {
        if(NMEAstr.at(i)!= '*')
        {
          checksum ^= NMEAstr.at(i);
        }
        else
        {
          asterisk = i;
        }
      }
      
      if(asterisk == -1 || asterisk == (NMEAstr.size() - 1))
      {
        //std::cout << "asterisk either doesnt exist or is at the end!" << std::endl;
        return false;
      }
      else
      {
        std::string apparentCS = NMEAstr.substr(asterisk + 1,NMEAstr.size());
        
        std::stringstream sstream;
        sstream << std::hex << checksum;
        std::string CS = sstream.str();
        if(apparentCS != CS)
        {
          //std::cout << "checksums dont match!" << std::endl;
          //std::cout << "\t" << apparentCS<< std::endl;
          //std::cout << "\t" << CS<< std::endl;
          return false;
        }
        else
        {
          if(NMEAstr.substr(1,5) != "GNGGA")
          {
            //std::cout << "tag isn't GNGGA!" << std::endl;
            //std::cout << "\t" << NMEAstr.substr(1,5)<< std::endl;
            return false; 
          }
          else
          {
            std::vector<std::string> NMEApieces = split(NMEAstr, ',');
            if(numEmptyStrings(NMEApieces) > 1)
            {
              //std::cout << "too many empty fields!" << std::endl;
              return false;
            }
            else
            {
              return true;
            }
          }
        }
      }
    }
  }
}