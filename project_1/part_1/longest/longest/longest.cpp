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
	int index;
	int prefix;

	ImportantIndex(int idx, int prfx) : index(idx), prefix(prfx) { }
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
vector<int> GenerateSimplifiedPrefixes(const InputVals& inputVals);
int GetMaximumLength(const vector<int>& prefixes);

// #pragma endregion


// Execution entrypoint.
// Algorithm reference:
// https://cs.stackexchange.com/questions/129353/find-the-length-of-the-longest-subarray-having-sum-greater-than-k
int main(int argc, char** argv)
{
	// Get the filename from the first command-line argument and parse its contents.
	auto inputVals = ParseInputFile(string(argv[1]));

	//// Convert the problem into a simpler one.
	//auto values = SimplifyInput(inputVals);
	//// Generate the prefix array.
	//auto prefixes = GeneratePrefixes(values);

	// Convert the problem into a simpler one and generat the prefix array in one step.
	auto prefixes = GenerateSimplifiedPrefixes(inputVals);

	// Solve the algorithm.
	auto result = GetMaximumLength(prefixes);

	// $$$
	std::cout << result << std::endl;
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
	size_t dc = std::stoi(line1.substr(0, pos)); // get the days count.
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
/// Afterwards, generate a prefix array, where the first element is 0 to properly calculate index distance.
/// In a normal prefix tree, for example Pn - P0 is the sum of n-1 elements, 
/// whereas the result needed should be n.
/// </summary>
vector<int> GenerateSimplifiedPrefixes(const InputVals& inputVals)
{
	vector<int> prefixes = { 0 };

	for (size_t i = 0; i < inputVals.days().size(); i++)
	{
		int newValue = -1 * (inputVals.days()[i] + inputVals.hospital_count());
		prefixes.push_back(prefixes[i] + newValue);
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
	if (prefixes.back() >= 0)
	{
		return prefixes.size() - 1;
	}

	int maxDiff, i, j;
	int n = prefixes.size();

	const int max_n = 500000;

	int* LMin = (int*)malloc(max_n * sizeof(int));
	int* RMax= (int*)malloc(max_n * sizeof(int));

	if (!LMin || !RMax)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}

	LMin[0] = prefixes[0];
	RMax[n - 1] = prefixes.back();

	for (i = 1; i < n; ++i)
	{
		LMin[i] = std::min(prefixes[i], LMin[i - 1]);
	}

	// Construct RMax[] such that RMax[j]
	// stores the maximum value
	// from (arr[j], arr[j+1], ..arr[n-1])
	for (j = n - 2; j >= 0; --j)
	{
		RMax[j] = std::max(prefixes[j], RMax[j + 1]);
	}

	// Traverse both arrays from left to right
	// to find optimum j - i
	// This process is similar to merge()
	// of MergeSort
	i = 0, j = 0, maxDiff = -1;
	while (j < n && i < n)
	{
		if (LMin[i] < RMax[j])
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

	/*vector<ImportantIndex> validLeft = { ImportantIndex(0, prefixes[0]) };
	vector<ImportantIndex> validRight = { ImportantIndex(prefixes.size() - 1, prefixes.back()) };

	int i = 1;
	int j = prefixes.size() - 2;
	int n = int(prefixes.size());

	for (i = 1; i < n; ++i)
	{
		if (prefixes[i] < validLeft.back().prefix)
		{
			validLeft.push_back({ i, prefixes[i] });
		}

		if (prefixes[j] > validRight.back().prefix)
		{
			validRight.push_back({ j, prefixes[j] });
		}

		--j;
	}

	int n_left = int(validLeft.size());
	int n_right = int(validRight.size());
	i = 0;
	j = n_right - 1;

	int ans = 0;

	while (i < n_left && j >= 0)
	{
		if (validLeft[i].prefix <= validRight[j].prefix
			&& validLeft[i].index <= validRight[j].index)
		{
			ans = std::max(ans, validRight[j].index - validLeft[i].index);
			--j;
		}
		else
		{
			++i;
		}
	}

	return ans;*/
}

