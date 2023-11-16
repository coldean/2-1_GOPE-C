#include "std_lib_facilities.h"

void final(string name, int score);

int main()
{
	vector<string> names;
	vector<int> scores;

	string name;
	int score;

	cout << "Input (name, score) pairs: \n";

	while (cin >> name >> score)
	{
		if (name == "NoName" && score == 0)
			break;

		names.push_back(name);
		scores.push_back(score);

	}


	for (int i = 0; i < names.size(); i++)
	{
		for (int j = i + 1; j < names.size(); j++)
		{
			if (names[i] == names[j])
			{
				cout << "\nBad! The name " << '\"' << names[i] << "\" is entered more than once.\n";
				return 1;
			}
		}
	}

	cout << "\nGood! Each name is entered just once.\n\n";

	for (int i = 0; i < names.size(); i++)
		final(names[i], scores[i]);
	
	return 0;
}

void final(string name, int score)
{
	cout << name << " " << score << "\n";
}