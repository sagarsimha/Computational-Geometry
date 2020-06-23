// author : Sagar Nagaraj Simha
// author : Gabriel De Ioannes

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;



public class triangle {
	public static void main(String[] args) {
		
	int screen_height = 20;
	int screen_width = 20;
	
	//Reds input from console	
	ArrayList<String> input = ReadConsole();
	
	for (int i = 0; i < input.size(); i++) {
		
		// Converts each input line from the Array list into a list of elements delimited by whitespace, to access the coordinates individually
		List<String> coordinates = Arrays.asList(input.get(i).split("\\s+"));
		
		Point_o a = new Point_o(Integer.valueOf(coordinates.get(0)),Integer.valueOf(coordinates.get(1)));
		Point_o b = new Point_o(Integer.valueOf(coordinates.get(2)),Integer.valueOf(coordinates.get(3)));
		Point_o c = new Point_o(Integer.valueOf(coordinates.get(4)),Integer.valueOf(coordinates.get(5)));
		
		//End of input
		if(a.x==0 && a.y==0 && b.x==0 && b.y==0 && c.x==0 && c.y==0) {
			break;
		}
		
		//Check if triangle
		if(checkiftriangle(a,b,c)) {
			System.out.println("Not a triangle\n");
			System.out.flush();
			continue;
		}
		
		for(int y = screen_height; y>= 0; y--) {
			System.out.print("\n");
			System.out.flush();
			for(int x = 0; x<=screen_width ; x++) {
				Point_o p = new Point_o(x, y);
				
				if( ( (isLeft(a,b,p) == isLeft(b,c,p)) && (isLeft(b,c,p) == isLeft(c,a,p)) ) || 
						( checkonline(a,b,c,p) || checkonline(b,c,a,p) || checkonline(c,a,b,p) ) ){
						System.out.print("*");
						System.out.flush();
					}
				else {
					System.out.print(".");
					System.out.flush();
				}
			}
		}
		System.out.println("\n");
		System.out.flush();		
	}
}	
	
		public static ArrayList<String> ReadConsole(){
			
			
			Scanner scanner = new Scanner(System.in);
	        ArrayList<String> input = new ArrayList<String>();
	        String lineNew;

	        while (scanner.hasNextLine()) {
	            lineNew = scanner.nextLine();
	            if (lineNew.isEmpty()) {
	                break;
	            }
	            input.add(lineNew);
	        }
	        scanner.close();
	        return input;
		}
		
		public static class Point_o { 
		    public int x;   
		    public int y;   
		   
		    public Point_o(int x, int y) {
		        this.x = x;
		        this.y = y;
		    }
		}
		
		//Implicit line eqution and check for sign
		public static Integer isLeft(Point_o a, Point_o b, Point_o c){
		     if (((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) > 0){
		    	 return 1;
		     }
		     else {
		    	 return -1;
		     }
		}
		
		//Orientation function
		public static Integer ORI2 (Point_o a, Point_o b, Point_o c) {
			
			double dx21 = b.x - a.x;
			double dy21 = b.y - a.y;
			double dx31 = c.x - a.x;
			double dy31 = c.y - a.y;
			
			if (dx21*dy31 < dy21*dx31) return -1; // ccw
			if (dx21*dy31 > dy21*dx31) return +1; // cw
			if (dx21*dx31 < 0 || dy21*dy31 < 0) return -1; // ccw
			if (dx31*dx31 + dy31*dy31 > dx21*dx21 + dy21*dy21) return +1; //cw
			return 0; // p3 is inside p1,p2
			
		}
		
		//Check if point is on the border of triangle
		public static Boolean checkonline(Point_o a, Point_o b, Point_o c, Point_o p) {
			if( (b.y-a.y)*p.x - (b.x-a.x)*p.y + (b.x*a.y) - (b.y*a.x) == 0 &&  (ORI2(a,c,p) != ORI2(b,c,p))) {
				return true;
			}
			else {
				return false;
			}
			
		}

		//Check if triangle
		public static Boolean checkiftriangle(Point_o a, Point_o b, Point_o c) {
			if( (b.y-a.y)*c.x - (b.x-a.x)*c.y + (b.x*a.y) - (b.y*a.x) == 0 ) {
				return true;
			}
			else {
				return false;
			}
			
		}
			
}

