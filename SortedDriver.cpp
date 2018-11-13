// SortedDriver.cpp

// tom bailey   1445  25 mar 2014
// Construct sorted sequences and call functions that
//   process the sorted sequences.

#include "stdafx.h"
#include "RandomUtilities.h"
#include "ContainerPrinting.h"
#include "winTimer.h"//CHANGE: this to unixTimer.h if on mac/linux
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> //for absolute value

using std::distance;

using namespace std;

// post: a sorted vector of listSize random doubles from
//         the range minNum to maxNum has been returned.
vector<double>
getNums(size_t listSize, double minNum, double maxNum)
{
	vector<double> theList;
	for (size_t i = 0; i < listSize; ++i)
	{
		theList.push_back(randReal(minNum, maxNum));
	}

	sort(begin(theList), end(theList));

	return theList;
}


// post: a sorted list of numWords words of length
//         wordLength with characters chosen randomly from
//         alphabet has been returned.
list<string>
getWords(size_t numWords, size_t wordLength, string alphabet)
{
	list<string> theWords;
	for (size_t i = 0; i < numWords; ++i)
	{
		string word;
		for (size_t j = 0; j < wordLength; ++j)
		{
			word += alphabet.at(randInt(0, alphabet.size()));
		}
		theWords.push_back(word);
	}

	theWords.sort();

	return theWords;
}


// pre:  number is not empty;
//       number is sorted from smallest to largest
// post: The most isolated entry in number has been returned
double
mostIsolated(vector<double> & number)
{
	if (number.empty()) //if the vector is empty just return -1, not really possible in the code anyways
	{
		return -1;
	}
	else if (number.size() == 1)
	{
		return number[0];
	}
	std::vector<double>::iterator iter = number.begin();
	sort(iter, iter + number.size());

	int iso = 0; //what member of the vector is it
	double gDiff = 0; //greatest difference
	double prevDiff = 0; // difference of most isolated element so far and the element preceding it


	for (size_t i = 0; i < number.size(); i++)
	{

		if (i == 0)
		{
			gDiff = number[i + 1] - number[i];
			iso = i;
		}
		else if (i == (number.size() - 1) )
		{
			if ( (number[i] - number[i-1]) > gDiff)
			{
				gDiff = number[i] - number[i-1];
				iso = i;
			}
		}
		else
		{
			int nearest = 0; //vector position of nearest element
			double spaceB = number[i] - number[i - 1]; //space before
			double spaceF = number[i + 1] - number[i]; //space following
			
			if (spaceB > spaceF){ nearest = i + 1; }			
			else { nearest = i - 1; }

			double absolute = abs(number[i] - number[nearest]);
			if ( (absolute > gDiff) || ( iso == nearest && prevDiff < spaceF )  )
			{
				iso = i;
				prevDiff = spaceB;
				gDiff = abs(number[i] - number[nearest]);	
			}

			
		}
	}

	return number[iso];
}


// pre:  A and B are sorted.
// post: The number of strings in A that do not occur in B
//         has been returned.
int
unmatched(list<string> & A, list<string> & B)
{
	A.sort();
	B.sort();

	std::list<string>::iterator iterA = A.begin();

	int unmatched = 0;
	while(iterA != A.end())
	{
		std::list<string>::iterator iterB = B.begin();
		boolean found = false;
		while (*iterA >= *iterB)
		{
			if (*iterA == *iterB)
			{
				found = true;
			}
			iterB++;
		}		
		iterA++;
		if (!found) { unmatched++; }
	}

	return unmatched;
}


int main()
{
	cout << "Find the most isolated number" << endl
		<< "-----------------------------" << endl << endl;
	while (true)
	{
		cout << "Enter size for numbers: ";
		int n = 0;
		cin >> n;
		if (n <= 0)
			break;
		cout << "Enter seed for rand: ";
		unsigned int seed;
		cin >> seed;
		srand(seed);

		// Construct a sorted list of numbers
		Timer get;
		get.start();
		vector<double> numbers = getNums(n, -n, n);
		get.stop();
		cout << "Constructed in " << get() << " seconds"
			<< endl;

		if (n < 10)
			cout << numbers << endl << endl;

		// Report a most isolated isolated number
		Timer time;
		time.start();
		double isolated = mostIsolated(numbers);
		time.stop();
		cout << "The most isolated number is "
			<< isolated << endl
			<< "calculated in " << time() << " seconds"
			<< endl << endl;
	}


	cout << endl << endl;
	cout << "Count the unmatched words" << endl
		<< "-------------------------" << endl << endl;
	while (true)
	{
		cout << "Enter size for words lists: ";
		int n = 0;
		cin >> n;
		if (n <= 0)
			break;
		cout << "Enter word length: ";
		int wordSize;
		cin >> wordSize;
		cout << "Enter alphabet: ";
		string alphabet;
		cin >> alphabet;

		cout << "Enter seed for rand: ";
		unsigned int seed;
		cin >> seed;
		srand(seed);

		// Construct two sorted vectors of words
		Timer get;
		get.start();
		list<string> A = getWords(n, wordSize, alphabet);
		list<string> B = getWords(n, wordSize, alphabet);
		get.stop();
		cout << "Constructed in " << get() << " seconds"
			<< endl;

		if (wordSize * n < 60)
		{
			cout << "A is: " << A << endl
				<< "B is: " << B << endl;
		}

		// Report the number of words in the first sorted list
		//   that are not in the second sorted list
		Timer time;
		time.start();
		int count = unmatched(A, B);
		time.stop();
		cout << count
			<< " words in A were not in B" << endl
			<< "calculated in " << time() << " seconds"
			<< endl << endl;
	}

	return 0;
}
