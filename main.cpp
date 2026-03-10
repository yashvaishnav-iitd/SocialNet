#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <unordered_map>

using namespace std;

// Helper function to conver a character to lower Case
char tolowerChar( char c){
    if (c>= 'A' && c <='Z'){
        return c + ('a'- 'A');
    }
    return c;
}

// Converting a string to LowerCase
void toLowerString (string &s){
    if (s.empty()) return;
    for (size_t i=0 ; i<s.length(); i++){
        s[i]= tolowerChar(s[i]);
    }
}


// Max Heap for sorting usernames and suggestions alphabetically.
template<typename T>
class Maxheap{
    private:
        vector<T> data;

        void HeapifyDown(vector<T> &vec, int n, int i){
            int largest=i;
            int left= 2*i +1;
            int right = 2*i+2;

            if (left<n && vec[largest]<vec[left]){
                largest=left;
            }
            if (right< n && vec[largest]<vec[right]){
                largest=right;
            }
            if (largest!=i){
                T temp = vec[i];
                vec[i]= vec[largest];
                vec[largest]=temp;

                HeapifyDown(vec,n,largest);
            }
        }

        void buildMaxHeap(){
            int n = data.size();
            if (n<2) return;
            for (int i = n/2 - 1 ; i>=0 ; i--){
                HeapifyDown(data,n,i);
            }
        }
    public:
        Maxheap(const vector <T> &inputVec){
            data=inputVec;
            buildMaxHeap();
        }

        T popMax(){
            T maxItem= data[0];
            data[0]=data.back();
            data.pop_back();
            HeapifyDown(data, data.size(),0);
            return maxItem;
        }

        vector<T> getSortedVector(){
            vector<T> sortedData=data;
            int n = sortedData.size();
            for (int i= n-1; i>0;i--){
                T temp= sortedData[0];
                sortedData[0]=sortedData[i];
                sortedData[i]= temp;
                HeapifyDown(sortedData,i,0);
            }
            return sortedData;
        }
        bool isEmpty(){
            return data.empty();
        }
};

// AVL TREE Implementation
class AVLTree {
    private:
        struct Node{
            long long timestamp;
            string content;
            Node* left;
            Node* right;
            int height;

            // Initializer
            Node (long long ts , string c):
                timestamp(ts), content(c), left(nullptr), right(nullptr), height(0){}
            
        };
        Node* root;
        
        // For getting the height of the node
        int getHeight(Node* n){
            if (n==nullptr) {
                return -1;
            }
            return n->height;
        }

        // For updating the Height of the node
        void UpdateHeight (Node* n){
            if (n!=nullptr){
                int Lh=getHeight(n->left);
                int Rh= getHeight(n->right);

                n->height = 1 + ((Lh>Rh) ? Lh : Rh);
            }
        }


        //To get the Balance factor of a node
        int getBalance (Node* n){
            if (n==nullptr) { 
                return 0;
            }
            return getHeight(n->left)-getHeight(n->right);
        }

        // Right rotation about the node y
        Node* rightRotate( Node* y){
            Node* x = y->left;
            Node* T2 = x->right;

            x->right= y ;
            y->left = T2;
            
            UpdateHeight(y);
            UpdateHeight(x);

            return x;
        }


        //Left rotation about the node x
        Node* leftRotate(Node* x){
            Node* y = x->right;
            Node* T2= y->left;

            y->left = x;
            x->right = T2;

            UpdateHeight(x);
            UpdateHeight(y);

            return y;
        }

        // Insertion in an AVL Tree
        Node* insertHelper(Node* node, long long ts, string content){
            if (node==nullptr){
                return new Node(ts,content);
            }

            if (ts < node->timestamp) {
                node->left = insertHelper(node->left, ts, content);
            } 
            else {
                node->right = insertHelper(node->right, ts, content);
            }

            UpdateHeight(node);

            int balance = getBalance(node);

            if (balance >1  && ts < node->left->timestamp){
                return rightRotate(node);
            }

            if (balance<-1  && ts > node->right->timestamp){
                return leftRotate(node);
            }

            if (balance > 1  && ts > node->left->timestamp) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }

            if (balance <-1 && ts < node->right-> timestamp){
                node->right= rightRotate(node->right);
                return leftRotate(node);
            }

