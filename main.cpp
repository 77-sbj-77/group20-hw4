#include <iostream>
#include "bowling_frame.h"
#include <random>

//for atoi//
#include <cstdlib>

using namespace std;

std::random_device dev_main;
std::mt19937 rng_main(dev_main());

int current_game_total(Bowling_Frame * first_frame)
{
	Bowling_Frame * temp = first_frame;
	int game_total = 0;
	int temp_total = 0;
	while(temp)
	{
		game_total += temp->frame_score();
		temp = temp->next_frame();
	}
	return game_total;	
}

//added command-line arguments//
int main(int argc, char *argv[])
{
    //throws error if user doesn't provide # of players//
    if (argc != 2) 
    {
        cout << "An error occurred. Please provide the number of players." << endl;
        return 1;
    }

    //coverts argument to an integer//
    int num_players = atoi(argv[1]);

    //throws error if user provides invalid # of players//
    if (num_players <= 0)
    {
        cout << "An error occurred. Please provide a valid number of players." << endl;
        return 1;
    }

    //creates array of Bowling_Frame pointers for each player//
    Bowling_Frame ** players = new Bowling_Frame*[num_players];

    //creates frames for each player//
    for (int i = 0; i < num_players; i++)
    {
        //first frame//
        players[i] = new Bowling_Frame(1);

        //current frame//
        Bowling_Frame * current = players[i];

        //remaining 9 frames//
        for (int j = 2; j <= 10; j++)
        {
            //creates next frame//
            Bowling_Frame * next = new Bowling_Frame(j);

            //link frames//
            current->next_frame(next);
            next->prev_frame(current);

            //move to next frame//
            current = next;
        }
    }

    //populate frames with scores and update scores//
    for (int i = 0; i < num_players; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            players[i]->update_scores();
        }
    }

    //print everything//
    cout << "Player count is: " << num_players << endl;

    for (int i = 0; i < num_players; i++)
    {
        cout << "\nPlayer " << i + 1 << " Stats:" << endl;

        //creates pointer to traverse frames//
        Bowling_Frame * traverse = players[i];

        while (traverse)
        {
            cout << "Frame " << traverse->frame_num() << ":" << endl;
            cout << " Score 1: " << traverse->score1() << endl;
            cout << " Score 2: " << traverse->score2() << endl;

            //adds third score for 10th frame if needed//
            if (traverse->score3() > 0)
            {
                cout << " Score 3: " << traverse->score3() << endl;
            }

            //moves to next frame//
            traverse = traverse->next_frame();
        }

        cout << endl;

        cout << "Total Game Score for player " << i + 1 << ": " << current_game_total(players[i]) << endl;

        cout << endl;
    }

    //OLD CODE//
    /*
	//make frames
	Bowling_Frame * f1 = new Bowling_Frame(1);
	Bowling_Frame * f2 = new Bowling_Frame(2);
	Bowling_Frame * f3 = new Bowling_Frame(3);

	//link frames together
	f1->next_frame(f2);

	f2->next_frame(f3);
	f2->prev_frame(f1);

	f3->prev_frame(f2);	

	//set frame scores	
	f1->update_scores();
	f1->update_scores();
	cout << "Frame 1 Total: " << current_game_total(f1) << endl;

	f1->update_scores();
	f1->update_scores();
	cout << "Frame 1 and 2 Total: " << current_game_total(f1) << endl;


	f1->update_scores();
	f1->update_scores();
	cout << "Frame 1,2,and 3 Total: " << current_game_total(f1) << endl;

	//print values
	cout << "Frame " << f1->frame_num() << ": " << endl
		 << " Score 1: " << f1->score1() << endl
		 << " Score 2: " << f1->score2() << endl;

	cout << "Frame " << f2->frame_num() << ": " << endl
	     << " Score 1: " << f2->score1() << endl
		 << " Score 2: " << f2->score2() << endl;

	cout << "Frame " << f3->frame_num() << ": " << endl
	     << " Score 1: " << f3->score1() << endl
		 << " Score 2: " << f3->score2() << endl;

	//iterate through linked list and print values
	Bowling_Frame * traverse = f1;
	
	while(traverse)
	{
		cout << "Frame " << traverse->frame_num() << ": " << endl
		 	 << " Score 1: " << traverse->score1() << endl
		 	 << " Score 2: " << traverse->score2() << endl;

		traverse = traverse->next_frame();
	}
    */

}