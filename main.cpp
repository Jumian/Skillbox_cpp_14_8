#include <iostream>

bool print(bool field1[][10],bool field2[][10],int player){
    int counter1=0,counter2=0;
    std::string gap="   ";
    std::cout << " |       PLAYER "<< (player==3?"1":std::to_string(player))<<"      |"<<(player==3?gap+" |       PLAYER 2      |":"")<< std::endl;
    std::cout << " |---------------------|"<< (player==3?gap+" |---------------------|":"")<<std::endl;
    for(int i=9;i>=0;--i) {
        std::cout <<i<< "| ";
        for (int k = 0; k < 10; ++k)
            if(field1[k][i]){
                std::cout << 'o'<< " ";
                ++counter1;
            }else std::cout << "  ";
        std::cout << "|";
        if (player==3) {
            std::cout <<gap <<i<<"| ";
            for (int k = 0; k < 10; ++k)
                if(field2[k][i]){
                    std::cout << 'o'<< " ";
                    ++counter2;
                }else std::cout << "  ";
            std::cout << "|";
        }
        std::cout << std::endl;
    }
    std::cout << " |-0-1-2-3-4-5-6-7-8-9-|"<< (player==3?gap+" |-0-1-2-3-4-5-6-7-8-9-|":"")<< std::endl;
    if (player==3) std::cout << "Player 1 have " << counter1 << " ship units, Player 2 have " << counter2 << " ship units"<< std::endl;
    if (counter2>0 && counter1>0&&player==3) std::cout << "No winner yet" << std::endl;
    else if (counter1==0 && counter2>0&&player==3) {
        std::cout << "=============== Player 2 win! ===============" << std::endl;
        return true;
    }
    else if (counter1>0 && counter2==0&&player==3) {
        std::cout << "=============== Player 1 win! ===============" << std::endl;
        return true;
    }
    return false;
}

void init(bool field[][10]){
    for(int i=0;i<10;++i)
        for(int k=0;k<10;++k)
            field[i][k]=false;
    std::cout << std::endl;
}

bool inRange(int a){
    return ((a>=0&&a<=9)?true:false);
}

bool validCoords(int x1,int y1,int x2,int y2,int size){
    if(!(inRange(x1)&&inRange(x2)&&inRange(y1)&&inRange(y2))) return false;
    if(x1==x2&&y1==y2&&size==1) return true;
    if(x1==x2 && abs(y2-y1)==size-1) return true;
    else if(y1==y2 && abs(x2-x1)==size-1) return true;
    std::cout << "Wrong coordinates for ship size " << size << ". Try again!\n";
    return false;
}

bool noShips(bool f[][10],int x1,int y1,int x2,int y2){
    if (x1>x2) std::swap(x1,x2);
    if (y1>y2) std::swap(y1,y2);
    for (int i=x1;i<10&&i<x2;++i){
        for(int k=y1;k<10&&k<y2;++k) if(f[i][k]) return false;
    }
    return true;
}

bool inputShip(bool f[][10],int size,int player) {
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    bool notDone = false;
    do {
        std::cout << "Player " << player << " ship size = " << size << ". ";
        std::cout << "Input ship coords:";
        if (size > 1 && size < 5) {
            std::cout << "[x1,y1,x2,y2]:";
            std::cin >> x1 >> y1 >> x2 >> y2;
        } else if (size == 1) {
            std::cout << "[x,y]:";
            std::cin >> x1 >> y1;
            x2 = x1;
            y2 = y1;
        } else {
            std::cout << "Wrong size of ship\n";
            continue;
        }
    } while (!validCoords(x1, y1, x2, y2, size) || !noShips(f, x1, y1, x2, y2));
    int coord[2], vector[2];
    coord[0] = x1;
    coord[1] = y1;
    vector[0] = (size > 1 ? (x2 - x1) / (size - 1) : 0);
    vector[1] = (size > 1 ? (y2 - y1) / (size - 1) : 0);
    for (int i = 0; i < size; ++i) {
        if (f[coord[0]][coord[1]]) return false;
        f[coord[0]][coord[1]] = true;
        coord[0] += vector[0];
        coord[1] += vector[1];
    }
    print(f, f, player);
    return true;
}

void inputShips(bool f[][10],int player){
    std::cout<< "=================== Player " << player << "! Input your ship coords! =========================="<< std::endl;
    for(int i=4;i>0;--i){
        for(int k=0;k<5-i;++k) while(!inputShip(f,i,player));
    }
}

int turn(int player, bool f1[][10],bool f2[][10]){
    if (player<1||player>2) return -1;
    int x=-1,y=-1, counter=0;
    while (true) {
        if(print(f1 ,f2 ,3)) return -2;
        do {
            std::cout << "Player " << player << " turn" << std::endl << "Input coords to shoot[x,y]:";
            std::cin >> x >> y;
        } while (!(inRange(x) && inRange(y)));
        if ((player == 2 ? f1 : f2)[x][y]) {
            (player == 2 ? f1 : f2)[x][y] = false;
            ++counter;
        } else return counter;

    }

}

int main() {
    bool field1[10][10];
    bool field2[10][10];
    init(field1);
    init(field2);
    print(field1,field2,3);
    inputShips(field1,1);
    inputShips(field2,2);
    int player=1;
    while(true){
        if(print(field1 ,field2 ,3)) break;
        int turnscore = turn(player,field1,field2);
        if (turnscore==-2) break;
        else if (turnscore==-1) std::cout << "Internal error" << std::endl;
        else if (turnscore==0) std::cout << "Player " << player << " missed!" << std::endl;
        else std::cout << "Player " << player << " hits " << turnscore << " ship units!" << std::endl;
        if (player==1) player =2;
        else player=1;
    }
}
