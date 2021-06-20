#include <vector>

using namespace std;

class map
{
public:
	map();
	void calculateMap(int offSet);
	bool checkColisions(int rocketX, int rocketY, int index, int scrollingOfSetMap, int rocketWidth);
	std::vector<vector<int>> getMap();
	int getTotalLength();

private:
	std::vector<vector<int>> mapValues;
	int totalLength;
	int actualHeight;
};
