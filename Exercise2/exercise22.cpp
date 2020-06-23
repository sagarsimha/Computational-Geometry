// author : Gabriel De Ioannes
// author : Sagar Nagaraj Simha

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

class Point{
public:
	bool pick=false;
	float x=0;
	float y=0;
};

vector<Point> GetPointList();
int ORI(Point p1, Point p2,Point p3);
float PointLineDistance(Point a,Point b,Point c);
vector<Point> DeleteDuplicates(vector<Point> list);
vector<int> DeleteDuplicatesInt(vector<int> list);
bool ComparePoints(Point a, Point b);
int GetBottomLeft(vector<Point> list);
bool CompareInterval(int i1, int i2);
vector<Point> DeleteColinearPoints(vector<Point> list);

//Operation counter just for fun to see the number of operation and the reductions
//using the reduced list in every iteration

int xMin;
int yMin;


int main() {
	vector<Point> points = GetPointList();

	vector<Point> hull;
	Point point;
	bool endHull=false;
	int hullCounter=0;

	vector<Point> savePointList=points;

	while(savePointList.size()>2){

		++hullCounter;
		hull.clear();

		vector<Point> savePoints;

			vector<Point> hull;
			vector<int> hullPointdeleteOrder;
			int numberBeginHull=4;
			Point point;

			for(int i=0;i<numberBeginHull;i++){
				hull.push_back(point);
				hullPointdeleteOrder.push_back(0);
			}
			//Pick extremes point of the Hull
			//The active thing by now is to avoid adding point with default 0 in the integer
			//when the object is initialized
			for(int i=0;i<points.size();i++){
				if(points[i].x > hull[3].x || !hull[3].pick){
					hull[3]=points[i];
					hullPointdeleteOrder[3]=i;
					hull[3].pick=true;
				}
				if(points[i].y > hull[2].y || !hull[2].pick){
					hull[2]=points[i];
					hullPointdeleteOrder[2]=i;
					hull[2].pick=true;
				}
				if(points[i].x < hull[1].x || !hull[1].pick){
					hull[1]=points[i];
					hullPointdeleteOrder[1]=i;
					hull[1].pick=true;
					xMin=points[i].x;
				}
				if(points[i].y < hull[0].y || !hull[0].pick){
					hull[0]=points[i];
					hullPointdeleteOrder[0]=i;
					hull[0].pick=true;
					yMin=points[i].y;
				}
			}

			sort(hullPointdeleteOrder.begin(), hullPointdeleteOrder.end(), CompareInterval);

			//Delete duplicated points
			hull=DeleteDuplicates(hull);
			//Delete hull points from list
			hullPointdeleteOrder=DeleteDuplicatesInt(hullPointdeleteOrder);
			for(int i=0;i<hullPointdeleteOrder.size();i++){
				points.erase(points.begin()+hullPointdeleteOrder[i]);
			}

			//Counter to keep track of the place in the list of the hull to add and move points
			int count=hull.size()-1;

			bool check=true;
			int a=0;
			int b=0;
			//This while runs the logic of Quick Hull until doen't find more points between lines
			while(true){
				//When the counter is 0, we need to compare the last line which is  the element of the
				//hull with the first ones
				if(count==0){
					a=0;
					b=hull.size()-1;
				}else{
					a=count;
					b=count-1;
				}

				//Cover all the points in reference to the n points of the hull increasing over iteration
				int index=-1;
				float dist=-1;
				for(int i=0;i<points.size();i++){
					if(ORI(hull[a],hull[b],points[i])<=0){
						if(PointLineDistance(hull[a],hull[b],points[i]) >= dist){
							index=savePoints.size();
							dist=PointLineDistance(hull[a],hull[b],points[i]);
						}
						savePoints.push_back(Point(points[i]));
						//I'm adding this because maybe there is special cases in big random clouds
						//where one side share points of the other, in those cases may add point in both sides
						//and the algorithm never stops this way we delete the point after finding they belong to
						//a specific hull side
						points.erase(points.begin()+i);
						i--;
					}
				}

				if(index!=-1){
					//Add the point to the hull and delete it from the saved points
					hull.insert(hull.begin()+a,savePoints[index]);
					savePoints.erase(savePoints.begin()+index);
					check=false;
				}

				//Reduce the counters to read the next segment of the hull in the iteration
				count--;

				//If the counter is -1 means that we read the the segments of the quick Hull
				if(count==-1){
					if(check){
						break;
					}else{
						check=true;
						//Reset counter to start to read again from the beginning hull with new points
						count=hull.size()-1;
						//Saved points are the new points to compare with the hull
						//a cool optimization would be to only compare sides of the hull with prior points to compare
						points=savePoints;
						savePoints.clear();
					}
				}
			}
			//For efficiency delete duplicates at the end
			hull=DeleteDuplicates(hull);

		//Delete use points
		for(int i=0;i<hull.size();i++){
			for(int k=0;k<savePointList.size();k++){
				if(ComparePoints(hull[i],savePointList[k])){
					savePointList.erase(savePointList.begin()+k);
					i--;
				}
			}
		}

		hull=DeleteColinearPoints(hull);
		int hullIndex=GetBottomLeft(hull);
		//Write hull info
		if(hull.size()>=3){
			cout << hull.size() << endl;
			int i=hullIndex;

			//This while write the points from the bottom left CW reading the array that way
			while(true){
				cout << hull[i].x << " " << hull[i].y << endl;
				i++;
				if(i>=hull.size()){
							i=0;
						}
						if(i==hullIndex){
							break;
						}
			}
		}

		points=savePointList;
	}
}

