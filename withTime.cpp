#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cctype>
#include <random>
#include <climits>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct cell{
    int id;
    int x; // column number
    int y; // row number
    vector<int> nets; //list of nets it's conencted to
    cell() {}
    cell(int id) {
        this->id = id;
        x = y = -1;
    }
};

vector<cell> cells;

//represents a connection between cells
struct net{
    int id;
    int minX, minY, maxX, maxY;
    int minXCell, minYCell, maxXCell, maxYCell;
    int HPWL;
    vector<int> cells;
    net() {}
    net(int id) {
        this->id = id;
        minX = minY = maxX = maxY = HPWL = 0;
    }
};

vector<net> nets;
int numOfRows, numOfColumns;
vector<vector<int>> grid;

bool isPositiveInteger(string word) {
    for (int i = 0; i < word.length(); i++) {
        if (!isdigit(word[i])) {
            return false;
        }
    }
    return true;
}

void parseNetListFirstLine(string firstLine) {
    vector<int> firstLineInts;
    stringstream firstLineStringStream(firstLine);
    string tempWord;
    while(firstLineStringStream >> tempWord && isPositiveInteger(tempWord)) {
        firstLineInts.push_back(stoi(tempWord));
    }
    if(firstLineInts.size() != 4) {
        cout << "Error: first line of netlist file does not contain four space separated positive integers" << endl;
        exit(1);
    } else {
        cells.resize(firstLineInts[0]);
        for(int i = 0; i < cells.size(); i++) {
            cells[i] = cell(i);
        }
        nets.resize(firstLineInts[1]);
        numOfRows = firstLineInts[2];
        numOfColumns = firstLineInts[3];
        grid.resize(numOfRows);
        for(int i = 0; i < grid.size(); i++) {
            grid[i].resize(numOfColumns, -1);
        }
    }
}

void parseNetListNetLine(string line, int netIndex) {
    vector<int> netLineInts;
    stringstream netLineStringStream(line);
    string tempWord;
    while(netLineStringStream >> tempWord && isPositiveInteger(tempWord)) {
        netLineInts.push_back(stoi(tempWord));
    }
    if(netLineInts.size() < 1 || netLineInts.size()-1 < netLineInts[0]) {
        cout << "Error: net line of netlist file is malformed" << endl;
        exit(1);
    } else {
        nets[netIndex] = net(netIndex);
        for(int i = 1; i < netLineInts.size(); i++) {
            nets[netIndex].cells.push_back(netLineInts[i]);
            cells[netLineInts[i]].nets.push_back(netIndex);
        }
    }
}

void parseNetListFile(string netListFileName) {
    cout << "Parsing netlist file: " << netListFileName << endl;
    ifstream netListFile(netListFileName);
    if (netListFile.is_open()) {
        string firstLine;
        if(getline(netListFile, firstLine)) {
            parseNetListFirstLine(firstLine);
        }
        string line;
        int netIndex = 0;
        while(getline(netListFile, line)) {
            parseNetListNetLine(line, netIndex);
            netIndex++;
        }
        netListFile.close();
    }
    else 
        cout << "Unable to open file" << endl;
}

void placeInitiallyRandom() {
    minstd_rand rng(time(0));
    uniform_int_distribution<int> dist(0, numOfRows*numOfColumns-1);
    for(int i = 0; i < cells.size(); i++) {
        do {
            int random_number = dist(rng);
            // x is column number
            // y is row number
            cells[i].y = random_number / numOfColumns;
            cells[i].x = random_number % numOfColumns;
        } while(grid[cells[i].y][cells[i].x] != -1);

        // double checking
        if (cells[i].y < numOfRows && cells[i].x < numOfColumns) {
            grid[cells[i].y][cells[i].x] = i;
        } else {
            cerr << "Error: Invalid cell position generated." << endl;
            exit(1);
        }
    }
}

void printBinaryGrid() {
    for(int i = 0; i < numOfRows; i++) {
        for(int j = 0; j < numOfColumns; j++) {
            if(grid[i][j] == -1) {
                cout << "1 ";
            } else {
                cout << "0 ";
            }
        }
        cout << endl;
    }
}

int computeHPWLofNet(int netIndex) {
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    int minXCell = -1, minYCell = -1, maxXCell = -1, maxYCell = -1;
    for(int i = 0; i < nets[netIndex].cells.size(); i++) {
        int cellIndex = nets[netIndex].cells[i];
        if(cells[cellIndex].x < minX) {
            minX = cells[cellIndex].x;
        }
        if(cells[cellIndex].x > maxX) {
            maxX = cells[cellIndex].x;
        }
        if(cells[cellIndex].y < minY) {
            minY = cells[cellIndex].y;
        }
        if(cells[cellIndex].y > maxY) {
            maxY = cells[cellIndex].y;
        }
    }
    nets[netIndex].minX = minX;
    nets[netIndex].minXCell = minXCell;

    nets[netIndex].minY = minY;
    nets[netIndex].minYCell = minYCell;

    nets[netIndex].maxX = maxX;
    nets[netIndex].maxXCell = maxXCell;

    nets[netIndex].maxY = maxY;
    nets[netIndex].maxYCell = maxYCell;

    nets[netIndex].HPWL = (maxX - minX) + (maxY - minY);
    return nets[netIndex].HPWL;
}