            return node;
        }

        //The inOrder in an AVL tree gives an ascending order sorted output. The timestamps in ascending order would mean it would give older to newer
        // so we used reversed inOrder traversal to get the descending order and thus Getting newer to older posts.
        void reverseInorder(Node* node, vector<string> &allposts, int N){
            if (node==nullptr || (N!=-1 && (int)allposts.size()>=N)){
                return;
            }

            // Visit Right subtree (higher timestamp ==> newer posts)
            reverseInorder(node->right, allposts, N);

            if (N != -1 && (int)allposts.size() >= N) {
            return;
        }
            // Visit root ()
            allposts.push_back(node->content);

            if (N != -1 && (int)allposts.size() >= N) {
            return;
        }

            // Visit Left subtree (lower timestamp ==> older posts)
            reverseInorder(node->left,allposts,N);
        }

        //Decontstructor helper
        void clear(Node* node){
            if (node!=nullptr){
                clear(node->left);
                clear(node->right);
                delete node;
            }
        }


    public:
        //Constructor
        AVLTree(): root(nullptr) {}

        //Deconstructor
        ~AVLTree(){
            clear(root);
        }

        //Add a nw post
        void addPost(long long ts, string content){
            root= insertHelper(root, ts , content);
        }

        //Retrieving N recent posts. (from newest to oldest)
        vector<string> getPosts(int N){
            vector<string> allPosts;
            reverseInorder(root,allPosts,N);
            return allPosts; 
        }
};  

// For storing a user details and his/hers posts.
class User {
    public:
        string username;
        AVLTree posts;

        //Initaializer
        User(string name): username(name){}
};

// For storing the no. of friends of a username and comparator for the MaxHeap
struct Suggestion {
    int mutuals;
    string name;

    Suggestion(int m , string s): mutuals(m), name(s){}

    bool operator < (const Suggestion &other) const{
        if (mutuals!=other.mutuals){
            return mutuals < other.mutuals;
        }
        return name>other.name;
    }

    bool operator > (const Suggestion& other) const{
        if (mutuals != other.mutuals) {
            return mutuals > other.mutuals;
        }
        return name < other.name;
    }
};


//Main SocialNet class
class SocialNet{
    private:
        vector<vector<int>> adj;
        vector <User> users;
        unordered_map<string,int> userMap;
        long long globalPostTimestamp;
        
        //Get an int ID according to the order the user is added
        int getID(string username){
            string Lusername=username;
            toLowerString(Lusername);

            if (userMap.find(Lusername)!= userMap.end()){
                return userMap[Lusername];
            }
            return -1;
        }

        // Check if the user exists or not.
        bool findUserAndHandleError(const std::string& username, int& userId) { 
        userId = getID(username); 
        if (userId == -1) { 
            cout << "Error: User '" << username << "' not found." << endl;
            return false; 
        } 
        return true;
    } 

        // To check if two users are friends
        bool areFriends(int u1id, int u2id){
            if (u1id == -1 || u2id == -1) {
                return false;
            }

            for (size_t i = 0; i < adj[u1id].size(); ++i) {
                if (adj[u1id][i] == u2id) {
                    return true;
                }
            }
        return false;
        }
        
    
    public:
        SocialNet(): globalPostTimestamp(0) {}
        
        //Adde a new user
        void addUser(const string &username){

            //Checks if the user already exists or not
            if (getID(username) !=-1){
                cout << "User "" << username << "" already exist." << endl;
                return;
            }

            User newUser(username);
            users.push_back(newUser);
            adj.push_back(vector<int>());

            int newID= users.size()-1;
            string Luser=username;
            toLowerString(Luser);
            userMap[Luser]=newID;

            cout << "User "<< username << " added. " << endl;
        }

        //For adding two users as friensds.
        void addFriend(const string& u1, const string& u2){
            int id1, id2;
            bool u1Found = findUserAndHandleError(u1, id1); 
            bool u2Found = findUserAndHandleError(u2, id2); 

            if (!u1Found  || !u2Found) return;

            if (id1==id2){
                cout<<"Error: Cannot friend yourself. "<<endl;
                return;
            }

            if (areFriends(id1,id2)){
                cout<< "Users "<< u1 << " and "<< u2<< " are already friends. "<< endl;
                return;
            }   
            
            adj[id1].push_back(id2);
            adj[id2].push_back(id1);
            cout << u1 << " and "<< u2 << " have been added as friends. "<< endl;
        }

