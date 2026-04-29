/*
 * PROJECT: CONNECT-U (Starter Code)
 * Course: ECE367L Data Structures & Algorithms
 * 
 * SAFETY UPDATE:
 * The saveData() function call in main() is currently COMMENTED OUT.
 * This prevents you from accidentally wiping your 'posts.csv' file
 * if your Lab 1 implementation is incomplete.
 * * ONLY uncomment saveData() after you have verified Lab 1 works!
 */

// small change

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

// ==========================================
// MODELS & DATA STRUCTURES
// ==========================================

// Lab 6 Comment struc
struct Comment {
    string username; 
    string content;
    long timestamp;
    Comment* next;

    Comment(string uname, string txt, long time)
        : username(uname), content(txt), timestamp(time), next(nullptr) {}
};

struct Post { 
    int postId;
    int userId;
    string content;
    int likes;
    long timestamp;
    Post* next; 
    Comment* commentsHead; // NEW: Head of the comments linked list

    // UPDATED Constructor
    Post(int pid, int uid, string txt, int lk, long time) 
        : postId(pid), userId(uid), content(txt), likes(lk), timestamp(time), next(nullptr), commentsHead(nullptr) {}
        
    double getScore() {
        long currentTime = time(0); 
        double hoursOld = (currentTime - timestamp) / 3600.0; 
        return (likes * 10.0) + (1000.0 / (hoursOld + 1.0)); 
    }

    // NEW: Add a comment to the FRONT of the list (Most recent at the top)
    void addComment(string uname, string txt) {
        long timeNow = time(0);
        Comment* newComment = new Comment(uname, txt, timeNow);
        
        newComment->next = commentsHead;
        commentsHead = newComment;
    }

    // NEW: Traverse and print comments
    void printComments() {
        if (commentsHead == nullptr) {
            cout << "\n--- Comments ---\n  (No comments)\n----------------\n";
            return;
        }
        
        cout << "\nComments Section:" << endl;
        Comment* curr = commentsHead;

        cout << "--------------------------\n";
        while (curr != nullptr) {

            cout << " @" << curr->username << ": " << curr->content << endl;
            cout << "--------------------------\n";
            curr = curr->next;
        }
        cout << "" << endl;
    }
};

// Helper function that converts time into readable format
string timeAgo(long timestamp) {
    long currentTime = time(0); // Get current time in seconds
    long diff = currentTime - timestamp; // Difference between current time and time a post was made

    if (diff < 60) {
        return "Just now";
    } else if (diff < 120) {
        return "1 minute ago"; // For grammar purposes
    } else if (diff < 3600) {
        return to_string(diff / 60) + " minutes ago";
    } else if (diff < 86400) {
        return to_string(diff / 3600) + " hours ago";
    } else if (diff < 604800) { 
        return to_string(diff / 86400) + " days ago";
    } else {
        // Uses date if it's been more than a week
        time_t t = timestamp;
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d", localtime(&t));
        return string(buffer);
    }
}

// TODO: LAB 1 - Linked List
class Timeline {
public:
    Post* head;
    Timeline() : head(nullptr) {}

    // Task: Add a new post to the FRONT of the list (O(1))
    void addPost(int pid, int uid, string content, int likes, long time) {
        // TODO: LAB 1
        // Create a new Post node 
        Post* newPost = new Post(pid, uid, content, likes, time);
        
        // Point the new node's pointer to the current head
        newPost->next = head;
        
        // Update head to be this new node
        head = newPost;
    }

    void printTimeline() {
        Post* current = head;
        if (!current) { cout << "  (No posts yet)" << endl; return; }
        
        // Task: Traverse the linked list and print content
        // TODO: LAB 1
        while (current != nullptr) {
        cout << "-----------------------------------" << endl; // Divider for aesthetics
        cout << "ID: " << current->userId << "\n";
        // Print the post
        cout << current->content << endl;
        // Print timestamp and likes
        cout << "Time: " << timeAgo(current->timestamp);
        cout << "\nLikes: " << current->likes << endl;
        

        // Move the pointer to the next node in the list
        current = current->next;
        }
        cout << "-----------------------------------" << endl;
    }
};

// Forward Declaration
class User;

// TODO: LAB 4 - Binary Search Tree
struct BSTNode {
    User* user;
    BSTNode* left;
    BSTNode* right;
    BSTNode(User* u) : user(u), left(nullptr), right(nullptr) {}
};

