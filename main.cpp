/*			8 Puzzle Solver with Hill Climb Algorithm
 *							coding by
 *					Mohammad Abdul Iman Syah
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>

using namespace std;

int GoalState[9], CurrentState[9], ChildState[9] = {0,0,0,0,0,0,0,0,0};

// -->	Pemilihan Goal State 8-Puzzle
// -->		Goal State Ganjil : jika jumlah perbandingan semua tile dengan tile setelahnya bernilai ganjil
// -->		Goal State Genap  : jika jumlah perbandingan semua tile dengan tile setelahnya bernilai genap
void GetGoalState(int initialstate[], int goalstate[]){
	int i, j, total = 0;
	int GoalStateGanjil[] = {1,2,3,8,0,4,7,6,5};
	int GoalStateGenap[] = {0,1,2,3,4,5,6,7,8};

	for(i=0; i<8; i++){
		for(j=i+1; j<9; j++){
			if(initialstate[j] < initialstate[i] && initialstate[j] != 0)
				total = total + 1;	
		}
	}

	if(total % 2 == 1){
		for(i=0; i<9; i++){
			goalstate[i] = GoalStateGanjil[i];
		}
	}else{
		for(i=0; i<9; i++){
			goalstate[i] = GoalStateGenap[i];
		}
	}
}

// -->	Tampilkan state pada layar
void PrintState(int state[]){
	for(int i=0; i<9; i++){
		if(i % 3 == 0)
			cout << endl;
			
		cout << " " << state[i];
	}
	cout << endl;
}

// -->	Copy array ke dalam array yang lain
void CopyState(int from[], int to[]){
	for(int i=0; i<9; i++){
		to[i] = from[i];
	}
}

// -->	Cek kesamaan antar state
bool SameState(int currentstate[], int goalstate[]){
	for(int i=0; i<9; i++){
		if(currentstate[i] != goalstate[i]){
			return false;
			break;
		}
	}
	return true;
}

// -->	Cari posisi kotak kosong pada state
int GetKotakKosong(int state[]){
	int index = 0;

	for(int i=0; i<9; i++){
		if(state[i] == 0)
			index = i;
	}

	return index;
}

// -->	Geser ke Atas kotak kosong state
void MoveUp(int state[]){
	int index = GetKotakKosong(state);
	
	if(index > 2){		
		state[index] = state[index-3];
		state[index-3] = state[index-3] - state[index];
	}
}

// -->	Geser ke Bawah kotak kosong state
void MoveDown(int state[]){
	int index = GetKotakKosong(state);

	if(index < 6){
		state[index] = state[index+3];
		state[index+3] = state[index+3] - state[index];
	}
}

// -->	Geser ke Kiri kotak kosong state
void MoveLeft(int state[]){
	int index = GetKotakKosong(state);

	if(index % 3 > 0){
		state[index] = state[index-1];
		state[index-1] = state[index-1] - state[index];
	}
}

// -->	Geser ke Kanan kotak kosong state
void MoveRight(int state[]){
	int index = GetKotakKosong(state);

	if(index % 3 < 2){
		state[index] = state[index+1];
		state[index+1] = state[index+1] - state[index];
	}
}

// -->	Hitung jumlah kotak yang sama dengan Goal State
int MatchTile(int currentstate[], int goalstate[]){
	int match = 0;
	
	for(int i=0; i<9; i++){
		if(currentstate[i] != 0 && currentstate[i] == goalstate[i])
			match++;
	}

	return match;
}

// -->	Cari langkah terbaik
int GetTheBestMove(int heuristic[]){
	int index = 0;
	int max = heuristic[0];

	for(int i=1; i<4; i++){
		if(heuristic[i] > max){
			max = heuristic[i];
			index = i;
		}
	}

	return index;
}

int main(int argc, char** argv) {
// -->	GoalStateGanjil
	int InitialState[] = {2,8,3,1,6,4,7,0,5};
// -->	GoalStateGenap
//	int InitialState[] = {3,1,2,6,4,5,0,7,8};
	GetGoalState(InitialState,GoalState);
	cout << "\nInit State\n" << "----------";		 
	PrintState(InitialState);
	cout << "\nGoal State\n" << "----------";
	PrintState(GoalState);
	cout << "\nSearching\n" << "---------";			
	CopyState(InitialState,CurrentState);

	int level = 0;
	clock_t t;
	t = clock();

// -->	Lakukan perulangan sampai ditemukan GoalState
	while(!SameState(CurrentState,GoalState)){
		PrintState(CurrentState);
		CopyState(CurrentState,ChildState);

		/*	Fungsi Heuristic
		 * --------------------------------------------------------------
		 *  f(n) = g(n) + h(n)
		 * 		f(n) = Nilai Heuristic
		 * 		g(n) = Jarak CurrentState ke InitialState (Level of Search)
		 * 		h(n) = Jumlah kotak yang sama dengan GoalState
		 *---------------------------------------------------------------
		 */
		 
		int Heuristic[] = {0,0,0,0};
		level++;
		
	/*	Percobaan Penggeseran untuk mendapatkan Nilai Heuristic	*/
		// -->	Geser ke atas dan hitung nilai heuristicnya
		MoveUp(ChildState);
		if(!SameState(ChildState,CurrentState)){
			Heuristic[0] = MatchTile(ChildState,GoalState) + level;
			MoveDown(ChildState);
		}

		// -->	Geser ke bawah dan hitung nilai heuristicnya
		MoveDown(ChildState);
		if(!SameState(ChildState,CurrentState)){
			Heuristic[1] = MatchTile(ChildState,GoalState) + level;
			MoveUp(ChildState);
		}

		// -->	Geser ke kiri dan hitung nilai heuristicnya
		MoveLeft(ChildState);
		if(!SameState(ChildState,CurrentState)){
			Heuristic[2] = MatchTile(ChildState,GoalState) + level;
			MoveRight(ChildState);
		}

		// -->	Geser ke kanan dan hitung nilai heuristicnya
		MoveRight(ChildState);
		if(!SameState(ChildState,CurrentState)){
			Heuristic[3] = MatchTile(ChildState,GoalState) + level;
			MoveLeft(ChildState);
		}
	/*	End of Percobaan Penggeseran	*/

		// -->	Geser ke arah nilai heuristic terbaik
		switch(GetTheBestMove(Heuristic)){
			case 0 : MoveUp(CurrentState); break;
			case 1 : MoveDown(CurrentState); break;
			case 2 : MoveLeft(CurrentState); break;
			case 3 : MoveRight(CurrentState); break;
		}		
	}

	PrintState(CurrentState);

	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	
	printf("\n Solved with %i steps, %f seconds...",level,time_taken);
	getchar();
	return 0;
}
