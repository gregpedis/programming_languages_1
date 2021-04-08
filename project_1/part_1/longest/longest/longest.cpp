#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>

#include <vector>
#include <string>


using std::vector;
using std::string;


// simple container for an important index.
struct ImportantIndex {
public:
	size_t index;
	int prefix;

	ImportantIndex(size_t idx, int prfx) : index(idx), prefix(prfx) { }
};

// simple container struct for the file parameters.
struct InputVals {

private:
	int m_hospital_count;
	vector<int> m_days;

public:
	int hospital_count() const { return m_hospital_count; }
	vector<int> days() const { return m_days; }

	InputVals(int hc, vector<int> ds)
		: m_hospital_count(hc), m_days(std::move(ds))
	{
	}
};


// Function declarations.
// #pragma region DECLARATIONS 

InputVals ParseInputFile(const string& filename);
vector<int> SimplifyInput(const InputVals& inputVals);
vector<int> GeneratePrefixes(const vector<int>& values);

int GetMaximumLength(const vector<int>& prefixes);
int FindLocalOptimalLeftPrefix(const vector<ImportantIndex>& importantIndexes, const int& rightPrefix);

// #pragma endregion


// Execution entrypoint.
// Algorithm reference:
// https://cs.stackexchange.com/questions/129353/find-the-length-of-the-longest-subarray-having-sum-greater-than-k
int main(int argc, char** argv)
{
	// Get the filename from the first command-line argument and parse its contents.
	auto inputVals = ParseInputFile(string(argv[1]));
	// Convert the problem into a simpler one.
	auto values = SimplifyInput(inputVals);

	// Generate the prefix array.
	auto prefixes = GeneratePrefixes(values);
	// Solve the algorithm.
	auto result = GetMaximumLength(prefixes);

	// $$$
	std::cout << result;
	std::cout << std::endl;
}

/// <summary>
/// Read some data from a file.
/// </summary>
InputVals ParseInputFile(const string& filename)
{
	string line1, line2;
	string delim = " ";
	string token;
	size_t pos = 0;

	std::ifstream inputFile(filename); //implicit constructor call.
	std::getline(inputFile, line1);
	std::getline(inputFile, line2);

	pos = line1.find(delim);
	auto dc = std::stoi(line1.substr(0, pos)); // get the days count.
	auto hc = std::stoi(line1.substr(pos)); // get the hospital count.

	vector<int> days;
	for (size_t i = 0; i < dc; i++)
	{
		pos = line2.find(delim);
		token = line2.substr(0, pos);
		days.push_back(std::stoi(token));
		line2.erase(0, pos + delim.length());
	}

	// most likely not needed, since the function's destructor calls this implicitly.
	inputFile.close();

	//days = { 42,-10, 8 };
	//days = { 42, -10, 8, 1, 11, -6, -12, 16, -15, -11, 13 };
	InputVals result(hc, days);
	return result;
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
vector<int> SimplifyInput(const InputVals& inputVals)
{
	vector<int> simplified;

	for (size_t i = 0; i < inputVals.days().size(); i++)
	{
		int newValue = -1 * (inputVals.days()[i] + inputVals.hospital_count());
		simplified.push_back(newValue);
	}

	return simplified;
}

/// <summary>
/// Generate a prefix array, where the first element is 0 to properly calculate index distance.
/// In a normal prefix tree, for example Pn - P0 is the sum of n-1 elements, 
/// whereas the result needed should be n.
/// </summary>
vector<int> GeneratePrefixes(const vector<int>& values)
{
	vector<int> prefixes = { 0 };

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
int GetMaximumLength(const vector<int>& prefixes)
{
	int ans = 0;
	//initialize the first prefix as it is a valid important index by definition.
	vector<ImportantIndex> validLeftPrefixes = { ImportantIndex(0, prefixes[0]) };

	for (size_t i = 0; i < prefixes.size(); i++)
	{
		if (prefixes[i] - validLeftPrefixes.back().prefix >= 0)
		{
			auto j = FindLocalOptimalLeftPrefix(validLeftPrefixes, prefixes[i]);
			ans = std::max(ans, int(i - j));
		}

		if (prefixes[i] < validLeftPrefixes.back().prefix)
		{
			validLeftPrefixes.push_back({ i, prefixes[i] });
		}
	}

	return ans;
}

/// <summary>
/// The binary search of the important index array.
/// </summary>
int FindLocalOptimalLeftPrefix(
	const vector<ImportantIndex>& leftPrefixes, const int& rightPrefix)
{
	// Starting and ending index of search space.
	int l = 0;
	int h = leftPrefixes.size() - 1;
	int m;

	// To store required index value.
	int ans = leftPrefixes[h].index;

	//Binary search loop to find the left-most valid prefix.
	while (l < h) {
		// calculate the midpoint like this 
		// to avoid a possible integer overflow.
		// ref https://en.wikipedia.org/wiki/Binary_search_algorithm#Implementation_issues
		m = l + (h - l) / 2;
		if (rightPrefix - leftPrefixes[m].prefix >= 0) {
			ans = leftPrefixes[m].index;
			h = m - 1;
		}
		else
			l = m + 1;
	}

	return ans;
}