void computeHPWLofAllNets() {
    for(int i = 0; i < nets.size(); i++) {
        computeHPWLofNet(i);
    }
}

int computeTotalWireLength() {
    int totalWireLength = 0;
    for(int i = 0; i < nets.size(); i++) {
        totalWireLength += nets[i].HPWL;
    }
    return totalWireLength;
}

void printGrid() {
    for(int i = 0; i < numOfRows; i++) {
        for(int j = 0; j < numOfColumns; j++) {
            if(grid[i][j] == -1) {
                cout << "----";
            } else if(grid[i][j] < 10) {
                cout << "000" << grid[i][j];
            } else if(grid[i][j] < 100) {
                cout << "00" << grid[i][j];
            } else if(grid[i][j] < 1000) {
                cout << "0" << grid[i][j];
            } else {
                cout << grid[i][j];
            }
            cout << " ";
        }
        cout << endl;
    }
}

void swapCellwithCell(cell &cell1, cell &cell2) {
    // double checking
    // Check for valid indices before swapping
    if (cell1.x < 0 || cell1.x >= numOfColumns || cell1.y < 0 || cell1.y >= numOfRows ||
        cell2.x < 0 || cell2.x >= numOfColumns || cell2.y < 0 || cell2.y >= numOfRows) {
        cerr << "Error: Invalid cell position encountered before swap." << endl;
        cerr << "cell1.x: " << cell1.x << endl;
        cerr << "cell1.y: " << cell1.y << endl;
        cerr << "cell2.x: " << cell2.x << endl;
        cerr << "cell2.y: " << cell2.y << endl;

        exit(1);
    }

    int tempX = cell1.x;
    int tempY = cell1.y;
    cell1.x = cell2.x;
    cell1.y = cell2.y;
    cell2.x = tempX;
    cell2.y = tempY;
    grid[cell1.y][cell1.x] = cell1.id;
    grid[cell2.y][cell2.x] = cell2.id;
    // update HPWL of nets connected to cell1
    for(int i = 0; i < cell1.nets.size(); i++) {
        computeHPWLofNet(cell1.nets[i]);
    }
    // update HPWL of nets connected to cell2
    for(int i = 0; i < cell2.nets.size(); i++) {
        computeHPWLofNet(cell2.nets[i]);
    }
}


void swapCellWithEmpty(cell &cell1, int emptyY, int emptyX) {
    int tempX = cell1.x;
    int tempY = cell1.y;
    cell1.x = emptyX;
    cell1.y = emptyY;
    grid[cell1.y][cell1.x] = cell1.id;
    grid[tempY][tempX] = -1;
    // update HPWL of nets connected to cell1
    for(int i = 0; i < cell1.nets.size(); i++) {
        computeHPWLofNet(cell1.nets[i]);
    }
}

void swapCells(int cell1Index, int cell2Index, int cell1Row, int cell1Column, int cell2Row, int cell2Column) {
    // cout<<"Swap cells called"<<endl;
    // cout<<"cell1Index: "<<cell1Index<<endl;
    // cout<<"cell2Index: "<<cell2Index<<endl;
    // cout<<"cell1Position: row: "<<cell1Row<<" column: "<<cell1Column<<endl;
    // cout<<"cell2Position: row: "<<cell2Row<<" column: "<<cell2Column<<endl;
    if (cell1Index >=0 && cell1Index < cells.size() && cell2Index >= 0 && cell2Index < cells.size()) {
        // cout<<"Swap two cells that are filled"<<endl;
        // Swap two cells that are filled
        swapCellwithCell(cells[cell1Index], cells[cell2Index]);
    } else  if (cell1Index >=0 && cell1Index < cells.size() && cell2Index == -1){
        // cout<<"Swap a filled cell1 with an empty position cell2"<<endl;
        // Swap a filled cell with an empty position
        swapCellWithEmpty(cells[cell1Index], cell2Row, cell2Column);
    } else if (cell2Index >=0 && cell2Index < cells.size() && cell1Index == -1) {
        // cout<<"Swap a filled cell2 with an empty position cell1"<<endl;
        // Swap an empty position with a filled cell
        swapCellWithEmpty(cells[cell2Index], cell1Row, cell1Column);
    } else if (cell1Index == -1 && cell2Index == -1) {
        // cout<<"Swap two empty positions"<<endl;
        // Both positions are empty, no swap needed
        return;
    } else {
        cerr << "Error: Invalid cell index generated." << endl;
        cerr << "cells size: " << cells.size() << endl;
        cerr << "cell1Index: " << cell1Index << endl;
        cerr << "cell2Index: " << cell2Index << endl;
        exit(1);
    }
}