class FriendBST {
public:
    BSTNode* root;
    FriendBST() : root(nullptr) {}

    BSTNode* insert(BSTNode* node, User* u) ;

    void printInOrder(BSTNode* node);

    void addFriend(User* u) { root = insert(root, u); }

    void printFriends() {
        if (root == nullptr) cout << "  (No friends yet)" << endl;
        else printInOrder(root);
    }
};

class User {
public:
    int userId;
    string username;
    int techScore, artScore, sportScore;
    
    Timeline timeline;       // Lab 1
    vector<User*> friends;   // Graph
    FriendBST friendTree;    // Lab 4

    User(int id, string name, int t, int a, int s) 
        : userId(id), username(name), techScore(t), artScore(a), sportScore(s) {}

    void addPost(int pid, string content, int likes, long time) {
        timeline.addPost(pid, userId, content, likes, time);
    }

    void addFriend(User* u) {
        friends.push_back(u);       
        friendTree.addFriend(u);    
    }
    
    vector<User*> getFriendsList() { return friends; }
};

// BST Implementation
BSTNode* FriendBST::insert(BSTNode* node, User* u) {
    // If the tree is empty, return a new node
    if (node == nullptr) {
        return new BSTNode(u);
    }

    // Compare usernames to decide whether to go Left or Right
    if (u->username < node->user->username) {
        node->left = insert(node->left, u);
    } else if (u->username > node->user->username) {
        node->right = insert(node->right, u);
    }

    return node;
}

void FriendBST::printInOrder(BSTNode* node) {
    // If the node is null, do nothing
    if (node == nullptr) {
        return;
    }

    // In-order Traversal (Left -> Root -> Right)
    printInOrder(node->left);
    cout << "- " << node->user->username << endl;
    printInOrder(node->right);
}

// TODO: LAB 3 - Max Heap
class FeedHeap {
private:
    Post* heap[1000]; 
    int size;

    void heapifyDown(int index) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int largest = index;

        // Check if the left child exists and has a larger score
        if (leftChild < size && heap[leftChild]->getScore() > heap[largest]->getScore()) {
            largest = leftChild;
        }

        // Check if the right child exists and has a larger score
        if (rightChild < size && heap[rightChild]->getScore() > heap[largest]->getScore()) {
            largest = rightChild;
        }

        // Bring the node down if it's smaller than its children 
        if (largest != index) {
            swap(heap[index], heap[largest]);
            heapifyDown(largest); // Recursively check the next children
        }
    }
    
    void heapifyUp(int index) {
        if (index == 0) return; // If root node has been reached
        
        int parentIndex = (index - 1) / 2; // Formula to find parent in an array 
        
        // If child post has higher score than parent, swap them
        if (heap[index]->getScore() > heap[parentIndex]->getScore()) {
            swap(heap[index], heap[parentIndex]);
            heapifyUp(parentIndex); // Recursively check the next parent
        }
    }

    

public:
    FeedHeap() : size(0) {}

    void push(Post* p) { 
        if (size >= 1000) return; // Prevent array going out of bounds
        
        heap[size] = p;   // Insert at the next available spot
        heapifyUp(size);  // Bring it up to the correct spot
        size++;           // Increase the size of our heap
    }
    Post* popMax() { 
        if (size == 0) return nullptr;
        
        // Return the highest score (root node) and fix the heap 
        Post* maxPost = heap[0];
        
        heap[0] = heap[size - 1]; // Move the very last post to the root
        size--;                   // Decrease the size of the heap
        
        heapifyDown(0);           // Bring the new root down to its proper place
        
        return maxPost; 
    }
    bool isEmpty() { return size == 0; }
};

vector<User*> allUsers;

// TODO: LAB 2 - Hash Map
// TODO: LAB 2 - Hash Map
struct HashNode {
    string key;
    User* value;
    HashNode* next;
    HashNode(string k, User* v) : key(k), value(v), next(nullptr) {}
};

class UserMap {
private:
    static const int TABLE_SIZE = 10007; 
    HashNode** table;

