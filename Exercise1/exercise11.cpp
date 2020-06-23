// author : Sagar Nagaraj Simha
// author : Gabriel De Ioannes

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class Point2D {
public:
    int x;
    int y;
};

int ORI(Point2D p1, Point2D p2,Point2D p3);
string Inter(Point2D a, Point2D b,Point2D p, Point2D q);
string InterPoint(Point2D a, Point2D b,Point2D p, Point2D q);
string GetPointCords(Point2D a);
string word;

int main (){
    string line;
    while(getline(cin,line)){
        if(line == "0 0 0 0 0 0 0 0"){
        	break;
        }
        istringstream ss(line);
        Point2D a,b,p,q;
        ss >> a.x >> a.y >> b.x >> b.y >> p.x >> p.y >> q.x >> q.y;
        cout << Inter(a,b,p,q) << "\n";
        cout.flush();
    }
    return 0;
}

int ORI(Point2D p1, Point2D p2,Point2D p3){
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

string Inter(Point2D a, Point2D b,Point2D p, Point2D q){

    //Cross Intersection
    if( ORI(a,b,p)*ORI(a,b,q)<0 && ORI(p,q,a)*ORI(p,q,b)<0){
        //Get point of Intersection
        return InterPoint(a,b,p,q);
    }

    //Segment intersection
    if( ORI(a,b,p)*ORI(a,b,q)==0 && ORI(p,q,a)*ORI(p,q,b)==0){

        //Segment one line has same x and y in both points return point cord
        if(a.x==b.x && a.y==b.y){
            return GetPointCords(a);
        }

        if(p.x==q.x && p.y==q.y){
            return GetPointCords(q);
        }

        //Segment where lines have a common point
        if(((b.x==p.x && b.y==p.y) || (a.x==p.x && a.y==p.y)) && !((b.x==q.x && b.y==q.y) || (a.x==q.x && a.y==q.y))){
            return GetPointCords(p);
        }

        if(((b.x==q.x && b.y==q.y) || (a.x==q.x && a.y==q.y)) && !((b.x==p.x && b.y==p.y) || (a.x==p.x && a.y==p.y))){
            return GetPointCords(q);
        }

        return "segment intersection";
    }
    return "no intersection";
}

string GetPointCords(Point2D a){
    ostringstream text;
    text << fixed << std::setprecision(2) << (double)(a.x) << " " << std::setprecision(2) << (double)(a.y);
    return text.str();
}

string InterPoint(Point2D a, Point2D b,Point2D p, Point2D q){
    double ua1 = ((q.x-p.x)*(a.y-p.y)-(q.y-p.y)*(a.x-p.x));
    double ua2 = ((q.y-p.y)*(b.x-a.x)-(q.x-p.x)*(b.y-a.y));
    double ua = ua1/ua2;
    double ub1 = ((b.x-a.x)*(a.y-p.y)-(b.y-a.y)*(a.x-p.x));
    double ub2 = ((q.y-p.y)*(b.x-a.x)-(q.x-p.x)*(b.y-a.y));
    double ub = ub1/ub2;

    double x = a.x + ua*(b.x-a.x);
    double y = a.y + ub*(b.y-a.y);

    ostringstream text;
    text << fixed << std::setprecision(2) << x << " " << std::setprecision(2) << y;
    return text.str();
}
