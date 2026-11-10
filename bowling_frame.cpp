#include "bowling_frame.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

Bowling_Frame::Bowling_Frame(int fnum)
{
	score1_ = 0;
	score2_ = 0;
	score3_ = 0;
	strike_ = false;
	spare_ = false;
	roll_one_ = false;
	roll_two_ = false;
	roll_three_ = false;
	total_score_ = 0;
	frame_num_ = fnum;
	frame_score_ = 0;

	next_frame_ = NULL;
	prev_frame_ = NULL;
}

Bowling_Frame::~Bowling_Frame()
{
	free(next_frame_);
	free(prev_frame_);
}

Bowling_Frame* Bowling_Frame::next_frame() { return next_frame_; }
void Bowling_Frame::next_frame(Bowling_Frame * new_frame) { next_frame_ = new_frame; }

Bowling_Frame* Bowling_Frame::prev_frame() { return prev_frame_; }
void Bowling_Frame::prev_frame(Bowling_Frame * new_frame) { prev_frame_ = new_frame; }

int Bowling_Frame::score1() { return score1_; }
void Bowling_Frame::score1(int score) { score1_ = score; }

int Bowling_Frame::score2() { return score2_; }
void Bowling_Frame::score2(int score) { score2_ = score; }

int Bowling_Frame::score3() { return score3_; }
void Bowling_Frame::score3(int score) { score3_ = score; }

bool Bowling_Frame::strike() { return strike_; }
void Bowling_Frame::strike(bool value) { strike_ = value; }

bool Bowling_Frame::spare() { return spare_; }
void Bowling_Frame::spare(bool value) { spare_ = value; }

int Bowling_Frame::frame_num()
{
	return frame_num_;
}

int Bowling_Frame::frame_score()
{
	return frame_score_;
}

void Bowling_Frame::update_scores()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 10);

	if (frame_num_ < 10 && (strike_ || roll_two_))
	{
		if (next_frame_)
			next_frame_->update_scores();
	}
	else if (frame_num_ == 10 &&
		(roll_three_ || (roll_two_ && !strike_ && !spare_)))
	{
		return;
	}
	else if (!roll_one_)
	{
		score1_ = dist10(rng);
		roll_one_ = true;

		if (score1_ == 10)
			strike_ = true;
	}
	else if (!roll_two_)
	{
		int max_score = 10 - score1_;

		if (frame_num_ == 10 && strike_)
			max_score = 10;

		std::uniform_int_distribution<std::mt19937::result_type> distX(0, max_score);

		score2_ = distX(rng);
		roll_two_ = true;

		if (score1_ + score2_ == 10 && !strike_)
			spare_ = true;
	}
	else if (frame_num_ == 10 && (strike_ || spare_) && !roll_three_)
	{
		int max_score = 10;

		if (strike_ && score2_ != 10)
			max_score = 10 - score2_;

		std::uniform_int_distribution<std::mt19937::result_type> distX(0, max_score);

		score3_ = distX(rng);
		roll_three_ = true;
	}

	// Update the score for this frame and previous frames.
	Bowling_Frame * current = this;

	while (current)
	{
		current->frame_score_ = current->score1_ + current->score2_ + current->score3_;

		if (current->frame_num_ < 10 && current->strike_)
		{
			Bowling_Frame * next = current->next_frame_;

			if (next && next->roll_one_)
			{
				current->frame_score_ = 10 + next->score1_;

				if (next->frame_num_ == 10)
				{
					if (next->roll_two_)
						current->frame_score_ += next->score2_;
				}
				else if (next->strike_)
				{
					Bowling_Frame * after_next = next->next_frame_;

					if (after_next && after_next->roll_one_)
						current->frame_score_ += after_next->score1_;
				}
				else if (next->roll_two_)
				{
					current->frame_score_ += next->score2_;
				}
			}
		}
		else if (current->frame_num_ < 10 && current->spare_)
		{
			Bowling_Frame * next = current->next_frame_;

			if (next && next->roll_one_)
				current->frame_score_ = 10 + next->score1_;
			else
				current->frame_score_ = 10;
		}

		current = current->prev_frame_;
	}
}