vector<Point> DeleteColinearPoints(vector<Point> list){
	int left=0;
	int right=0;
	int count=0;
	bool deleteCenter=false;
	while(true){
		if(list.size()<3){
			break;
		}
		left=count-1;
		right=count+1;
		if(left<0){
			left=list.size()-1;
		}
		if(right>list.size()-1){
			right=0;
		}
		if(ORI(list[left],list[right],list[count])==0){
			list.erase(list.begin()+count);
			count--;
		}

		count++;
		if(count>=list.size()){
			break;
		}
	}
	return list;
}

bool CompareInterval(int i1, int i2)
{
    return (i1 > i2);
}

int GetBottomLeft(vector<Point> list){
	float saveDist=10000000;
	float saveY=10000000;
	int index;
	for(int i=0;i<list.size();i++){
		float cat1=list[i].x-xMin;
		float cat2=list[i].y-yMin;
		float dist=sqrt(pow(cat1,2)+pow(cat2,2));
		if(dist<saveDist){
			if(list[i].y<=saveY){
				saveDist=dist;
				saveY=list[i].y;
				index=i;
			}
		}

	}
	return index;
}

vector<Point> DeleteDuplicates(vector<Point> list){
	vector<Point> newList;
	vector<Point> eraseList=vector<Point>(list);
	bool flag=false;
	while(eraseList.size()>=1){
		Point pCompare=eraseList[0];
		for(int k=eraseList.size()-1;k>=0;k--){
			//Start from the last
			if(ComparePoints(pCompare,eraseList[k])){
				if(!flag){
					newList.push_back(pCompare);
				}
				flag=true;
				eraseList.erase(eraseList.begin()+k);
			}
		}
		flag=false;
		list=vector<Point>(eraseList);
	}
	return newList;
}

vector<int> DeleteDuplicatesInt(vector<int> list){
	vector<int> newList;
	vector<int> eraseList=vector<int>(list);
	bool flag=false;
	while(eraseList.size()>=1){
		int pCompare=eraseList[0];
		for(int k=eraseList.size()-1;k>=0;k--){
			//Start from the last
			if(pCompare==eraseList[k]){
				if(!flag){
					newList.push_back(pCompare);
				}
				flag=true;
				eraseList.erase(eraseList.begin()+k);
			}
		}
		flag=false;
		list=vector<int>(eraseList);
	}
	return newList;
}

vector<Point> GetPointList(){
	string str;
	int num;
	int count=0;
	getline(cin,str);
	istringstream ss(str);
	//Number of points
	ss >> num;
	//List of Points
	vector<Point> points;

	while(getline(cin,str)){
		istringstream ss(str);
		Point point;
		ss >> point.x >> point.y;
		points.push_back(point);
		count++;
		if(count==num){
			break;
		}
	}
	return points;
}

float PointLineDistance(Point a, Point b,Point c){

	float s1= sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
	float s2=sqrt(pow(a.x-c.x,2)+pow(a.y-c.y,2));
	float s3=sqrt(pow(b.x-c.x,2)+pow(b.y-c.y,2));
	float s=(s1+s2+s3)/2;
	float area= sqrt(s*(s-s1)*(s-s2)*(s-s3));
	float h=area/(0.5*s1);
	return h;
}

bool ComparePoints(Point a, Point b){
	return a.x == b.x && a.y==b.y;
}

int ORI(Point p1, Point p2,Point p3){
	double dx21 = p2.x - p1.x;
	double dy21 = p2.y - p1.y;
	double dx31 = p3.x - p1.x;
	double dy31 = p3.y - p1.y;
	if (dx21*dy31 < dy21*dx31) return -1; // ccw
	if (dx21*dy31 > dy21*dx31) return +1; // cw
	if (dx21*dx31 < 0 || dy21*dy31<0) return -1; // ccw
	if (dx31*dx31 + dy31*dy31 > dx21*dx21 + dy21*dy21) return +1; //cw
	return 0; // p3 is inside p1,p2
}
