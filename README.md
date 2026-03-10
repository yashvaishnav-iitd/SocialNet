# SocialNet Simulator

## Project Overview

This project is a command-line simulator for a social network's backend services, developed for the COL106: Data Structures and Algorithms course. The system manages users, friendships, and content with support for friend recommendations and social graph analysis. The primary goal is to apply a practical understanding of AVL Trees and Graphs to a complex, real-world application.

**Author:** Yash Vaishnav
**ENTRY No.:** 2024CS50855

## How to Compile and Run

The project is designed to be compiled in a Linux-like environment (e.g., Linux, macOS, or **Git Bash/WSL on Windows**).

### 1. Navigate to the Directory
Open a bash terminal and use the `cd` command to navigate to the project's root directory where `socialnet.cpp` and `compile.sh` are located.

### 2. Make the Script Executable (First Time Only)
This command gives the system permission to run the script. You only need to do this once.

```bash
chmod +x compile.sh
```

### 3. Compile the Code
Run the compilation script. This will use `g++` to compile the source code.

```bash
./compile.sh
```

### 4. Run the Program
Execute the compiled program to start the social network simulator.

- On **Linux or macOS**, the command is:
```bash
./socialnet
```

- On **Windows (using Git Bash)**, you must include the `.exe` extension:
```bash
./socialnet.exe
```

## Available Commands

### Social Network Operations
- **`ADD_USER <username>`**: Adds a new user to the network, initially with no friends and no posts.
- **`ADD_FRIEND <username1> <username2>`**: Establishes a bidirectional friendship between two existing users.
- **`LIST_FRIENDS <username>`**: Prints an alphabetically sorted list of the specified user's friends.
- **`SUGGEST_FRIENDS <username> <N>`**: Recommends up to N new friends who are "friends of friends" but not already friends. Recommendations are ranked by the number of mutual friends (descending), with ties broken by alphabetical order.
- **`DEGREES_OF_SEPARATION <username1> <username2>`**: Calculates the length of the shortest path of friendships between two users. Returns -1 if no path exists.

### User Content Operations
- **`ADD_POST <username> "<post_content>"`**: Adds a post to the specified user's timeline. Post content must be enclosed in quotes.
- **`OUTPUT_POSTS <username> <N>`**: Outputs the most recent N posts of the user in reverse chronological order. If N is -1, outputs all posts. If fewer than N posts exist, lists all available posts.

### Additional Commands
- **`HELP`**: Displays the list of available commands.

**Note:** Commands and usernames are case-insensitive (e.g., "Lakshay col106" and "lakshay COL106" refer to the same user).

## Hard-Coded Values

The following values are configured in the system's source code:

- **Global Post Timestamp**: Starts at `0` and increments with each new post to maintain chronological ordering.
- **Initial User ID**: `0` (Users are assigned sequential IDs based on the order they are added).
- **Friend Suggestion Ranking**: Primary metric is mutual friend count (descending), with alphabetical username ordering as the tiebreaker.

## Implementation Details & Key Concepts

### Data Structures Used

As per the assignment requirements, the following core data structures were implemented from scratch:

- **Graph (Adjacency List)**: The social network is represented as an undirected graph where each vertex represents a user and each edge represents a friendship relation. Implemented using `vector<vector<int>>` for efficient neighbor iteration.
- **AVL Tree**: Each user's posts are stored in a self-balancing AVL tree, keyed by timestamp. This ensures O(log n) insertion time and efficient retrieval of the most recent N posts through reverse in-order traversal.
- **Max-Heap**: Two custom max-heaps are used for:
  - Sorting friend names alphabetically for the `LIST_FRIENDS` command
  - Ranking friend suggestions by mutual friend count for the `SUGGEST_FRIENDS` command

Note: C++ STL's `unordered_map` is used for username-to-ID mapping as permitted by the assignment specifications.

### Key Semantics

- **Bidirectional Friendship**: When user A adds user B as a friend, the friendship is automatically established in both directions (A→B and B→A).
- **Case-Insensitive Usernames**: Usernames are stored with their original casing but all comparisons are case-insensitive. Custom helper functions (`tolowerChar`, `toLowerString`) handle this conversion.
- **Post Chronology**: A global timestamp counter increments with each post, ensuring unique timestamps and maintaining chronological order even for posts created in rapid succession.
- **Friend Suggestions Algorithm**: Identifies "friends of friends" who are not already friends. Ranking prioritizes users with the highest mutual friend count, with alphabetical ordering breaking ties.
- **Shortest Path Calculation**: Uses Breadth-First Search (BFS) to find the minimum number of friendship connections between two users.

### Assumptions

- **In-Memory Storage**: The social network operates entirely in memory. All data is lost when the program exits.
- **Input Formatting**: User input is assumed to be well-formed. Post content must be enclosed in double quotes.
- **Unique Usernames**: Each username (case-insensitive) can only be registered once.
- **No Self-Friendship**: Users cannot add themselves as friends.
- **No Duplicate Friendships**: The system prevents establishing the same friendship twice.
- **Existing Users**: Operations like `ADD_FRIEND`, `ADD_POST`, etc., assume the specified users already exist in the system.

### Libraries & Technical Choices

- **Standard Libraries Used**: `<iostream>`, `<string>`, `<vector>`, `<sstream>`, `<queue>`, `<unordered_map>`
- **Graph Implementation**: Adjacency list representation was chosen for space efficiency O(V + E) and fast neighbor access, which is critical for BFS traversal and friend suggestion algorithms.
- **AVL Tree Design**: Self-balancing property ensures O(log n) operations for post insertion and retrieval. Reverse in-order traversal naturally yields posts in newest-to-oldest order.
- **Heap Usage**: Max-heap provides O(n log n) sorting for friend lists and suggestion rankings while demonstrating custom data structure implementation skills.
- **Case-Insensitive Implementation**: All case conversions are handled through custom functions rather than external libraries, ensuring portability and demonstrating low-level string manipulation.