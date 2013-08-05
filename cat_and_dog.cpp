//qaa12
#include <iostream>
#include <fstream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define M_PI 3.14159265358979323846
#define DEBUG 1
#define DISTR 65.0
#define DISTL 25.0

#define ANGLE4 110
#define ANGLE3 95
#define ANGLE2 85
#define ANGLE1 70


using namespace std;

class Animal{
public:
	double x, y;
	short speed;

	Animal(double x1, double y1, short speed1){
		x = x1;
		y = y1;
		speed = speed1;
	}

	void run(double dir){
		x += speed*cos(dir*M_PI/180.0);
		y += speed*sin(dir*M_PI/180.0);
	}

	double operator-(Animal other){ // dist
		return sqrtf(pow((x-other.x),2) + pow((y-other.y),2));
	}
};

double dist_small(double dist){
	if (dist>DISTR) return 0;
	if (dist>DISTL) return 1-(dist-DISTL)/(DISTR-DISTL);
	return 1;
}

double dist_big(double dist){
	if (dist<DISTL) return 0;
	if (dist<DISTR) return 1-(DISTR-dist)/(DISTR-DISTL);
	return 1;
}

double angle_right(double angle){
	if (angle>ANGLE2) return 0;
	if (angle>ANGLE1) return 1-(angle-ANGLE1)/(ANGLE2-ANGLE1);
	return 1;
}

double angle_left(double angle){
	if (angle<ANGLE3) return 0;
	if (angle<ANGLE4) return 1-(ANGLE4-angle)/(ANGLE4-ANGLE3);
	return 1;
}

double angle_center(double angle){
	if (angle<ANGLE1 || angle > ANGLE4) return 0;
	if (angle<ANGLE2) return 1-(ANGLE2-angle)/(ANGLE2-ANGLE1);
	if (angle>ANGLE3) return 1-(angle-ANGLE3)/(ANGLE4-ANGLE3);
	return 1;
}


double getPl(double *pl, int step){
	double ans;
	if (step >= pl[1] && step <= pl[2]){
		if(step<=pl[2]-pl[1]){
			ans = 1 - ((pl[2]-pl[1])/2.0+pl[1] - step)/((pl[2]-pl[1])/2.0+pl[1]-pl[1]);
		}else{
			ans = 1 - (step - (pl[2]-pl[1])/2.0+pl[1])/(pl[2]-(pl[2]-pl[1])/2.0+pl[1]);
		}
		if (ans > pl[0]) ans = 0;
	}else{
		ans = 0;
	}
	return ans;
}

int get_angle(double dist, double angle){
	double outPl[6][3], sum, max, value, sum2;
	int step, j;
	sum = 0;
	sum2 = 0;
	outPl[0][0] = min(dist_small(dist), angle_left(angle));
	outPl[0][1] = 80;
	outPl[0][2] = 120;
	outPl[1][0] = min(dist_small(dist), angle_center(angle));
	outPl[1][1] = 45;
	outPl[1][2] = 90;
	outPl[2][0] = min(dist_small(dist), angle_right(angle));
	outPl[2][1] = 10;
	outPl[2][2] = 60;
	outPl[3][0] = min(dist_big(dist), angle_left(angle));
	outPl[3][1] = 60;
	outPl[3][2] = 100;
	outPl[4][0] = min(dist_big(dist), angle_center(angle));
	outPl[4][1] = 20;
	outPl[4][2] = 70;
	outPl[5][0] = min(dist_big(dist), angle_right(angle));
	outPl[5][1] = 0;
	outPl[5][2] = 45;
	for(step = 0; step <= 180; step++){
		max = 0;
		for(j = 0; j < 6; j++){
			value = getPl(outPl[j], step);
			if (value>max)	max = value;
		}
		sum += max;
	}

	for(step = 0; step <= 180; step++){
		max = 0;
		for(j = 0; j < 6; j++){
			value = getPl(outPl[j], step);
			if (value>max)	max = value;
		}
		sum2 += max;
		if (sum2>=sum/2.0) break;
	}
	return step;
}

int main()
{
	double cat_dir, dog_dir, dist, angle;
	ofstream fcat("cat");
	ofstream fdog("dog");
	Animal dog(0, 0, 3);
	Animal cat(0, 200, 2);
	srand(time(0));
	int iter = 0;
	int priorAngle = 60;

	while((dist = dog - cat)>dog.speed-cat.speed){
		if ( dog.x <= cat.x){
                  angle = 90 - atan2((fabs(cat.x-dog.x))/dist, (fabs(cat.y - dog.y))/dist)/M_PI*180;
		}else{
                  angle = 90 + atan2((fabs(cat.x-dog.x))/dist, (fabs(cat.y - dog.y))/dist)/M_PI*180;
		}

		if (iter%30==0) priorAngle = rand()%30;
		dog_dir = get_angle(dist, angle);
		if (dog.y>cat.y) dog_dir*=-1;
		cat_dir = rand()%30-priorAngle;

		cat.run(cat_dir);
		dog.run(dog_dir);
		fcat << cat.x << " | " << cat.y << endl;
		fdog << dog.x << " | " << dog.y << endl;
		if(DEBUG){
			cout << "cat = " << cat.x << endl;
			cout << "dog = " << dog.x << endl;
			cout << "dist = " << dog - cat << endl;
			cout << "angle = " << angle << endl;
			cout << "dog_dir = " << dog_dir << endl;
			cout << "cat_dir = " << cat_dir << endl;
		}
		iter++;
	}
	cout << iter << endl;
    return 0;
}
