#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

using std::vector;
using std::string;


// Function declarations.
// #pragma region DECLARATIONS 

vector<int> ParseInputFile(const string& filename);
int GetMaximumLength(const vector<int>& prefixes);

// #pragma endregion


// Execution entrypoint.
// Algorithm reference:
// https://www.geeksforgeeks.org/longest-subarray-having-average-greater-than-or-equal-to-x-set-2/
int main(int argc, char** argv)
{
	// Get the filename from the first command-line argument and parse its contents,
	// generating a simplified prefix vector.
	auto prefixes = ParseInputFile(string(argv[1]));

	// Solve the algorithm.
	auto result = GetMaximumLength(prefixes);

	// $$$
	std::cout << result << std::endl;
}

/// <summary>
/// Read some data from a file.
/// </summary>
vector<int> ParseInputFile(const string& filename)
{
	// Initialize some local variables.
	string line1, line2;
	string delim = " ";
	int token, simplifiedValue;
	size_t pos = 0;

	std::ifstream inputFile(filename);
	std::getline(inputFile, line1);

	pos = line1.find(delim);
	size_t dc = std::stoi(line1.substr(0, pos)); // get the days count.
	auto hc = std::stoi(line1.substr(pos)); // get the hospital count.

	vector<int> prefixes = { 0 };
	prefixes.reserve(dc + 1);

	int i = 0;
	while (std::getline(inputFile, line2, ' '))
	{
		token = std::stoi(line2);
		// This is kinda wonky, but the gist of it is our problem is solved by the metric: 
		// [[ SUM( M )/( N*K ) <= -1 ]]
		// where M : a sub-array, N: length of M, K: a constant.
		// By moving around some math, it is trivial to restructure it as:
		// [[ -1 * (SUM(M) + N*K) >=0 ]]
		// which in turn means "add K to every M1, M2... and invert the sign:
		// [M1,M2, ...] ---> [-1 *(M1+K), -1*(M2+K), ...]
		// Afterwards, generate a prefix array, where the first element is 0 to properly calculate index distance.
		simplifiedValue = -1 * (token + hc);
		prefixes.push_back(simplifiedValue + prefixes[i]);
		i++;
	}

	// most likely not needed, since the function's destructor calls this implicitly.
	inputFile.close();

	return prefixes;
}

/// <summary>
/// The actual algorithm solver.
/// The main concept is that we need to find the indexes i, j,
/// where j < i and Pi - Pj >= 0 and [i-j] is maximized.
/// To do that, we need to calculate two vectors, LMin and RMax, where:
/// LMin[i] = min(LMin[0], LMin[1], ..., LMin[i]),
/// RMax[j] = max(RMax[j], RMax[j+1], ..., RMax[n-1])
/// and afterwards do a two-index traversal of both of them.
/// Total algorithmic complexity should be O(n).
/// </summary>
int GetMaximumLength(const vector<int>& prefixes)
{
	// short-circuit the execution on best-case because we are O(1)-cool like that.
	if (prefixes.back() >= 0)
	{
		return prefixes.size() - 1;
	}

	int maxDiff, i, j;
	int n = prefixes.size();

	vector<int> validLeft, validRight; // the LMin, RMax vectors.

	// Preemptively reserve the needed space.
	validLeft.reserve(n);
	validRight.reserve(n);

	// Initialize the initial conditions of the vectors.
	validLeft[0] = prefixes[0];
	validRight[n - 1] = prefixes.back();

	j = n - 2;
	// Populate the LMin and RMax vectors.
	for (i = 1; i < n; ++i)
	{
		validLeft[i] = std::min(prefixes[i], validLeft[i - 1]);
		validRight[j] = std::max(prefixes[j], validRight[j + 1]);
		j--;
	}

	// Do a two-pointer traversal to calculate total optimal j-i.
	i = 0, j = 0, maxDiff = -1;
	while (j < n && i < n)
	{
		if (validLeft[i] < validRight[j])
		{
			maxDiff = std::max(maxDiff, j - i);
			j = j + 1;
		}
		else
		{
			i = i + 1;
		}
	}

	return maxDiff;
}
