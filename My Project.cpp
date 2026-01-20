#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

struct Player {
    string name;
    int balls, six, four, runs, wickets;
    int runsConceded, oversBowled;
    float strikerate, economy;
};

int team1Score=0, team2Score=0;
string team1Name, team2Name;

// Toss function
void toss(){
    string choice, decision;
    string tosswinner, tossloser;
    
    cout<<"Enter name of team 1 : ";
    cin>>team1Name;
    cout<<"Enter name of team 2 : ";
    cin>>team2Name;
    
    cout<<"\n\nEnter your choice (head/tail) : ";
    cin>>choice;
    
    srand(time(0));
    int toss=rand()%2+1;
    string otoss=(toss==1? "head" : "tail");
    cout<<"Toss result : "<<otoss<<endl; 
    
    if(choice==otoss){
        tosswinner=team1Name;
        tossloser=team2Name;
    }
    else if(choice=="head" || choice=="tail"){
        tosswinner=team2Name;
        tossloser=team1Name;
    }
    else{
        cout<<"Invalid choice, please enter head or tail.\n\n";
        return;
    }
    cout<<tosswinner<<" has won the toss.\n";
    cout<<tosswinner<<", would you like to bat first or bowl? ";
    cin>>decision;
    
    if(decision=="bat"){
        cout<<tossloser<<" will bowl first.\n";
    }
    else if(decision=="bowl"){
        cout<<tossloser<<" will bat first.\n";
    }
    else{
        cout<<"Invalid decision, choose bat or bowl.\n";
    }
}

// Function to play one innings
int playInnings(string innings, int overs, int numplayers, Player team[], int target=1){
    cout<<"\n\n========== Let's start the "<<innings<<" innings ==========\n\n";
    
    for(int i=0;i<numplayers;i++){
        team[i].name="";
        team[i].balls=team[i].six=team[i].four=team[i].runs=team[i].wickets=0;
        team[i].runsConceded=team[i].oversBowled=0;
        team[i].strikerate=team[i].economy=0.0;
    }
    
    cin.ignore();
    cout<<"Enter opener 1 name : ";
    getline(cin,team[0].name);
    cout<<"Enter opener 2 name : ";
    getline(cin,team[1].name);
    
    int striker=0, nonstriker=1;
    int nextPlayerIndex=2;
    int totalRuns=0;
    
    for(int i=0;i<overs;i++){
        cout<<"\n=== Over "<<i+1<<" ===\n";
        string bowler;
        cout<<"Enter bowler name for this over : ";
        getline(cin,bowler);
        
        int bowlerIndex=-1;
        for(int k=0;k<numplayers;k++){
            if(team[k].name==bowler){
                bowlerIndex=k;
                break;
            }
        }
        if(bowlerIndex==-1){ // new bowler entry
            for(int k=0;k<numplayers;k++){
                if(team[k].name==""){
                    team[k].name=bowler;
                    bowlerIndex=k;
                    break;
                }
            }
        }
        
        int runsThisOver=0;
        for(int j=0;j<6;j++){
            int run;
            cout<<"Ball "<<j+1<<" runs (-1 for wicket) : ";
            cin>>run;
            cin.ignore();
            
            if(run==-1){
                string outplayer=team[striker].name;
                cout<<" Wicket! "<<outplayer<<" is out by "<<bowler<<"\n";
                team[bowlerIndex].wickets++;
                
                if(nextPlayerIndex<numplayers){
                    cout<<"Enter next batsman name : ";
                    getline(cin,team[nextPlayerIndex].name);
                    striker=nextPlayerIndex;
                    nextPlayerIndex++;
                } else {
                    cout<<"All players are out!\n";
                    break;
                }
            }
            else{
                team[striker].runs+=run;
                team[striker].balls++;
                totalRuns+=run;
                runsThisOver+=run;
                
                if(run==6) team[striker].six++;
                if(run==4) team[striker].four++;
                team[striker].strikerate=(team[striker].runs*100.0)/team[striker].balls;
                
                if(run%2==1){ 
                    int temp=striker;
                    striker=nonstriker;
                    nonstriker=temp;
                }
            }
            
            // Stop second innings if target is chased
            if(innings=="Second" && totalRuns>target){
                cout<<"\nTarget chased successfully!\n";
                return totalRuns;
            }
        }
        team[bowlerIndex].runsConceded+=runsThisOver;
        team[bowlerIndex].oversBowled++;
        if(team[bowlerIndex].oversBowled>0)
            team[bowlerIndex].economy=(float)team[bowlerIndex].runsConceded/team[bowlerIndex].oversBowled;
        
        int temp=striker;
        striker=nonstriker;
        nonstriker=temp;
    }
    
    return totalRuns;
}