        //Lists all friends of a person
        void listFriends( string username){
            int uId;
            if (!findUserAndHandleError(username, uId)) return;
            

            vector<string> friendNames;
            for (size_t i =0; i<adj[uId].size(); i++){
                int friendID= adj[uId][i];
                friendNames.push_back(users[friendID].username);
            }

            if (friendNames.empty()){
                cout << "User '"<< username<< "' has no friends."<<endl;
                return;
            }

            Maxheap< string> heap (friendNames);
            vector<string> sortedNames= heap.getSortedVector();

            cout << "Friends of "<< username<< " : "<< endl;
            for (size_t i = 0 ; i< sortedNames.size(); i++){
                cout << sortedNames[i] << endl;
            }
        }


        // For finding how many minimum number of people u1 or u2 has to add to become get suggestion of each other +1 .
        void degreesofSeperation( string u1 , string u2 ) {
            int startId, endId;
            bool u1Found = findUserAndHandleError(u1, startId); 
            bool u2Found = findUserAndHandleError(u2, endId); 

            if (!u1Found  || !u2Found) return;

            if (startId==endId){
                cout << "0 "<< endl;
                return;
            }

            queue< pair<int,int>> q;
            q.push(make_pair(startId,0));

            vector<bool> visited(users.size(), false);
            visited[startId]=true;

            while (!q.empty()){
                pair <int,int> current= q.front();
                q.pop();
                int currentId= current.first;
                int currentDist= current.second;

                for (size_t i= 0; i < adj[currentId].size();i++){
                    int neighbourId= adj[currentId][i];

                    if (neighbourId==endId){
                        cout<< (currentDist+1)<< endl;
                        return;
                    }

                    if (!visited[neighbourId]){
                        visited[neighbourId]=true;
                        q.push(make_pair(neighbourId, currentDist+1));
                    }
                }
            }

            cout<< "-1 "<< endl;
        }

        // For suggesting mutual Friends

        void suggestFriends( string username, int N){
            if (N<=0) return ;

            int uId;
            if (!findUserAndHandleError(username, uId)) return;

            vector<int> suggestionCounts(users.size(),0); //For counting friends of friends which are not user's friends'.

            for (size_t i =0 ; i< adj[uId].size(); i++){
                int friend1Id= adj[uId][i];

                for (size_t j =0 ; j < adj[friend1Id].size(); j++){
                    int friend2Id = adj[friend1Id][j];

                    if (friend2Id == uId) {
                        continue;
                    }
                    
                    bool isAlreadyFriend = areFriends(uId, friend2Id);

                    if (!isAlreadyFriend) {
                        suggestionCounts[friend2Id]++;
                    }
                }
            }

            vector <Suggestion> rankedSuggestions; 
            for (size_t i =0 ; i<suggestionCounts.size(); i++){
                if (suggestionCounts[i]>0){
                    rankedSuggestions.push_back(Suggestion(suggestionCounts[i], users[i].username));
                }
            }

            if (rankedSuggestions.empty()){
                cout<< "No friend suggestions for "<< username<< ". "<< endl;
                return;
            }

            Maxheap<Suggestion> heap(rankedSuggestions);

            cout<< "Friend suggestions for "<< username<< " :"<<endl;
            
            for (int i = 0 ; i<N && !heap.isEmpty();i++ ){
                Suggestion best=heap.popMax();
                cout << best.name<< " (Mutual friends: "<< best.mutuals<< ") "<< endl;
            }
        }

        //For adding a new posts for the user.
        void addPost( string username, string content){
            int uId;
            if (!findUserAndHandleError(username, uId)) return;

            users[uId].posts.addPost(globalPostTimestamp,content);
            globalPostTimestamp++;
            cout<< "Post added for "<< username<< ". "<<endl;
        }

        //For showing the top "N" recently added posts of the user.
        void outputPosts ( string username, int N){
            int uId;
            if (!findUserAndHandleError(username, uId)) return;

            vector <string> posts= users[uId].posts.getPosts(N);

            if (posts.empty()){
                cout << "User "<< username<< " has no posts. "<< endl;
                return;
            }

            cout<< "Posts by "<< username<< " :"<<endl;
            for (size_t i=0; i<posts.size(); i++){
                cout << "- \""<< posts[i]<< "\""<< endl;
            }
        }
        
};


