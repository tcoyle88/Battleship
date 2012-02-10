/*

BATTLESHIP! Console Game
w/ Online Multiplayer
Written by Terrance Coyle 2012

**must be running server for game to work**

compile and include hostname / port on run
ie:    ./battleship localhost 2010

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int coords[10][10];
int mcoords[10][10];

struct CoordXY {
	int x;
	int y;
};


class Ship
{
	public:
	int size;
	int hits; //number of points that are hit
	int direct; //direction of ship (horizontal or vertical)
	int stat; //horizontal or vertical point that is same throughout
	int start; //starting point

	void set_values(int,int,int,int,int);
	void printCoords();
	bool checkPoint(int,int,int);
	
friend ostream& operator<< (ostream& out, Ship& object) {
out << object.size << " " << object.hits << " " << object.direct << " " << object.stat << " " << object.start; 
return out;
}

friend istream& operator>> (istream& in, Ship& object) {
in >> object.size;
in >> object.hits;
in >> object.direct;
in >> object.stat;
in >> object.start;
return in;
}

};

void Ship::set_values(int a, int b, int c, int d, int f){ //set ship values
	size=a;
	hits=b;
	direct=c;
	stat=d;
	start=f;
};

void Ship::printCoords(){ //print coordinates of single ship
	int i,x,y;
	if(direct==0){
		x=stat;
		y=start;
	} else {
		y=stat;
		x=start;
	}
	cout << "Ship Coordinates:" << endl;
	for(i=0;i<size;i++){
		cout << "X:" << x << " Y:" << y << endl;
		if(direct==0)
			y++;
		else
			x++;
	}
};


bool Ship::checkPoint(int xg, int yg, int hitc){ //check called point for hit on ship
	int i,x,y;
	if(direct==0){
		x=stat;
		y=start;
	} else {
		y=stat;
		x=start;
	}
	for(i=0;i<size;i++){
		if((xg==x)&&(yg==y)){
			if(hitc>0)	hits++;
			return true;
		}
		if(direct==0)
			y++;
		else
			x++;
	}
	return false;
};



bool pointExists(Ship *gShips, int i, int size, int d, int stat, int start){ //check to see if random generated ship point is already in use
	int n,v,ii,x,y;
	int npt=start;
	int xp[size];
	int yp[size];
	
	for(ii=0;ii<size;ii++){
		if(d==1){
			xp[ii]=npt;
			yp[ii]=stat;
		} else {
			xp[ii]=stat;
			yp[ii]=npt;
		}
		npt++;
	}
	
	for(n=i;n>=0;n--){
		int point=gShips[n].start;
		for(v=0;v<gShips[n].size;v++){
			if(gShips[n].direct==1){
				x=point;
				y=gShips[n].stat;
			} else {
				x=gShips[n].stat;
				y=point;
			}
			
			for(ii=0;ii<size;ii++){
				if((x==xp[ii])&&(y==yp[ii])){
					return true;
				}
			}
			point++;
		}
	}
	return false;
}

string get_letter(int num){ //get corresponding letter to number (ie 1 = A, 2= B)
	string letter;
	switch(num){
    	case 0:
    		letter=" ";
    	break;
    	case 1:
    		letter="A";
    	break;
    	case 2:
    		letter="B";
    	break;
    	case 3:
    		letter="C";
    	break;
    	case 4:
    		letter="D";
    	break;
    	case 5:
    		letter="E";
    	break;
    	case 6:
    		letter="F";
    	break;
    	case 7:
    		letter="G";
    	break;
    	case 8:
    		letter="H";
    	break;
    	case 9:
    		letter="I";
    	break;
    	case 10:
    		letter="J";
    	break;
    }
	return letter;
}

string checkCoords(int x, int y, int who){
	int p;
	if(who>0)
		p=coords[x][y];
	else
		p=mcoords[x][y];
		
	if(p==1)
		return "X";
	else
		return "-";
}


int StringToNumber (string s)
{
	stringstream ss(s);
	int result;
	return ss >> result ? result : 0;
}


int runCheck(Ship *gShips, int x, int y, int hitc){
	int i=0;
	for(i=0;i<5;i++){
		bool hit=gShips[i].checkPoint(x,y,hitc);
		if(hit){
			if(gShips[i].size==gShips[i].hits)
				return 2;
			else
				return 1;
		}
	}
	return 0;
}

int shipsRemain(Ship *gShips){
	int n=5;
	int i=0;
	for(i=0;i<5;i++){
			if(gShips[i].hits>=gShips[i].size)
				n--;
	}
	return n;
}

int getXcoor(string s){
	int x;
		if(s=="A"||s=="a")
			x=1;
		if(s=="B"||s=="b")
			x=2;
		if(s=="C"||s=="c")
			x=3;
		if(s=="D"||s=="d")
			x=4;
		if(s=="E"||s=="e")
			x=5;
		if(s=="F"||s=="f")
			x=6;
		if(s=="G"||s=="g")
			x=7;
		if(s=="H"||s=="h")
			x=8;
		if(s=="I"||s=="i")
			x=9;
		if(s=="J"||s=="j")
			x=10;		
	return x;
}


string print_board(Ship *gShips,int who){
	string out,letter,pin;
	int count,count2,hit;
	
	out = "    1    2    3    4    5    6    7    8    9    10";
	for(count=0;count<=10;count++){
    
    out += get_letter(count);
    out += "  ";
    
    	for(count2=1;count2<=10;count2++){
    		if(count>0){
    			pin=checkCoords(count,count2,who);
    				hit=runCheck(gShips, count, count2,0);
    				if(who>0){
    					if(hit>0){
    						out += "[";
    						out += pin;
    						out += "]";
    					} else {
    						out += " ";
    						out += pin;
    						out += " ";
    					}
    				} else {
    					if((hit>0)&&(pin=="X")){
    						out += "[";
    						out += pin;
    						out += "]";
    					} else {
    						out += " ";
    						out += pin;
    						out += " ";
    					}
    				}
    			out += "  ";
    		}
    	}
	out += "\n\n";
	
    }
    
    return out;
}


string packShips(Ship *gShips){
	stringstream out;
	string ships;
	int i;
	for(i=0;i<5;i++){
			out << gShips[i].size;
			out << " ";
			out << gShips[i].hits;
			out << " ";
			out << gShips[i].direct;
			out << " ";
			out << gShips[i].stat;
			out << " ";
			out << gShips[i].start;
			out << " ";
	}
	ships=out.str();
	return ships;
}


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

vector<string> explode( const string &delimiter, const string &explodeme);

int main(int argc, char *argv[])
{

string rerun;

do{

	
    int count=0;
    int count2=0;
    int num1=0;
    int num2=0;
    int guesses=0;
    int o_guesses=0;
    int remains;
	string letter,guess;
	
	srand(time(NULL));
	int number = rand()%10+1;
	int start;
	int stat;
	int i,ii,s,x,y;
	
	int d=0;
	int ssize=5;
	Ship *gShips;
	Ship *mShips;
	mShips = new Ship[5];
	gShips = new Ship[5];
	
	bool point_empty=true;
	
	//generate random ships
	for(i=0;i<5;i++){
	point_empty=true;
		while(point_empty){
		stat=rand()%10+1;
		start=rand()%10+1;
		while((10-start)<ssize){
			start=rand()%10+1;
		}
			point_empty=pointExists(gShips, i-1, ssize, d, stat, start);
		}
		
		
		gShips[i].set_values(ssize, 0, d, stat, start);
		if((ssize==3)&&(i==2)){
			//do nothing
		} else {
			ssize--;
		}
			
		d++;
		if(d==2)
			d=0;
	}
	
	
	cout << endl;


	//PRINT MAIN MENU
cout << " ******       **     ********** ********** **       ********  ******** **      ** ** ******* " << endl;
cout << "/*////**     ****   /////**/// /////**/// /**      /**/////  **////// /**     /**/**/**////**" << endl;
cout << "/*   /**    **//**      /**        /**    /**      /**      /**       /**     /**/**/**   /**" << endl;
cout << "/******    **  //**     /**        /**    /**      /******* /*********/**********/**/******* " << endl;
cout << "/*//// ** **********    /**        /**    /**      /**////  ////////**/**//////**/**/**////  " << endl;
cout << "/*    /**/**//////**    /**        /**    /**      /**             /**/**     /**/**/**      " << endl;
cout << "/******* /**     /**    /**        /**    /********/******** ******** /**     /**/**/**      " << endl;
cout << "///////  //      //     //         //     //////// //////// ////////  //      // // //       " << endl;

	cout << "Welcome to BATTLESHIP Online!" << endl << "Written by TJ Coyle" << endl << endl;
    

    cout << endl << "The computer will randomly place 5 ships on the game board..." << endl;
    cout << "Your objective is to guess coordinates until you have successfully sank all 5 ships." << endl;
    cout << "The high score is achieved by sinking all 5 ships in the least amount of guesses!" << endl << endl;
    
    cout << "***Press Q to quit at any time***" << endl << endl;
    
    int stn;
    cout << "Press Any Key To PLAY!" << endl;
    cin.ignore();
    getc(stdin);
    
    
    //##########################################
    //CONNECT MULTIPLAYER ######################
    //##########################################
    
    int sockfd, portno, n, player;
    struct sockaddr_in serv_addr;
    struct hostent *server;