bool detectTwoEmptyCells() {
    int emptyCellCount = 0;
    for(int i = 0; i < numOfRows; i++) {
        for(int j = 0; j < numOfColumns; j++) {
            if(grid[i][j] == -1) {
                emptyCellCount++;
                if(emptyCellCount == 2) {
                    return true;
                }
            }
        }
    }
    return false;
}

void printCell(cell cell1) {
    cout << "Cell id: " << cell1.id << " row: " << cell1.y << " column: " << cell1.x << endl;
}

void simulateAnnealing(int initialCost) {
    double initialTemperature = 500*initialCost;
    double finalTemperature = 0.000005*initialCost/nets.size();
    double currentTemperature = initialTemperature;
    int moves = 10*cells.size();
    // int moves = 5;
    minstd_rand rng(time(0));
    uniform_int_distribution<int> intRowsRange(0, numOfRows-1);
    uniform_int_distribution<int> intColumnsRange(0, numOfColumns-1);
    uniform_real_distribution<double> doubleDist(0, 1);

    int cell1Row, cell1Column, cell2Row, cell2Column;
    int cell1Index, cell2Index;
    int initialTotalHPWL, newTotalHPWL, deltaHPWL;
    double probability, random_number;
    int count = 0;

    while(currentTemperature > finalTemperature) {
        for(int i = 0; i < moves; i++) {
            // randomly select 2 cells
            cell1Row = intRowsRange(rng);
            cell1Column = intColumnsRange(rng);
            cell2Row = intRowsRange(rng);
            cell2Column = intColumnsRange(rng);

            cell1Index = grid[cell1Row][cell1Column];
            cell2Index = grid[cell2Row][cell2Column];

            initialTotalHPWL = computeTotalWireLength();
            // swap cells
            // if(detectTwoEmptyCells()) {
            //     cout << "Error: Two empty cells detected." << endl;
            //     cout << "helppppppppppp" <<endl;
            // }
            swapCells(cell1Index, cell2Index, cell1Row, cell1Column, cell2Row, cell2Column);
            // if(detectTwoEmptyCells()) {
            //     cout << "Error: Two empty cells detected." << endl;
            //     cout << "helppppppppppp" <<endl;
            // }
            newTotalHPWL = computeTotalWireLength();
            deltaHPWL = newTotalHPWL - initialTotalHPWL;
            if(deltaHPWL >= 0) {
                // reject with probability e^(-deltaHPWL/currentTemperature)
                probability = exp(-1*(double)deltaHPWL/currentTemperature); 
                random_number = doubleDist(rng);
                if(random_number > probability) {
                    // cout<<"reject hence swapping: "<<endl;
                    // cout<<"first cell"<<endl;
                    // printCell(cells[cell1Index]);
                    // cout<<"second cell"<<endl;
                    // printCell(cells[cell2Index]);
                    // cout<<"grid before swapping: "<<endl;
                    // printGrid();
                    swapCells(cell1Index, cell2Index, cell2Row, cell2Column, cell1Row, cell1Column);
                    // cout<<"after swapping: "<<endl;
                    // cout<<"first cell"<<endl;
                    // printCell(cells[cell1Index]);
                    // cout<<"second cell"<<endl;
                    // printCell(cells[cell2Index]);
                    // cout<<"grid after swapping: "<<endl;
                    // printGrid();
                    // if(detectTwoEmptyCells()) {
                    //     cout << "Error: Two empty cells detected." << endl;
                    //     cout << "helppppppppppp" <<endl;
                    // }
                }
            }
        }
        currentTemperature = 0.95 * currentTemperature;
    }
}

int main() {
    string netListFileName = "test.txt";
    
    //start timer
    auto start = high_resolution_clock::now();

    //existing code
    parseNetListFile(netListFileName);
    placeInitiallyRandom();
    printBinaryGrid();
    computeHPWLofAllNets();
    cout << "Total wire length: " << computeTotalWireLength() << endl;
    cout << endl << endl;
    simulateAnnealing(computeTotalWireLength());
    printGrid();
    cout << "Total wire length: " << computeTotalWireLength() << endl;

    //stop timer
    auto stop = high_resolution_clock::now();

    //calculate duration in milliseconds
    auto duration = duration_cast<milliseconds>(stop - start);

    //print time taken
    if (duration.count() < 1000) {
        cout << "Time taken by function: "
             << fixed << setprecision(3) << duration.count() / 1000.0 << " seconds" << endl;
    } else {
        cout << "Time taken by function: "
             << duration.count() / 1000 << " seconds" << endl;
    }

    return 0;
}