    unsigned long hashFunction(string key) {
        // TODO: LAB 2
        unsigned long hashValue = 0;
        int p = 31; // Prime number needed to turn strings into a psuedo-random integer to avoid collisions 
        
        // Loop through each character to calculate the polynomial hash
        for (char c : key) {
            hashValue = (hashValue * p + c) % TABLE_SIZE;
        }
        
        return hashValue; 
    }

public:
    UserMap() {
        table = new HashNode*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    // Chaining method to handle collisions 
    void put(string key, User* user) { /* TODO: LAB 2 */ 
        unsigned long index = hashFunction(key);
        
        // Create a new node for the user
        HashNode* newNode = new HashNode(key, user);
        
        // Insert at the head of the linked list to handle collisions
        newNode->next = table[index];
        table[index] = newNode;
    }

    User* get(string key) {
        // Get the array index using the hash function
        unsigned long index = hashFunction(key);
        
        // Go directly to that specific bucket in the table
        HashNode* current = table[index];
        
        // Go through the Linked List at that bucket (if there are collisions)
        while (current != nullptr) {
            if (current->key == key) {
                return current->value; // If a user has been found
            }
            current = current->next;
        }
    
        // TODO: LAB 2 - REPLACE ABOVE WITH HASH LOOKUP
        return nullptr;
    }
};

UserMap userMap;

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

vector<string> split(string s) {
    vector<string> tokens;
    string token;
    bool inQuotes = false;
    
    for (char c : s) {
        if (c == '"') {
            inQuotes = !inQuotes; // Toggle quote state
            continue; // Skip the quote character itself
        }
        
        if (c == ',' && !inQuotes) {
            // Found a delimiter outside of quotes -> New Token
            tokens.push_back(token);
            token.clear();
        } else {
            // Regular character or comma inside quotes
            token += c;
        }
    }
    tokens.push_back(token); // Add last token
    return tokens;
}


int GLOBAL_POST_ID_COUNTER = 1;

Post* findPostById(int id) {
    for (User* u : allUsers) {
        Post* curr = u->timeline.head;
        while (curr != nullptr) {
            if (curr->postId == id) return curr;
            curr = curr->next;
        }
    }
    return nullptr;
}

void createNewPost(User* author, string content) {
    int postId = GLOBAL_POST_ID_COUNTER++;
    long timestamp = time(0); 
    author->addPost(postId, content, 0, timestamp);
    cout << "\n[SUCCESS] Post saved to timeline." << endl;
}

void registerNewUser(string username, int tech, int art, int sport) {
    int newId = allUsers.size() + 1; 
    User* newUser = new User(newId, username, tech, art, sport);
    allUsers.push_back(newUser);
    userMap.put(username, newUser);
    cout << "\n[SUCCESS] Account created." << endl;
}

void addFriendship(User* requester, User* target) {
    requester->addFriend(target);
    target->addFriend(requester);
    cout << "\n[SUCCESS] You are now friends with @" << target->username << endl;
}

// TODO: LAB 5 - Breadth First Search
void recommendFriends(User* startUser) {
    cout << "\n[GRAPH ANALYSIS] Finding friends of friends..." << endl;
    // TODO: LAB 5
    // In case of no recommended friends
    if (startUser == nullptr) return;

    // Use a std::queue<User*> for BFS
    queue<User*> q;
    
    // Use a std::set<int> visited to keep track of seen users
    set<int> visited;

    // Add the starting user to the visited set so they aren't recommended to themselves
    visited.insert(startUser->userId);

    // Start by adding all your direct friends to the queue
    for (User* directFriend : startUser->getFriendsList()) {
        q.push(directFriend);
        // Mark direct friends as visited so we don't recommend people you're already friends with
        visited.insert(directFriend->userId);
    }

    bool found = false;

    // While the queue is not empty:
    while (!q.empty()) {
        // Dequeue a user
        User* curr = q.front();
        q.pop();

        // Look at their friends 
        for (User* fof : curr->getFriendsList()) {
            
            // If that person is not you, and not already your friend (not in the visited set)
            if (visited.find(fof->userId) == visited.end()) {
                
                // Print them as a recommendation
                cout << "- " << fof->username << endl;
                
                // Mark them as visited immediately to avoid printing duplicates
                visited.insert(fof->userId);
                found = true;
            }
        }
    }

    if (!found) {
        cout << "  (No recommendations found.)" << endl;
    }
}


// ==========================================
// FILE I/O 
// ==========================================

void loadData() {
    cout << "Loading data from CSV files..." << endl;
    ifstream userFile("users.csv");
    string line;
    if (userFile.is_open()) {
        getline(userFile, line); 
        while (getline(userFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 5) continue;
            User* newUser = new User(stoi(row[0]), row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]));
            allUsers.push_back(newUser);
            userMap.put(row[1], newUser); 
        }
        userFile.close();
    }
    ifstream relFile("relations.csv");
    if (relFile.is_open()) {
        getline(relFile, line); 
        while (getline(relFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 2) continue;
            int u1 = stoi(row[0]); int u2 = stoi(row[1]);
            if (u1 <= allUsers.size() && u2 <= allUsers.size()) {
                allUsers[u1-1]->addFriend(allUsers[u2-1]);
                allUsers[u2-1]->addFriend(allUsers[u1-1]);
            }
        }
        relFile.close();
    }
    ifstream postFile("posts.csv");
    if (postFile.is_open()) {
        getline(postFile, line); 
        while (getline(postFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 5) continue;
            int pid = stoi(row[0]); int uid = stoi(row[1]);
            if (uid <= allUsers.size()) {
                allUsers[uid-1]->addPost(pid, row[2], stoi(row[3]), stol(row[4])); 
                if (pid >= GLOBAL_POST_ID_COUNTER) GLOBAL_POST_ID_COUNTER = pid + 1;
            }
        }
        postFile.close();
    }
    // Lab 6
    ifstream commFile("comments.csv");
    if (commFile.is_open()) {
        getline(commFile, line); // Skip header
        while (getline(commFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 4) continue;

            int pid = stoi(row[0]);
            string uname = row[1];
            string content = row[2];
            // Note: We don't need to save/load the timestamp manually if we 
            // modify addComment to accept a timestamp, but for simplicity, 
            // let's just add it to the post:
            
            Post* targetPost = findPostById(pid);
            if (targetPost) {
                // We add it to the post's linked list
                targetPost->addComment(uname, content);
            }
        }
        commFile.close();
    }
}