//	string kill;
	
	//int portno=2500;
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
        
    //##########################################
    
    
	//connection established time to rock n roll
    
    //receive wait message
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
			printf("%s\n\n",buffer);
			
	//receive confirmation and continue
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
			printf("%s\n\n",buffer);
			
	//get player number
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) 
			error("ERROR reading from socket");
			
		if(strcmp(buffer,"player1") != 0)
			player=2;
		else
			player=1;
    		
    		//pack local ship coords into single string
    		string ships=packShips(gShips);
    	
    		//send local ship coords and receive opponents
			n = write(sockfd,ships.c_str(),sizeof(ships)*21);
			if (n < 0) error("ERROR writing to socket");
			bzero(buffer,sizeof(ships)*21);
			n = read(sockfd,buffer,sizeof(ships)*21);
			if (n < 0) error("ERROR reading from socket");
			ships = buffer;
			
			//break opponent coords into array
			vector<string> v = explode(" ", ships);
			
			//create my ships
			int scount=0;
			for(i=0;i<5;i++){
					mShips[i].set_values(StringToNumber(v[scount]),StringToNumber(v[scount+1]),StringToNumber(v[scount+2]),StringToNumber(v[scount+3]),StringToNumber(v[scount+4]));
					scount=scount+5;
					//mShips[i].printCoords();
			}
			

    	cout << "You are Player #" << player << endl;
    	int opp_n=2;
    	bool myturn=false;
    	if(player==1)
    		myturn=true;
    	else
    		opp_n=1;
    		
    	//START GAME

	
	cout << "||||||||||||||||||||||||||||||||||||||||||" << endl;
    cout << "|||||HERE ARE YOUR SHIPS! GOOD LUCK!||||||" << endl;
    cout << "||||||||||||||||||||||||||||||||||||||||||" << endl << endl;
    cout << print_board(gShips,1) << endl << endl << "LETS PLAY!" << endl << endl;

	int g=0;
	while(g==0){
	bool bad=false;
	
	if(myturn){
	
		bad=true;
		while(bad){
		cout << "Please guess a coordinate (ex: A5):\n>";
		cin >> guess;
		
		//quit game if Q is entered
		if((guess=="q")||(guess=="Q")){
			close(sockfd);
			return 0;
		}
		
		string yS=guess.substr(1);
		y=StringToNumber(yS);
		string xS=guess.substr(0,1);
		x=getXcoor(xS);
		if(mcoords[x][y]==1)
			cout << "You already played this point. Guess again." << endl << endl;
		else
			bad=false;
		}
		
		mcoords[x][y]=1;
		guesses++; //tally guesses
		cout << endl << print_board(mShips, 0) << endl << endl;
		
		int result = runCheck(mShips,x,y,1); //check point guessed to ships on board
		if(result==0)
			cout << endl << endl << "SORRY! YOU MISSED!" << endl << endl;
		else if(result==1)
			cout << endl << endl << "YOU HIT A SHIP!!" << endl << endl;
		else
			cout << endl << endl << "HIT AND SUNK! YOU SUNK THE OPPONENTS SHIP!" << endl << endl;
			
		remains=shipsRemain(mShips); //CHECK REMAINING UNSUNKEN SHIPS
		if(remains==0) //IF NO REMAINING SHIPS, END LOOP/GAME
			g=1;
		
		//display remaining ships
		cout << "Opponent Ships Remaining: " << remains << endl;
		cout << "# of Guesses Made: " << guesses << endl << endl;
		
		//send guess too opponent
		n = write(sockfd,guess.c_str(),sizeof(guess)*21);
		if (n < 0) error("ERROR writing to socket");
		
		myturn=false;
			
	} else {
	
		cout << "Please Wait... Player " << opp_n << "'s Turn..." << endl;
		
		//read opponent guess and record
		bzero(buffer,256);
		n = read(sockfd,buffer,256);
		if (n < 0) error("ERROR reading from socket");
		guess = buffer;
	
	
		string yS=guess.substr(1);
		int y=StringToNumber(yS);
		string xS=guess.substr(0,1);
		int x=getXcoor(xS);
		coords[x][y]=1;
		
		o_guesses++; //tally guesses
		cout << endl << print_board(gShips,1) << endl << endl;
		
		int result = runCheck(gShips,x,y,1); //check point guessed to ships on board
		if(result==0)
			cout << endl << endl << "OPPONENT MISSED YOUR SHIPS!" << endl << endl;
		else if(result==1)
			cout << endl << endl << "OPPONENT HIT YOUR BATTLESHIP!!" << endl << endl;
		else
			cout << endl << endl << "HIT AND SUNK! YOUR OPPONENT SUNK ONE OF YOUR SHIPS!" << endl << endl;
			
		remains=shipsRemain(gShips); //CHECK REMAINING UNSUNKEN SHIPS
		if(remains==0) //IF NO REMAINING SHIPS, END LOOP/GAME
			g=2;
		
		//display remaining ships
		cout << "Your Ships Remaining: " << remains << endl;
		cout << "# of Guesses Player " << opp_n << " Made: " << o_guesses << endl << endl;
	
		myturn=true;
	}
	
	
	}
	
	if(g==1){
			cout << "YOU WIN! YOU HAVE SUCCESSFULLY SANK ALL 5 OF YOUR OPPONENTS SHIPS!" << endl << endl;
			close(sockfd);
  			cout << "You sunk all 5 ships in " << guesses << " trys!" << endl << endl;
  			cout << "Press <M> To Go Back To The Main Menu\n>";
  			cin>>rerun;
  	} else {
  		cout << "YOU LOST! YOUR OPPONENT SUNK ALL OF YOUR SHIPS!" << endl << endl;
  		close(sockfd);
  		cout << "Press <M> To Go Back To The Main Menu\n>";
  		cin>>rerun;
  	}
	
	for(i=0;i<10;i++){
		for(ii=0;ii<10;ii++){
			coords[i][ii]=0;
			mcoords[i][ii]=0;
		}
	}
} while(rerun=="M"||rerun=="m");
 
    //system("PAUSE");
    return 0;
    return EXIT_SUCCESS;
}


vector<string> explode( const string &delimiter, const string &str)
{
    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0; 
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}
