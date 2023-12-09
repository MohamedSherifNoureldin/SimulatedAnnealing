# Simulated Annealing Placement Algorithm

## Abstract

This C++ program implements a sophisticated simulated annealing-based placement algorithm for the optimization of logic cell spatial arrangements on an integrated circuit (IC) grid. The principal objective is to minimize the total wire length, a critical metric influencing signal delay and power consumption in IC design. This comprehensive README document provides an exhaustive understanding of the program, encompassing dependencies, data structures, algorithmic nuances, implementation intricacies, results, and guidance on building and running the system.

## 1. Dependencies

The program relies on external libraries and standard C libraries:

- **CImg.h:** A robust C++ image processing library.
- **sys/types.h, sys/stat.h, unistd.h:** Standard C libraries for file and directory manipulation.

Ensuring the proper installation and accessibility of these dependencies is crucial for successful compilation and execution.

## 2. Data Structures

### 2.1 `cell` Structure

Represents an individual logic cell with a nuanced set of attributes:

- **ID:** A unique identifier for the cell.
- **Position (x, y):** Cartesian coordinates defining the cell's precise location on the grid.
- **Nets:** A vector containing the IDs of nets to which the cell is connected. Facilitates efficient net traversal during wire length computations.

### 2.2 `net` Structure

Defines a logical connection between cells with an extensive set of attributes:

- **ID:** A unique identifier for the net.
- **Bounding Box Coordinates (minX, minY, maxX, maxY):** Delimits the spatial extent of the net, essential for computing HPWL.
- **HPWL (Half Perimeter Wire Length):** Quantifies the total wire length of the net, providing a comprehensive measure of net congestion.
- **Cells:** A vector containing the IDs of cells connected by the net. Enables swift identification of cells participating in a given net.

## 3. Usage

The program requires a netlist file detailing cell and connection information. The main function orchestrates a sequence of critical steps:

1. **Parsing Netlist:** Extracts and interprets information from the provided netlist file, initializing cells and nets accordingly.
2. **Random Placement:** Positions cells randomly on the grid, laying the foundation for the simulated annealing process.
3. **Initial Wire Length Calculation:** Determines the initial total wire length of the circuit, serving as a baseline for subsequent optimizations.
4. **Simulated Annealing:** Executes the simulated annealing algorithm to optimize cell placement, leveraging probabilistic acceptance of suboptimal solutions.
5. **Visualization:** Generates visualizations of the grid at different simulated annealing iterations, aiding in the qualitative assessment of algorithmic progression.

## 4. Algorithm Overview

### 4.1 Simulated Annealing

The simulated annealing algorithm is a metaheuristic inspired by annealing processes in metallurgy. It traverses the solution space by iteratively accepting or rejecting candidate solutions, allowing exploration of both locally optimal and globally optimal regions. The algorithm relies on a temperature schedule to control the degree of randomness and exploration.

### 4.2 Temperature Schedule

The temperature schedule is a critical component of the simulated annealing algorithm. It determines the probability of accepting worse solutions and gradually decreases over iterations. The schedule consists of the following phases:

#### 4.2.1 Initial Temperature

The initial temperature is set to a high value, allowing the algorithm to explore a broad solution space. This high temperature facilitates escaping local minima and reaching a more diverse set of solutions.

#### 4.2.2 Cooling Rate

The cooling rate defines how fast the temperature decreases during the annealing process. A higher cooling rate results in a quicker reduction of temperature, favoring convergence but potentially overlooking promising regions of the solution space.

#### 4.2.3 Final Temperature

The final temperature is a lower bound, limiting the exploration to a more focused solution space. The gradual reduction in temperature ensures convergence towards an optimal solution.

#### 4.2.4 Metropolis Criterion

The acceptance of worse solutions is governed by the Metropolis criterion, a probabilistic approach. If a proposed solution increases the objective function (e.g., wire length), it is accepted with a probability determined by the current temperature and the magnitude of the increase. This probabilistic acceptance allows the algorithm to escape local minima and explore potentially superior solutions.