void saveData() {
    // SAFETY CHECK: If no users exist, do not overwrite files!
    if (allUsers.empty()) {
        cout << "[SAFETY] No data in memory. Skipping save to prevent file wipe." << endl;
        return;
    }

    cout << "Saving data..." << endl;
    ofstream userFile("users.csv");
    userFile << "user_id,username,tech_score,art_score,sport_score\n";
    for (User* u : allUsers) {
        userFile << u->userId << "," << u->username << "," << u->techScore << "," << u->artScore << "," << u->sportScore << "\n";
    }
    userFile.close();

    ofstream relFile("relations.csv");
    relFile << "user_id_1,user_id_2\n";
    for (User* u : allUsers) {
        for (User* f : u->friends) {
            if (u->userId < f->userId) relFile << u->userId << "," << f->userId << "\n";
        }
    }
    relFile.close();

    ofstream postFile("posts.csv");
    postFile << "post_id,user_id,content,likes,timestamp\n";
    for (User* u : allUsers) {
        vector<Post*> temp;
        Post* curr = u->timeline.head;
        while (curr) { temp.push_back(curr); curr = curr->next; }
        for (int i = temp.size() - 1; i >= 0; i--) {
            Post* p = temp[i];
            string safeContent = p->content;
            if (safeContent.find(',') != string::npos) {
                safeContent = "\"" + safeContent + "\"";
            } 
            postFile << p->postId << "," << p->userId << "," << safeContent << "," << p->likes << "," << p->timestamp << "\n";
        }
    }
    postFile.close();
    cout << "Done." << endl;

    // Save Comments
    cout << "Saving comments..." << endl;
    ofstream commFile("comments.csv");
    commFile << "post_id,username,content,timestamp\n";
    for (User* u : allUsers) {
        Post* currPost = u->timeline.head;
        while (currPost) {
            Comment* currComm = currPost->commentsHead;
            while (currComm) {
                string safeComm = currComm->content;
                if (safeComm.find(',') != string::npos) safeComm = "\"" + safeComm + "\"";
                
                commFile << currPost->postId << "," << currComm->username << "," 
                         << safeComm << "," << currComm->timestamp << "\n";
                currComm = currComm->next;
            }
            currPost = currPost->next;
        }
    }
    commFile.close();
    cout << "Done." << endl;

}

