// Author      : Gabriel De Ioannes
// author 	   : Sagar Nagaraj Simha

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

class Point2D{
public :
	float x;
	float y;

	Point2D(float x1,float y1){
		x=x1;
		y=y1;
	}

	Point2D(){
		x=0;
		y=0;
	}

	void printPoint(){
		cout << x << " " << y << endl;;
	}
};

struct sort_by_x
{
    inline bool operator() (const Point2D& struct1, const Point2D& struct2)
    {
        return (struct1.x < struct2.x);
    }
};

struct sort_by_y
{
    inline bool operator() (const Point2D& struct1, const Point2D& struct2)
    {
    	if(struct1.y != struct2.y){
    		return (struct1.y < struct2.y);
    	}else{
    		return (struct1.x < struct2.x);
    	}
    }
};

class BoundingBox{
public :

	Point2D a;
	Point2D b;

	BoundingBox(Point2D pa,Point2D pb){
		a=pa;
		b=pb;
	}

	BoundingBox(){

	}

	bool contains(Point2D point){
		//that checks if a point is fully inside this bounding box.
		float maxX,maxY,minX,minY;
		if(a.x>b.x){
			maxX=a.x;
			minX=b.x;
		}else{
			maxX=b.x;
			minX=a.x;
		}
		if(a.y>b.y){
			maxY=a.y;
			minY=b.y;
		}else{
			maxY=b.y;
			minY=a.y;
		}

		if(maxX >= point.x && maxY >= point.y && minX <= point.x && minY <= point.y){
			return true;
		}
		return false;
	}

	bool inside(BoundingBox boundingBox){
		//that checks if the bounding box itself is fully inside
		//another bounding box.
		if(contains(boundingBox.a) && contains(boundingBox.b)){
			return true;
		}
		return false;
	}

	bool intersect(BoundingBox boundingBox){
		//that checks if the bounding box itself intersects
		//with another bounding box.
		if(contains(boundingBox.a) || contains(boundingBox.b) || boundingBox.contains(a) || boundingBox.contains(b)){
			return true;
		}
		return false;
	}

	void printRange(){
		cout << " a:" << " x->"<< a.x << " y->"<< a.y << " b:" << " x->" << b.x << " y->"<< b.y << endl;
	}

};

class KDNode{
public:
	//a flag whether it is an internal node or a leaf node.
	bool leaf;
	//two pointers to its left and right children (in case it is an internal node).
	KDNode * left;
	KDNode * right;
	//The dimension of the splitting line (in case it is an internal node).
	int splitDimension;
	//The position of the splitting line (in case it is an internal node).
	int splitPosition;
	//A (2D) point (in case it is a leaf node).
	Point2D point;
	//A range of type boundingbox that stores its corresponding bounds (and
	//implicitly the bounds of its subtree).
	BoundingBox range;
	//store the number of points
	int count;
	//Debug purpose
	void printPoint(){
		cout << fixed << std::setprecision(1) << " x->"<< point.x << " y->"<< point.y << endl;
	}
};

int quickMedian(vector<Point2D> &p,int dim){
	int k,i,j;
	j=0;
	i=-1;
	k=p.size()/2;
	int pivot=p.size();
	int cutPos=0;
	for(int w=0;w<p.size()-cutPos;w++){
		if(p[j].x>p[pivot].x){
			j++;
			i++;
		}else{
			p[i++]=p[pivot];
			j++;
		}

	}
	return k;
}

class KdTree{
private:
	KDNode* node;
	vector<Point2D> all_leafs;
	int nodeCount;
	//a private pointer to a kdnode (the root node of the kD-tree)
	//a private method reportSubtree(kdnode* n) that returns the set of all
	//(2D) points which are stored in the leaves of the subtree.

	vector<Point2D> reportSubtree(KDNode * n){

		if(n->leaf==true){
			all_leafs.push_back(n->point);
		}
		else{
			reportSubtree(n->left);
			reportSubtree(n->right);
		}
		return all_leafs;
	}

	KDNode* build(vector<Point2D> p,int dim){
			KDNode* newNode=new KDNode;
			if(dim==0){
				node=newNode;
			}


			if(newNode->splitDimension % 2 == 0){
				sort(p.begin(),p.end(),sort_by_x());
			}else{
				sort(p.begin(),p.end(),sort_by_y());
			}

			//increase depth for the next node
			//DEBUG
			//cout << "<-----KNODE----->";
			//newNode->splitDimension=dim;;
			//cout << "DEEP:" << dim << endl;
			dim++;
			//Print Points in each node creation
			//for(int i=0;i<p.size();i++){
			//	cout << " _" << i << "->" << "x:" << p[i].x << " y:" << p[i].y;
			//}
			//cout << endl;

			//Set Range of the bounding box, by now arranging by x and Y to get min and max on each
			//array, this is n(log n) by quick sort
			//Initialize variables
			float minX,maxX,minY,maxY;
			minX=p[0].x;
			maxX=p[0].y;
			minY=p[0].x;
			maxY=p[0].y;
			for(int i=0;i<p.size();i++){
				if(p[i].x<minX){
					minX=p[i].x;
				}
				if(p[i].x>maxX){
					maxX=p[i].x;
				}
				if(p[i].y<minY){
					minY=p[i].y;
				}
				if(p[i].y>maxY){
					maxY=p[i].y;
				}
			}

			newNode->range.a.x=minX;
			newNode->range.a.y=maxY;
			newNode->range.b.x=maxX;
			newNode->range.b.y=minY;

			//DEBUG
			//cout << "RANGE ------>";
			//newNode->range.printRange();
			newNode->count=p.size();
			if(p.size()==1){
				newNode->leaf=true;
				newNode->range.a=p[0];
				newNode->range.b=p[0];
				newNode->point=p[0];
				//DEBUG
				//cout << "-------------------------->Leaf :";
				//p[0].printPoint();
				return newNode;
			}else{
				newNode->leaf=false;
			}

			//AVG to split
			//Sice the array is ordered in every node by X or Y, the median is easy found the size of
			//the array, by default when is odd number gets round in c++, quickMedian is no nessesary in this
			//implementation if we sort the array in very node
			int median=newNode->count/2;
			//DEBUG
			//cout << "MEDIAN:" << median << endl;

			//https://stackoverflow.com/questions/9811235/best-way-to-split-a-vector-into-two-smaller-arrays
			vector<Point2D> split_lo(p.begin(), p.begin() + median);
			vector<Point2D> split_hi(p.begin() + median, p.end());


			if(split_lo.size()>0){
				newNode->left = build(split_lo,dim);
			}
			if(split_hi.size()>0){
				newNode->right = build(split_hi,dim);
			}
			return newNode;
		}

public:
	//a public method build(P) that builds the kD-tree from a set P of (2D) points.
	void build(vector<Point2D> p){
		build(p,0);
	}