//LIST OF AVAILABLE COMMANDS:
void printHelp() { 
    cout << endl<< "--------SocialNet Simulator--------"<<endl;
    cout << " Available Commands: " << endl;
    cout << "(Commands are case-insensitive)" << endl;
    cout << "   ADD_USER <username>" << endl;
    cout << "   ADD_FRIEND <username1> <username2>" << endl;
    cout << "   LIST_FRIENDS <username>" << endl;
    cout << "   SUGGEST_FRIENDS <username> <N>" << endl;
    cout << "   DEGREES_OF_SEPARATION <username1> <username2>" << endl;
    cout << "   ADD_POST <username> \"<post_content in quotes>\"" << endl;
    cout << "   OUTPUT_POSTS <username> <N>" << endl;
    cout << "   HELP" << endl;
    cout << "   EXIT" << endl;
    cout << "--------------------------------------" << endl;
}

//Start Function
int main() {
    SocialNet net;
    string line;

    cout << "Welcome to SocialNet Simulator!" << endl; 
    printHelp();

    while (getline(cin, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string command;
        ss >> command;
        
        toLowerString(command);

        if (command == "add_user") {
            string username;
            ss >> username;
            if (!username.empty()) {
                net.addUser(username);
            } else {
                cout << "Error: Missing username for add_user." << endl;
                printHelp();
            }
        } 
        else if (command == "add_friend") {
            string user1, user2;
            ss >> user1 >> user2;
            if (!user1.empty() && !user2.empty()) {
                net.addFriend(user1, user2);
            } else {
                cout << "Error: Missing one or more usernames for add_friend." << endl;
                printHelp();
            }
        } 
        else if (command == "list_friends") {
            string username;
            ss >> username;
            if (!username.empty()) {
                net.listFriends(username);
            } else {
                cout << "Error: Missing username for list_friends." << endl;
                printHelp();
            }
        } 
        else if (command == "suggest_friends") {
            string username;
            int N = -999;
            ss >> username;

            if (username.empty()) {
                cout << "Error: Missing username for suggest_friends." << endl;
                printHelp();
            }
            
            else if (!(ss >> N)) { 
                if (ss.eof()) { 
                    cout << "Error: Missing number <N> for suggest_friends." << endl;
                } else { 
                    cout << "Error: Invalid number <N>. Must be a positive integer." << endl;
                }
                printHelp();
            }
            
            else if (N < 0) { 
                cout << "Error: Number <N> for suggest_friends cannot be negative." << endl;
                printHelp();
            }
            else {
                
                net.suggestFriends(username, N);
        } 
    }
        else if (command == "degrees_of_separation") {
            std::string user1, user2;
            ss >> user1 >> user2;
            if (!user1.empty() && !user2.empty()) {
                net.degreesofSeperation(user1, user2);
            } else {
                cout << "Error: Missing one or more usernames for degrees_of_separation." << endl;
                printHelp();
            }
        } 
        else if (command == "add_post") {
            string username;
            ss >> username;
            
            string content;
            char c;
            
            while(ss.get(c) && (c == ' ' || c == '\t')); 
            
            if(c == '"') {
                getline(ss, content, '"');
            }
            
            if (!username.empty() && !content.empty()) { 
                net.addPost(username, content);
            } else if (username.empty()) {
                cout << "Error: Missing username for add_post." << endl;
                printHelp();
            } else {
                cout << "Error: Missing post content (must be in quotes) for add_post." << endl;
                printHelp();
            }
        } 
        else if (command == "output_posts") {
            string username;
            int N = -999; // Initialize to a "magic number"
            ss >> username;
            
            if (username.empty()) {
                cout << "Error: Missing username for output_posts." << endl;
                printHelp();
            }
            
            else if (!(ss >> N)) {
                if (ss.eof()) { 
                    cout << "Error: Missing number <N> for output_posts." << endl;
                } else { 
                    cout << "Error: Invalid number <N>. Must be an integer greater than -2." << endl;
                }
                printHelp();
            }
            
            else if (N < -1) { 
                cout << "Error: Number <N> for output_posts cannot be less than -1." << endl;
                printHelp();
            }
            else {
                // All checks passed
                net.outputPosts(username, N);
            }
        } 
        else if (command == "help") { 
            printHelp(); 
        }
        else if (command=="exit"){
            cout<< "Goodbye!"<<endl;
            break;
        }
        else {
            cout << endl <<"##### Error: Unknown command. #####" << endl<<endl;
            printHelp(); 
        }
    }
    return 0;
}