### 4.3 Swapping Mechanism

The success of the simulated annealing process hinges on an efficient swapping mechanism. Two primary scenarios govern cell movements:

#### 4.3.1 Cell-to-Cell Swapping

Two cells are randomly selected for potential swapping. The Metropolis criterion is applied to determine whether the swap is accepted, considering the impact on the total wire length. If the proposed swap increases the wire length, it may still be accepted probabilistically based on the current temperature.

#### 4.3.2 Cell-to-Empty-Position Swapping

A filled cell is swapped with an empty position on the grid. Similar to cell-to-cell swapping, the Metropolis criterion is employed to decide whether the swap is accepted. This mechanism ensures the exploration of the solution space while avoiding invalid cell positions.

## 5. Implementation Details

### 5.1 Grid Visualization

The CImg library is leveraged to visualize the grid at different simulated annealing iterations. Cells are represented as colored rectangles, and optional grid lines enhance the visual interpretability of the algorithmic progression.

### 5.2 Temperature Schedule

The annealing process's effectiveness is heavily reliant on a well-tuned temperature schedule. The initial temperature, final temperature, and the cooling rate are crucial parameters that influence the overall exploration and convergence characteristics of the algorithm.

## 6. Results

The program generates essential output files for meticulous analysis:

- **Visualizations:** `images/grid_*.png` showcases grid snapshots during various simulated annealing iterations, offering insights into the dynamic evolution of the cell placements.
- **Data Files:** `Temp_TWL.csv` captures wire length vs. temperature data, providing a quantitative understanding of the annealing process's impact on wire length. `CoolingRate_TWL.csv` illustrates the final wire length vs. cooling rate, aiding in the selection of an optimal cooling rate. `CoolingRate_Temp_TWL.csv` details wire length vs. temperature for distinct cooling rates, facilitating a nuanced exploration of the algorithm's behavior.

## 7. Building and Running

Compile the program using the following command:

```bash
g++ main.cpp -O3 -o main -lX11 -lpthread -std=c++17
```

Execute the program:

```bash
./main
```

The program outputs the total wire length, generates visualizations, and provides data files for meticulous analysis. Additionally, it reports the execution time for the simulated annealing algorithm. Parameters and netlist files can be adjusted to tailor the program to specific circuit layouts.

## 8. Swapping Mechanism

The algorithm employs a sophisticated swapping mechanism crucial for the success of the simulated annealing process. The swapping mechanism encompasses two primary scenarios:

### 8.1 Cell-to-Cell Swapping

Cells are randomly selected from the grid for potential swapping. The algorithm ensures that the chosen cells are valid, i.e., they are within the grid boundaries. The swapping operation involves exchanging the positions of two cells. During this operation, the grid is updated, and the Half Perimeter Wire Length (HPWL) of nets connected to these cells is recalculated. If the proposed swap results in a higher wire length, the swap is accepted probabilistically based on the Metropolis criterion, thereby allowing the algorithm to explore solutions that might lead to a lower wire length.

### 8.2 Cell-to-Empty-Position Swapping

This scenario involves swapping a filled cell with an empty position on the grid. Similar to cell-to-cell swapping, the algorithm ensures the selected cell is valid, and the empty position is within the grid boundaries. The swap operation involves updating the grid, recalculating the HPWL of connected nets, and accepting the swap based on the Metropolis criterion.

## 9. Conclusion

This README provides an exhaustive technical insight into the simulated annealing placement algorithm implemented in C++. The algorithm's sophistication lies in its adept utilization of data structures, the careful orchestration of the simulated annealing process, and a meticulous swapping mechanism. The program's output files facilitate a nuanced analysis of the wire length optimization process, aiding researchers and practitioners in comprehending the intricacies of this cutting-edge IC placement algorithm.