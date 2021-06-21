#include <vector>

using namespace std;

class map
{
public:
	map();
	void calculateMap(int offSet, int difficulty);
	bool checkColisions(int rocketX, int rocketY, int index, int scrollingOfSetMap, int rocketWidth, int difficult);
	std::vector<vector<int>> getMap();
	int getTotalLength();

private:
	std::vector<vector<int>> mapValues;
	int totalLength;
	int actualHeight;
};
