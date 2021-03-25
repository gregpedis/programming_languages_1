#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>

#include <vector>
#include <array>
#include <string>

// simple container struct for the file parameters.
struct InputVals {
	int hospital_count;
	std::vector<int> days;

	InputVals() { hospital_count = 0; };
};

// Function declarations.
#pragma region DECLARATIONS 

InputVals ParseInputFile(std::string filename);
std::vector<int> SimplifyInput(InputVals& inputVals);
std::vector<int> GeneratePrefixes(std::vector<int>& values);

int GetMaximumLength(std::vector<int>& prefixes);
int FindLocalOptimalLeftPrefix(std::vector<std::pair<int, int>>& importantIndexes, int rightPrefix);

#pragma endregion


// Execution entrypoint.
// Algorithm reference:
// https://cs.stackexchange.com/questions/129353/find-the-length-of-the-longest-subarray-having-sum-greater-than-k
int main(int argc, char** argv)
{
	// Get the filename from the first command-line argument and parse its contents.
	InputVals inputVals = ParseInputFile(std::string(argv[1]));
	// Convert the problem into a simpler one.
	std::vector<int> values = SimplifyInput(inputVals);

	// Generate the prefix array.
	std::vector<int> prefixes = GeneratePrefixes(values);
	// Solve the algorithm.
	int result = GetMaximumLength(prefixes);
	// $$$
	std::cout << result;
}

/// <summary>
/// Read some data from a file.
/// </summary>
InputVals ParseInputFile(std::string filename)
{
	InputVals inputVals;

	std::string line1, line2;
	std::string delim = " ";
	std::string token;
	size_t pos = 0;

	std::ifstream inputFile(filename); //implicit constructor call.
	std::getline(inputFile, line1);
	std::getline(inputFile, line2);

	pos = line1.find(delim);
	inputVals.hospital_count = std::stoi(line1.substr(pos));

	while ((pos = line2.find(delim)) != std::string::npos)
	{
		token = line2.substr(0, pos);
		inputVals.days.push_back(std::stoi(token));
		line2.erase(0, pos + delim.length());
	}

	inputFile.close();

	//	inputVals.days = { 42,-10, 8 };
	//	inputVals.days = { 42, -10, 8, 1, 11, -6, -12, 16, -15, -11, 13 };
	return inputVals;
}

/// <summary>
/// This is kinda wonky, but the gist of it is our problem is solved by the metric: 
/// [[ SUM( M )/( N*K ) <= -1 ]]
/// where M : a sub-array, N: length of M, K: a constant.
/// By moving around some math, it is trivial to restructure it as:
/// [[ -1 * (SUM(M) + N*K) >=0 ]]
/// which in turn means "add K to every M1, M2... and invert the sign:
/// [M1,M2, ...] ---> [-1 *(M1+K), -1*(M2+K), ...]
/// </summary>
std::vector<int> SimplifyInput(InputVals& inputVals)
{
	std::vector<int> simplified;

	for (size_t i = 0; i < inputVals.days.size(); i++)
	{
		int newValue = -1 * (inputVals.days[i] + inputVals.hospital_count);
		simplified.push_back(newValue);
	}
	return simplified;
}

/// <summary>
/// Generate a prefix array, where the first element is 0 to properly calculate index distance.
/// In a normal prefix tree, for example Pn - P0 is the sum of n-1 elements, 
/// whereas the result needed should be n.
/// </summary>
std::vector<int> GeneratePrefixes(std::vector<int>& values)
{
	std::vector<int> prefixes = { 0 };
	//prefixes.push_back(values[0]);

	for (size_t i = 0; i < values.size(); i++)
	{
		prefixes.push_back(prefixes[i] + values[i]);
	}
	return prefixes;
}

/// <summary>
/// The actual algorithm solver.
/// The main concept is that we need to find the indexes i, j,
/// where j < i and Pi - Pj >= 0 and [i-j] is maximized.
/// In order to find the possible J values, 
/// it is needed to search for a decreasing sub-sequence of the prefix array important indexes,
/// since if j2 > j1 and Pj2 > Pj1, Pj2 is always a worse solution than Pj1 [any distance i-j2 will be shorter than i-j1],
/// hence j2 is not an important index.
/// After that a binary search can be executed [since important index array is sorted, decreasing] 
/// to find the left-most viable index, which will be optimal [since important index array is sorted, again].
/// </summary>
int GetMaximumLength(std::vector<int>& prefixes)
{
	int ans = 0;
	//initialize the first prefix as it is a valid important index by definition.
	std::vector<std::pair<int, int>> validLeftPrefixes = { {0,prefixes[0]} };

	for (size_t i = 0; i < prefixes.size(); i++)
	{
		if (prefixes[i] - validLeftPrefixes.back().second >= 0)
		{
			int j = FindLocalOptimalLeftPrefix(validLeftPrefixes, prefixes[i]);
			ans = std::max(ans, int(i - j));
		}

		if (prefixes[i] < validLeftPrefixes.back().second)
		{
			validLeftPrefixes.push_back({ i, prefixes[i] });
		}
	}

	return ans;
}

/// <summary>
/// The binary search of the important index array.
/// </summary>
int FindLocalOptimalLeftPrefix(std::vector<std::pair<int, int>>& leftPrefixes, int rightPrefix)
{
	// Starting and ending index of search space.
	int l = 0;
	int h = leftPrefixes.size() - 1;
	int m;

	// To store required index value.
	int ans = leftPrefixes[h].first;

	//Binary search loop to find the left-most valid prefix.
	while (l < h) {
		// calculate the midpoint like this 
		// to avoid a possible integer overflow.
		// ref https://en.wikipedia.org/wiki/Binary_search_algorithm#Implementation_issues
		m = l + (h - l) / 2;
		if (rightPrefix - leftPrefixes[m].second >= 0) {
			ans = leftPrefixes[m].first;
			h = m - 1;
		}
		else
			l = m + 1;
	}

	return ans;
}

