/*

NecklaceMatching.cpp - Baran Berkay Hökelek

This program contains the necessary functions to complete the "Necklace Matching" challenge (Challenge #383, Easy), and the 2 bonus challenges presented in r/dailyprogrammer at 09/03/2020.
Link: https://www.reddit.com/r/dailyprogrammer/comments/ffxabb/20200309_challenge_383_easy_necklace_matching/

*/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <sys/time.h>

// returns the current time
static const double kMicro = 1.0e-6;
double get_time() {
	struct timeval TV;
	struct timezone TZ;
	const int RC = gettimeofday(&TV, &TZ);
	if(RC == -1) {
		printf("ERROR: Bad call to gettimeofday\n");
		return(-1);
	}
	return( ((double)TV.tv_sec) + kMicro * ((double)TV.tv_usec) );
}

// puts the first character of the given string to its last place.
std::string putBack(std::string str)
{
  return (str.substr(1) + str.substr(0,1));
}


// checks whether 2 given strings belong to the same necklace, i.e. if they belong to the same cyclical permutation.
std::string same_necklace(std::string n1, std::string n2)
{
  std::string::size_type size = n1.size(); 
  for(std::string::size_type i = 0; i < 2*size; i++)
    {
      n2 = putBack(n2);
      if (n1.compare(n2) == 0) return "true";
    }
  return "false";
}

// checks how many times the original string ends up being repeated if putBack operation is applied to it n times. (n = length of the string)
int repeats(std::string str)
{
  std::string::size_type size = str.size();
  int count = 0;
  std::string tmp = str;
  for(std::string::size_type i = 0; i < size; i++)
    {
      tmp = putBack(tmp);
      if (str.compare(tmp) == 0) count++;
    }
  return count;
}

// main function
int main(void)
{
  std::string  necklace1, necklace2, cont;
  double time_0, time_1;
  int mode;
  std::cout << "Select mode: (1: Necklace Matching, 2: Repeats, 3: enable1 List Find)" << std::endl;
  std::cin >> mode;
  switch (mode)
    {
    case 1: // necklace matching
      while (true)
	{
	  std::cout << "Necklace 1:    ";
	  std::cin >> necklace1;
	  std::cout << "Necklace 2:    ";
	  std::cin >> necklace2;
	  std::string result = same_necklace(necklace1, necklace2);
	  std::cout << "same_necklace(\"" << necklace1 << "\", \"" << necklace2 << "\") => " << result << std::endl;
	  std::cout << "continue?(y/n)" << std::endl;
	  std::cin >> cont;
	  while (cont.compare("n") != 0 && cont.compare("y") != 0)
	    {
	      std::cout << "Please enter a valid string. Continue?(y/n)" << std::endl;
	    }
	  if (cont.compare("n") == 0) break;
	}
    case 2: // repeats
      while (true)
	{
	  std::cout << "Necklace:    ";
	  std::cin >> necklace1;
	  int result = repeats(necklace1);
	  std::cout << "repeats(\"" << necklace1 << "\") => " << result << std::endl;
	  std::cout << "continue?(y/n)" << std::endl;
	  std::cin >> cont;
	  while (cont.compare("n") != 0 && cont.compare("y") != 0)
	    {
	      std::cout << "Please enter a valid string. Continue?(y/n)" << std::endl;
	    }
	  if (cont.compare("n") == 0) break;
	}
    case 3: // read file
      time_0 = get_time();
      std::ifstream myfile ("enable1.txt");
      std::string line;
      int maxLength = 0;
      int numLines = 0;
      if(myfile.is_open())
	{
	  while(std::getline(myfile,line)) {
	    ++numLines;
	    if (line.length() > maxLength) maxLength = static_cast<int>(line.length());
	  }
	}
      myfile.close();
      std::string *lines = (std::string *) malloc(sizeof(std::string) * numLines);
      std::vector<std::string> lenSorted[maxLength + 1];
      myfile.open("enable1.txt");
      if(myfile.is_open())
	{
	  for (int i=0; i<numLines; i++)
	    {
	      std::getline(myfile, line);
	      lenSorted[static_cast<int>(line.length())].push_back(line);
	    }
	}
      myfile.close();
      const std::string alpha = "abcdefghijklmnopqrstuvwxyz";
      for(int i=4; i<maxLength + 1; i++)
	{
	  std::vector<std::string> charSums[26 * i];
	  for(int j=0; j<lenSorted[i].size(); j++)
	    {
	      int sum = 0;
	      std::string text = lenSorted[i][j];
	      for(char& c: text)
		{
		  const auto pos = alpha.find(c);

		  if( pos != std::string::npos ) // if found (if the character is an alpha character)
		    // note: non-alpha characters are ignored
		    {
		      const int value = pos + 1 ; // +1 because position of 'A' is 0, value of 'A' is 1
		      sum += value ;
		    }
		}
	      charSums[sum].push_back(text);
	    }
	  for(int j=0; j<26*i; j++)
	    {
	      if(!charSums[j].empty() && charSums[j].size() >= 4)
		{	  
		  for (int k=0; k<charSums[j].size() - 3; k++)
		    {
		      for (int l=k+1; l<charSums[j].size() - 2; l++)
			{
			  if(same_necklace(charSums[j][k], charSums[j][l]) == "true")
			    {
			      for (int m=l+1; m<charSums[j].size() - 1; m++)
				{
				  if(same_necklace(charSums[j][l], charSums[j][m]) == "true")
				    {
				      for (int n=m+1; n<charSums[j].size(); n++)
					{
					  if(same_necklace(charSums[j][m], charSums[j][n]) == "true")
					    {
					      time_1 = get_time();
					      std::cout << "Four words are: (" << charSums[j][k] << ", " << charSums[j][l] << ", " << charSums[j][m] << ", " << charSums[j][n] << ")" << std::endl;
					      std::cout << std::setprecision(6) << "Elapsed time: " << time_1-time_0 << "s." <<  std::endl;
					      return 0;
					    }
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }

  return 0;
}



