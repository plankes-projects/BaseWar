/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef Achievement_H_
#define Achievement_H_

#include <string>

class Achievement {
public:
	Achievement(std::string id, std::string image, std::string title, std::string description, int points);
	virtual ~Achievement();

	std::string getDescription() const {
		return _description;
	}

	std::string getImage() const {
		return _image;
	}

	int getPoints() const {
		return _points;
	}

	int getTimeAchieved() const {
		return _timeAchieved;
	}

	std::string getTitle() const {
		return _title;
	}

	bool isAchieved() {
		return _timeAchieved != -1;
	}
	bool updateAchievement();
	void setAchievedNow();

protected:
	bool virtual isAchievedInternCalculation() = 0;
	std::string _persistentLoadSaveId;	//needed for storing the timeAchievedValue
	std::string _image;					//the image which belongs to this achievment
	std::string _title; 				//the title which belongs to this achievment
	std::string _description;			//the description which belongs to this achievment
	int _points;						//the points this achievement is worth
	int _timeAchieved;					//the time this achievement was achieved
	int getAchievementAchievedPersistent();
	void setAchievementAchievedPersistent(int time);
};

#endif /* Achievement_H_ */