// ==========================================
// MAIN MENU (UI)
// ==========================================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showUserDashboard(User* currentUser) {
    int choice = 0;
    while (choice != 7) {
        cout << "\n--- Welcome, @" << currentUser->username << " ---" << endl;
        cout << "1. View My Post (Lab 1)" << endl;
        cout << "2. Create New Post (Lab 1)" << endl;
        cout << "3. Add Friend (Lab 2)" << endl;
        cout << "4. Algorithmic Feed (Lab 3)" << endl;
        cout << "5. View Friends Sorted (Lab 4)" << endl;
        cout << "6. Get Friend Recommendations (Lab 5)" << endl;
        cout << "7. Logout" << endl;
        cout << "Select >> ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n[MY POST]" << endl;
            currentUser->timeline.printTimeline();
        }
        else if (choice == 2) {
            cout << "\nEnter post content: ";
            cin.ignore(); 
            string content;
            getline(cin, content);
            createNewPost(currentUser, content);
        }
        else if (choice == 3) {
            string friendName;
            cout << "Enter username to add: "; cin >> friendName;
            User* target = userMap.get(friendName);
            if(target && target != currentUser) addFriendship(currentUser, target);
            else cout << "Invalid user (or Hash Map not implemented)." << endl;
        }
        else if (choice == 4) {
            cout << "\n[ALGORITHMIC FEED]" << endl;
            FeedHeap feed;
            vector<User*> friends = currentUser->getFriendsList();
            for(User* f : friends) {
                Post* p = f->timeline.head;
                int limit = 0;
                while(p != nullptr && limit < 5) {
                    feed.push(p);
                    p = p->next;
                    limit++;
                }
            }
            int count = 0;
            while(!feed.isEmpty() && count < 10) {
                Post* top = feed.popMax();
                if(top)
                    cout << "  > [ID: " << top->postId << "] [Score: " << (int)top->getScore() << "] @" 
                         << allUsers[top->userId - 1]->username << ": " << top->content 
                         << " (" << top->likes << " likes)" << endl;
                count++;
            }
            if(count == 0) cout << "  No posts found." << endl;
            else {
                cout << "\nWould you like to \n(1) Like \n(2) View Comments \n(3) Return to Main Menu\n";
                char resp; cin >> resp;
                
                if (resp == '1') {
                    int pid; cout << "Enter Post ID: "; cin >> pid;
                    Post* p = findPostById(pid);
                    if (p) { p->likes++; cout << "Liked!" << endl; }
                    else { cout << "Post not found." << endl; }
                }
                else if (resp == '2') {
                    int pid; cout << "Enter Post ID to view comments: "; cin >> pid;
                    Post* p = findPostById(pid);
                    
                    if (p) {
                        p->printComments(); // Shows comments from newest to oldest
                        
                        cout << "\nDo you want to make a comment? (y/n): ";
                        char addComm; cin >> addComm;
                        if (addComm == 'y' || addComm == 'Y') {
                            cout << "Enter comment: ";
                            cin.ignore(); // Clear the newline character from the buffer
                            string commentTxt;
                            getline(cin, commentTxt);
                            
                            p->addComment(currentUser->username, commentTxt);
                            cout << "Comment added!" << endl;
                        }
                    } else {
                        cout << "Post not found." << endl;
                    }
                }
            }
        }
        else if (choice == 5) {
            cout << "\n[MY FRIENDS]" << endl;
            currentUser->friendTree.printFriends();
        }
        else if (choice == 6) {
             recommendFriends(currentUser);
        }
        else if (choice == 7) {
            cout << "Logging out..." << endl;
        }
    }
}

void showMainMenu() {
    int choice = 0;
    while (choice != 3) {
        cout << "\n=== CONNECT-U ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit & Save" << endl;
        cout << "Select >> ";
        cin >> choice;

        if (choice == 1) {
            string username;
            cout << "Username: "; cin >> username;
            User* user = userMap.get(username);
            if (user) showUserDashboard(user);
            else cout << "User not found." << endl;
        } 
        else if (choice == 2) {
            string username;
            int t, a, s;
            cout << "Username: "; cin >> username;
            cout << "Tech/Art/Sport (1-10): "; cin >> t >> a >> s;
            registerNewUser(username, t, a, s);
        }
        else if (choice == 3) {
            // SAFETY: Commented out to prevent data loss on initial run.
            // Students must uncomment this ONLY when Lab 1 is complete.
            saveData(); 
            cout << "Goodbye! " << endl;
        }
    }
}

int main() {
    loadData(); 
    clearScreen();
    showMainMenu();
    return 0;
}