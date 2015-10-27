//
// EagleAeroSport
//
// B.Davis
//
//////////////////


//------------------------------------------------
//
// Temp Development
//
//----------------------
// TinyConsole usage
// inherited class
// Poor Code Style - only #include-able from a single .cpp in a project
//
//


class tcon : public tinyConsole
{
public:
	tcon (std::string s): tinyConsole(s) {;}
	
	int trigger (std::string s)
	{
		if (s == "exit")
		{
			std::cout << "Exiting..." << std::endl;
      sv->gDone = true;
			quit();
		}
   return 0;
	}

	int hotkeys(char c)
	{
		switch (tolower(c))
    {
    //
    // Quit
    case 'q':
      std::cout << std::endl << "<CR> to Exit" << std::endl;
      sv->gDone = true;
      quit();
      break;
      
    case 'a':
      sv->gAccelEnabled = !(sv->gAccelEnabled);
      break;
      
    case 't':
      sv->gTempEnabled = !(sv->gTempEnabled);
      break;
      
    case 'p':
      sv->gPressEnabled = !(sv->gPressEnabled);
      break;
      
    //
    // Increase Speed
    case '+':
      {
        unsigned int new_period;
        //---------------------
        new_period = (sv->iLoopPause / 2);
        if (new_period < MIN_ILOOPPERIOD)
          new_period = MIN_ILOOPPERIOD;
        sv->iLoopPause = new_period;
        std::cout << "New Period = " << ((float)new_period/1E6) << std::endl;
        usleep(1.5E6);
        break;
      }
          //
    // Decrease Speed
    case '-':
      {
        unsigned int new_period;
        //---------------------
        new_period = (sv->iLoopPause * 2);
        if (new_period < MIN_ILOOPPERIOD)
          new_period = MIN_ILOOPPERIOD;
        sv->iLoopPause = new_period;
        std::cout << "New Period = " << ((float)new_period/1E6) << std::endl;
        usleep(1.5E6);
        break;
      }
    } // switch
		return 0;
	} // hotkeys
}; // tcon