// Function to print batting and bowling summary
void printSummary(string innings, int numplayers, Player team[]){
    cout<<"\n\n====== Batting Summary ("<<innings<<") ======\n\n";
    cout<<left<<setw(20)<<"Player_Name"<<setw(10)<<"Runs"<<setw(10)<<"Balls"<<setw(10)<<"4s"<<setw(10)
        <<"6s"<<setw(10)<<"SR"<<endl;
    
    for(int i=0;i<numplayers;i++){
        if(team[i].name!="" && team[i].balls>0)
        cout<<left<<setw(20)<<team[i].name<<setw(10)<<team[i].runs<<setw(10)<<team[i].balls<<setw(10)<<team[i].four
        <<setw(10)<<team[i].six<<setw(10)<<team[i].strikerate<<endl;
    }
    
    cout<<"\n\n====== Bowling Summary ("<<innings<<") ======\n\n";
    cout<<left<<setw(20)<<"Bowler_Name"<<setw(10)<<"Overs"<<setw(15)<<"RunsConceded"<<setw(10)<<"Wickets"<<setw(10)<<"Economy"<<endl;
    for(int i=0;i<numplayers;i++){
        if(team[i].oversBowled>0)
        cout<<left<<setw(20)<<team[i].name<<setw(10)<<team[i].oversBowled<<setw(15)<<team[i].runsConceded
        <<setw(10)<<team[i].wickets<<setw(10)<<team[i].economy<<endl;
    }
}

// Function to declare winner
void declareWinner(){
    cout<<"\n\n========== Match Result ==========\n";
    if(team1Score>team2Score){
        cout<<team1Name<<" won the match by "<<team1Score-team2Score<<" runs.\n";
    } else if(team2Score>team1Score){
        cout<<team2Name<<" won the match by "<<team2Score<<" wickets (chased target).\n";
    } else {
        cout<<"Match tied!\n";
    }
}



// Function to display and save top 3 performers with bat and ball
void topPerformers(string teamName, int numplayers, Player team[]){
    ofstream fout("TopPerformers.txt", ios::app); 

    cout<<"\n\n========== Top Performers of "<<teamName<<" ==========\n";
    fout<<"\n\n========== Top Performers of "<<teamName<<" ==========\n";

    // --- Batting performers ---
    cout<<"\nTop 3 Batting Performers:\n";
    fout<<"\nTop 3 Batting Performers:\n";

    // Sort players by runs 
    Player *batSorted = new Player[numplayers];
    for(int i=0;i<numplayers;i++) batSorted[i]=team[i];
    for(int i=0;i<numplayers-1;i++){
        for(int j=i+1;j<numplayers;j++){
            if(batSorted[j].runs > batSorted[i].runs){
                swap(batSorted[i], batSorted[j]);
            }
        }
    }

    for(int i=0;i<3 && i<numplayers;i++){
        if(batSorted[i].balls>0){
            cout<<i+1<<". "<<batSorted[i].name<<" - Runs: "<<batSorted[i].runs
                <<", Balls: "<<batSorted[i].balls<<", SR: "<<batSorted[i].strikerate<<"\n";
            fout<<i+1<<". "<<batSorted[i].name<<" - Runs: "<<batSorted[i].runs
                <<", Balls: "<<batSorted[i].balls<<", SR: "<<batSorted[i].strikerate<<"\n";
        }
    }

    // --- Bowling performers ---
    cout<<"\nTop 3 Bowling Performers:\n";
    fout<<"\nTop 3 Bowling Performers:\n";

    Player *bowlSorted = new Player[numplayers];
    for(int i=0;i<numplayers;i++) bowlSorted[i]=team[i];
    for(int i=0;i<numplayers-1;i++){
        for(int j=i+1;j<numplayers;j++){
            if(bowlSorted[j].wickets > bowlSorted[i].wickets){
                swap(bowlSorted[i], bowlSorted[j]);
            }
        }
    }

    for(int i=0;i<3 && i<numplayers;i++){
        if(bowlSorted[i].oversBowled>0){
            cout<<i+1<<". "<<bowlSorted[i].name<<" - Wickets: "<<bowlSorted[i].wickets
                <<", Runs Conceded: "<<bowlSorted[i].runsConceded
                <<", Economy: "<<bowlSorted[i].economy<<"\n";
            fout<<i+1<<". "<<bowlSorted[i].name<<" - Wickets: "<<bowlSorted[i].wickets
                <<", Runs Conceded: "<<bowlSorted[i].runsConceded
                <<", Economy: "<<bowlSorted[i].economy<<"\n";
        }
    }

    delete[] batSorted;
    delete[] bowlSorted;
    fout.close();
}



int main(){
    int overs, numplayers;
    cout<<"Enter overs for match : ";
    cin>>overs;
    cout<<"Enter number of players per side : ";
    cin>>numplayers;
    
    cout<<endl<<endl;
    toss();
    
    Player *team1=new Player[numplayers];
    Player *team2=new Player[numplayers];
    
    team1Score=playInnings("First", overs, numplayers, team1);
    printSummary("First", numplayers, team1);
    
    team2Score=playInnings("Second", overs, numplayers, team2, team1Score);
    printSummary("Second", numplayers, team2);
    
    declareWinner();
    
    topPerformers(team1Name, numplayers, team1);
	topPerformers(team2Name, numplayers, team2);

    
    delete[] team1;
    delete[] team2;
    
    return 0;
}

