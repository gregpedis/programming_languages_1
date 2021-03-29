#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>

#include <vector>
#include <array>
#include <string>

using std::vector;
using std::string;
using std::pair;

// easier to compare than chars.
enum class RoomDirection { Up, Down, Left, Right };

// easier way of reading the coordinates than a pair.
struct RoomPosition {
	size_t x, y;
	RoomPosition(size_t x, size_t y) : x(x), y(y) {}
};

// simple container struct for the file parameters.
struct InputVals {
private:
	size_t m_size_x;
	size_t m_size_y;
	vector <vector<RoomDirection>> m_rooms;

public:
	size_t size_x() const { return m_size_x; }
	size_t size_y() const { return m_size_y; }
	vector<vector<RoomDirection>> rooms() const { return m_rooms; }

	InputVals(size_t x, size_t y, vector<vector<RoomDirection>> rooms)
		: m_size_x(x), m_size_y(y), m_rooms(std::move(rooms))
	{
	}
};

// Function declarations.
// #pragma region DECLARATIONS 

InputVals ParseInputFile(const string& filename);
RoomDirection GetRoomValue(const char& room);

int GetInvalidRoomCount(const InputVals& inputVals);
vector<RoomPosition> GetValidRimRooms(const InputVals& inputVals);
int GetValidRoomCountByRoom(const InputVals& inputVals, const RoomPosition& position);

// #pragma endregion

// Execution entrypoint.
int main(int argc, char** argv)
{
	// Get the filename from the first command-line argument and parse its contents.
	auto inputVals = ParseInputFile(string(argv[1]));

	// Solve the algorithm.
	auto result = GetInvalidRoomCount(inputVals);

	// $$$
	std::cout << result;
	std::cout << std::endl;
}

InputVals ParseInputFile(const string& filename)
{
	std::ifstream inputFile(filename); //implicit constructor call.

	string headerline;
	std::getline(inputFile, headerline);
	auto pos = headerline.find(' ');
	size_t size_x = std::stoi(headerline.substr(0, pos));
	size_t size_y = std::stoi(headerline.substr(pos + 1));

	vector<vector<RoomDirection>> rooms;
	string line;

	for (size_t i = 0; i < size_x; ++i)
	{
		std::getline(inputFile, line);
		vector<RoomDirection> roomRow;

		for (size_t j = 0; j < size_y; ++j)
		{
			auto roomValue = GetRoomValue(line[j]);
			roomRow.push_back(roomValue);
		}

		rooms.push_back(roomRow);
	}

	// most likely not needed, since the function's destructor calls this implicitly.
	inputFile.close();

	InputVals result(size_x, size_y, rooms);
	return result;
}

RoomDirection GetRoomValue(const char& room)
{
	switch (room)
	{
	case 'U':
		return RoomDirection::Up;
	case 'D':
		return RoomDirection::Down;
	case 'L':
		return RoomDirection::Left;
	case 'R':
  default:
		return RoomDirection::Right;
	}
}

int GetInvalidRoomCount(const InputVals& inputVals)
{
	const auto validRimRooms = GetValidRimRooms(inputVals);
	int count = validRimRooms.size();

	for (auto& room : validRimRooms)
	{
		count += GetValidRoomCountByRoom(inputVals, room);
	}

	return inputVals.size_x() * inputVals.size_y() - count;
}

vector<RoomPosition> GetValidRimRooms(const InputVals& inputVals)
{
	vector<RoomPosition> validRimRooms;

	size_t max_x = inputVals.size_x() - 1;
	size_t max_y = inputVals.size_y() - 1;

	for (size_t j = 0; j <= max_y; ++j)
	{
		if (inputVals.rooms()[0][j] == RoomDirection::Up)
		{
			const auto newPos = RoomPosition(0, j);
			validRimRooms.push_back(newPos);
		}

		if (inputVals.rooms()[max_x][j] == RoomDirection::Down)
		{
			const auto newPos = RoomPosition(max_x, j);
			validRimRooms.push_back(newPos);
		}
	}

	for (size_t i = 0; i <= max_x; ++i)
	{
		if (inputVals.rooms()[i][0] == RoomDirection::Left)
		{
			const auto newPos = RoomPosition(i, 0);
			validRimRooms.push_back(newPos);
		}

		if (inputVals.rooms()[i][max_y] == RoomDirection::Right)
		{
			const auto newPos = RoomPosition(i, max_y);
			validRimRooms.push_back(newPos);
		}
	}

	return validRimRooms;
}

int GetValidRoomCountByRoom(const InputVals& inputVals, const RoomPosition& position)
{
	int count = 0;

	if (position.x + 1 < inputVals.size_x()
		&& inputVals.rooms()[position.x + 1][position.y] == RoomDirection::Up)
	{
		const auto newPos = RoomPosition(position.x + 1, position.y);
		count += 1 + GetValidRoomCountByRoom(inputVals, newPos);
	}

	if (position.x > 0)
	{
		if (inputVals.rooms()[position.x - 1][position.y] == RoomDirection::Down)
		{
			const auto newPos = RoomPosition(position.x - 1, position.y);
			count += 1 + GetValidRoomCountByRoom(inputVals, newPos);
		}
	}

	if ((position.y + 1) < inputVals.size_y()
		&& inputVals.rooms()[position.x][position.y + 1] == RoomDirection::Left)
	{
		const auto newPos = RoomPosition(position.x, position.y + 1);
		count += 1 + GetValidRoomCountByRoom(inputVals, newPos);
	}
	if (position.y > 0
		&& inputVals.rooms()[position.x][position.y - 1] == RoomDirection::Right)
	{
		const auto newPos = RoomPosition(position.x, position.y - 1);
		count += 1 + GetValidRoomCountByRoom(inputVals, newPos);
	}

	return count;
}