	//a public method search(boundingbox range) that performs a range search
	//on the kD-tree and returns a set of (2D) points.
	void search(BoundingBox range,KDNode * sNode){
		if(sNode->leaf){
			if(range.contains(node->point)){
				//cout << "Leaft x->" << sNode->point.x << " y->"<< sNode->point.y << endl;
				all_leafs.push_back(sNode->point);
			}
		}else{
			if(range.inside(sNode->range)){
				//07 ReportSubtree(root.left);
				//cout << "inside report sub tree" << endl;
				reportSubtree(sNode);
			}else{
				if(range.inside(sNode->left->range)){
					//07 ReportSubtree(root.left);
					//cout << "inside report sub tree left" << endl;
					reportSubtree(sNode->left);
				}else if (range.intersect(sNode->left->range)){
					//root.left.bbox intersects R) then
					//SearchkDTree(root.left, R)
					//cout << "intersect Left" << endl;
					search(range,sNode->left);
				}
				if(range.inside(sNode->right->range)){
					//07 ReportSubtree(root.left);
					//cout << "inside report sub tree right" << endl;
					reportSubtree(sNode->right);
				}else if (range.intersect(sNode->right->range)){
					//else if (root.right.bbox intersects R) then
					//SearchkDTree(root.right, R)
					//cout << "intersect right" << endl;
					search(range,sNode->right);
				}
			}
		}
	}

	void count(BoundingBox range,KDNode* sNode){
		if(sNode->leaf){
			if(range.contains(node->point)){
				//cout << "Leaft" << endl;
				nodeCount+= sNode->count;
			}
		}else{
			if(range.inside(sNode->range)){
				//cout << "inside count sub tree" << endl;
				nodeCount+= sNode->count;
			}else{
				if(range.inside(sNode->left->range)){
					//cout << "inside count sub tree left" << endl;
					nodeCount+= sNode->left->count;
				}else if (range.intersect(sNode->left->range)){
					//cout << "intersect Left" << endl;
					count(range,sNode->left);
				}
				if(range.inside(sNode->right->range)){
					//cout << "inside count sub tree right" << endl;
					//cout << sNode->right->count << endl;
					nodeCount+=sNode->right->count;
				}else if (range.intersect(sNode->right->range)){
					//cout << "intersect right" << endl;
					count(range,sNode->right);
				}
			}
		}
	}

	KDNode getNode(){
		return *node;
	}

	vector<Point2D> getAllLeafs(){
		return all_leafs;
	}

	int getNodeCount()
	{
		return nodeCount;
	}

	void search(BoundingBox range){
		all_leafs.clear();
		search(range,node);
	}
	void count(BoundingBox range){
		nodeCount=0;
		count(range,node);
	}

};

void GetPointList(vector<Point2D> &pointList,BoundingBox &bb, string &finalCommand);

int main() {

	vector<Point2D> pointList;
	string finalCommand;
	BoundingBox bb;
	GetPointList(pointList,bb,finalCommand);
	//cout << "Debug_1:" << finalCommand << endl;
	KdTree tree;
	tree.build(pointList);

	if(finalCommand=="PRINT"){
		tree.search(bb);
		vector<Point2D> result=tree.getAllLeafs();
		sort(result.begin(),result.end(),sort_by_y());
		for(int i=0;i<result.size();i++){
			cout << fixed << setprecision(2) << result[i].x << " " << result[i].y << endl;
		}
	}

	if(finalCommand=="COUNT"){
		tree.count(bb);
		cout << fixed << setprecision(0) << tree.getNodeCount() << endl;
	}
}

void GetPointList(vector<Point2D> &pointList,BoundingBox &bb, string &finalCommand){
	string str;
	int num;
	int count=0;
	getline(cin,str);
	istringstream ss(str);
	//Number of points
	ss >> num;
	while(getline(cin,str)){
		if(count<num){
			istringstream ss(str);
			Point2D point_a;
			ss >>  point_a.x >> point_a.y;
			pointList.push_back(point_a);
		}else{
			istringstream ss(str);
			ss >> bb.a.x >> bb.a.y >> bb.b.x >> bb.b.y >> finalCommand;
			break;
		}
		count++;
	}
}
