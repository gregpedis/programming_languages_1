#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>

#include <vector>
#include <array>
#include <string>

using std::vector;
using std::string;

struct Rooms {
public:
	size_t x, y;
	vector<char> rooms;

	Rooms(size_t x, size_t y, vector<char> rooms)
		: x(x), y(y), rooms(std::move(rooms))
	{ }
};

// easier way of reading the coordinates than a pair.
struct RoomPosition {
public:
	size_t x, y;

	RoomPosition(size_t x, size_t y) : x(x), y(y) { }
};

// Function declarations.
// #pragma region DECLARATIONS 

Rooms ParseInputFile(const string& filename);
int GetRowMajorIndex(int i, int j, int n_cols);

int GetInvalidRoomCount(const Rooms& rooms);
vector<RoomPosition> GetValidPerimeterRooms(const Rooms& rooms);
int GetValidRoomCountByRoom(const Rooms& rooms, const RoomPosition& position);

// #pragma endregion


// Execution entrypoint.
int main(int argc, char** argv)
{
	// Get the filename from the first command-line argument and parse its contents.
	auto rooms = ParseInputFile(string(argv[1]));

	// Solve the algorithm.
	auto result = GetInvalidRoomCount(rooms);

	// $$$
	std::cout << result << std::endl;
}

/// <summary>
/// Read some data from a file.
/// </summary>
Rooms ParseInputFile(const string& filename)
{
	std::ifstream inputFile(filename); //implicit constructor call.

	string headerline;
	std::getline(inputFile, headerline);
	auto pos = headerline.find(' ');
	size_t size_x = std::stoi(headerline.substr(0, pos));	// read up until delimiter, convert to int.
	size_t size_y = std::stoi(headerline.substr(pos + 1));	// read remaining string, convert to int.

	vector<char> rooms;
	rooms.reserve(size_x * size_y);

	string line;

	// read all room positions and 
	// convert their value to an instance of the enum RoomDirection.
	for (size_t i = 0; i < size_x; ++i)
	{
		std::getline(inputFile, line);

		for (size_t j = 0; j < size_y; ++j)
		{
			rooms.push_back(line[j]);
		}
	}

	// most likely not needed, since the function's destructor calls this implicitly.
	inputFile.close();

	Rooms res = Rooms(size_x, size_y, rooms);
	return res;
}

int GetRowMajorIndex(int i, int j, int n_cols)
{
	return i * n_cols + j;
}


/// <summary>
/// The actual algorithm solver.
/// It starts by finding the valid perimeter rooms(first and last row, first and last column).
/// Then foreach one of them it executes a Depth-First Traversal to find all the valid rooms.
/// The solution is [AllRooms - ValidRooms], since the question is the number of invalid rooms.
/// </summary>
int GetInvalidRoomCount(const Rooms& rooms)
{
	const auto validRimRooms = GetValidPerimeterRooms(rooms);
	int count = validRimRooms.size();

	for (auto& room : validRimRooms)
	{
		count += GetValidRoomCountByRoom(rooms, room);
	}

	return rooms.x * rooms.y - count;
}

/// <summary>
/// Finds the valid rooms on the perimeter, 
/// meaning the rooms of the first and last row,
/// as well as the rooms of the first and last column
/// which direction is looking "outside" the maze.
/// </summary>
vector<RoomPosition> GetValidPerimeterRooms(const Rooms& rooms)
{
	vector<RoomPosition> validRimRooms;

	for (size_t j = 0; j < rooms.y; ++j)
	{
		if (rooms.rooms[GetRowMajorIndex(0, j, rooms.y)] == 'U')
		{
			const auto newPos = RoomPosition(0, j);
			validRimRooms.push_back(newPos);
		}

		if (rooms.rooms[GetRowMajorIndex(rooms.x - 1, j, rooms.y)] == 'D')
		{
			const auto newPos = RoomPosition(rooms.x - 1, j);
			validRimRooms.push_back(newPos);
		}
	}

	for (size_t i = 0; i < rooms.x; ++i)
	{
		if (rooms.rooms[GetRowMajorIndex(i, 0, rooms.y)] == 'L')
		{
			const auto newPos = RoomPosition(i, 0);
			validRimRooms.push_back(newPos);
		}

		if (rooms.rooms[GetRowMajorIndex(i, rooms.y-1, rooms.y)] == 'R')
		{
			const auto newPos = RoomPosition(i, rooms.y-1);
			validRimRooms.push_back(newPos);
		}
	}

	return validRimRooms;
}

/// <summary>
/// Finds the valid rooms ending in room [position],
/// by executing a relative Depth-First Traversal.
/// </summary>
int GetValidRoomCountByRoom(
	const Rooms& rooms, const RoomPosition& position)
{
	int count = 0;

	if (position.x + 1 < rooms.x
		&& rooms.rooms[GetRowMajorIndex(position.x + 1, position.y, rooms.y)] == 'U')
	{
		const auto newPos = RoomPosition(position.x + 1, position.y);
		count += 1 + GetValidRoomCountByRoom(rooms, newPos);
	}

	if (position.x > 0
		&& rooms.rooms[GetRowMajorIndex(position.x - 1, position.y, rooms.y)] == 'D')
	{
		const auto newPos = RoomPosition(position.x - 1, position.y);
		count += 1 + GetValidRoomCountByRoom(rooms, newPos);
	}

	if ((position.y + 1) < rooms.y
		&& rooms.rooms[GetRowMajorIndex(position.x, position.y + 1, rooms.y)] == 'L')
	{
		const auto newPos = RoomPosition(position.x, position.y + 1);
		count += 1 + GetValidRoomCountByRoom(rooms, newPos);
	}
	if (position.y > 0
		&& rooms.rooms[GetRowMajorIndex(position.x, position.y - 1, rooms.y)] == 'R')
	{
		const auto newPos = RoomPosition(position.x, position.y - 1);
		count += 1 + GetValidRoomCountByRoom(rooms, newPos);
	}

	return count;
}

