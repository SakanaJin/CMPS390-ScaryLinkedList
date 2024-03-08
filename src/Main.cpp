#include <iostream>//input output
#include <fstream>//reading files
#include <string>//reading in strings
#include <memory>//allows for better memory manipulation (especially with smart pointers)
#include <math.h>//allows for exponents without a loop

class Node{
public:
    int data;
    std::string name;
    std::shared_ptr<Node> next;

    Node(int x, std::string y){//standard Node stuff
        data = x;
        name = y;
        next = NULL;
    }

};

class ScaryLinkedList{
private:
    std::shared_ptr<Node> front;
    std::shared_ptr<Node> indexArray[26]; //stores a pointer to the first node of each letter
    int indexLengths[26]; //stores the length of the sections
    int length; 

    int nametonum(std::string name){ //converts the first three letters of a name to a unique num (will cause names like bob and bobby to be duplicates, names also cant be less than three letters)
        int temp, sum = 0;
        for(int i = 0; i < 3; i++){
            temp = (name.at(i) - 'a') * pow(26, (2-i));
            sum = sum + temp;
        }
        return sum;
    }

    std::string toLower(std::string name){//converts all the chars in a str to lowercase
        char c;
        std::string lowered = "";
        for(int i = 0; i < name.length(); i++){
            c = name.at(i);
            if(c <= 'Z' && c >= 'A'){
                c = c - ('Z' - 'z');
            }
            lowered = lowered + c;
        }
        return lowered;
    }

    char toLower(char x){//changes an uppercase char to a lowercase char
        if(x <= 'Z' && x >= 'A'){
            return x - ('Z' - 'z');
        }
        return x;
    }

public:
    ScaryLinkedList(){
        front = NULL;
        length = 0;
        for(int i = 0; i < 26; i++){//C++ doesn't like having arrays without values in them, this fixes that by filling the lengths with zero which is technically true
            indexLengths[i] = 0;
        }
    }

    void insert(std::string name){//inserts a node into the list sorted alphabetically
        std::string loweredName = toLower(name);//for convinience all the names in the list will consist of only lowercase letters
        char first = loweredName.at(0);
        int index = first - 'a';
        int x = nametonum(loweredName);//generates the unique number from the name
        if(front == NULL){//case for if the list has no elements yet
            std::shared_ptr<Node> n = std::make_shared<Node>(x, loweredName);
            front = n;
            indexArray[index] = n;
            length++;
        }
        else if(x < front->data){//case for if the new node is the new front
            std::shared_ptr<Node> n = std::make_shared<Node>(x, loweredName);
            n->next = front;
            front = n;
            indexArray[index] = n;
            length++;
        }
        else{
            std::shared_ptr<Node> curr = front;
            if(indexArray[index] != NULL){//instead of searching the whole array for a place we have the possibility to search just that letters index, this causes an issue addressed later
                curr = indexArray[index];
            }
            std::shared_ptr<Node> prev = curr;
            bool searching = true;
            while(searching){//starts a search
                if(curr->data < x){//new name is lowerdown alphabetically than current
                    if(curr->next == NULL){//case for the tail node
                        std::shared_ptr<Node> n = std::make_shared<Node>(x, loweredName);
                        curr->next = n;
                        length++;
                        if(indexArray[index] == NULL || indexArray[index]->data > x){//handles the index
                            indexArray[index] = n;
                        }
                        searching = false;
                    }
                    else{//moves to next node
                        prev = curr;
                        curr = curr->next;
                    }
                }
                else if(curr->data > x){//name need to be inserted before current
                    std::shared_ptr<Node> n = std::make_shared<Node>(x, loweredName);
                    if(prev == curr){//addresses an issue where if coming from an 'index name' prev and curr could be equal creating an infinite loop
                        prev = front;
                        while(prev->next != curr){//unfortunately prev does have to go through the whole list now
                            prev = prev->next;
                        }
                    }
                    n->next = curr;
                    prev->next = n;
                    if(indexArray[index] == NULL || indexArray[index]->data > x){//handles the index
                        indexArray[index] = n;
                    }
                    length++;
                    searching = false;
                }
                else if(curr->data == x){//alerts the user the name was a duplicate
                    std::cout << "\nduplicate " << name << '\n';
                    searching = false;
                    return;
                }
            }
        }
        indexLengths[index]++;//increments the size of the appropriate index
    }

    void remove(std::string name){//removes a name from the list 
        std::string lowered = toLower(name);
        std::shared_ptr<Node> curr = front;
        char first = lowered.at(0);
        std::shared_ptr<Node> indexptr = indexArray[first - 'a'];
        if(indexptr != NULL){
            curr = indexptr;
        }
        std::shared_ptr<Node> prev = curr;
        while(curr != NULL){
            if(curr->name == name){
                if(curr == indexptr && curr->next != NULL && curr->next->name.at(0) == first){//this handles if your deleting an 'index name' appropriately setting the next node to the index if it fits the criteria
                    indexArray[first - 'a'] = curr->next;
                }
                else{
                    indexArray[first - 'a'] = NULL;
                }
                if(curr == prev){//this covers an error where if you're removing an 'index name' where prev and curr can be equal 
                    prev = front;
                    while(prev->next != curr){
                        prev = prev->next;
                    }
                }
                prev->next = curr->next;
                curr = NULL; //these two lines makes the smart pointer delete the object it 'owns'
                curr.reset();//
                indexLengths[first - 'a']--;
                return;
            }
            else{
                prev = curr;
                curr = curr->next;
            }
        }
        std::cout << "Name not found";
    }

    int Length(){//returns the length of the entire linked list 
        return length;
    }

    int Length(char x){//returns the length of a single index in the linked list
        x = toLower(x);
        return indexLengths[x - 'a'];
    }

    void indexes(){//prints out the 'table of contents'
        for(int i = 0; i < 26; i++){
            if(indexArray[i] == NULL){
                std::cout << "null\n";
            }
            else{
                std::cout << indexArray[i]->name << '\n';
            }
        }
    }

    void showList(){//prints out the entire linked list
        std::shared_ptr<Node> curr = front;
        while(curr != NULL){
            std::cout << curr->name << '\n';
            curr = curr->next;
        }
    }

    void showList(char x){//prints out one index of the linekd list
        x = toLower(x);
        if(indexArray[x - 'a'] == NULL){
            std::cout << "Index doesn't exist";
            return;
        }
        std::shared_ptr<Node> curr = indexArray[x - 'a'];
        for(int i = 0; i < indexLengths[x - 'a']; i++){
            std::cout << curr->name << '\n';
            if(curr->next != NULL){
                curr = curr->next;
            }
        }
    }
};

int main(){
    /*
    std::string filepath;
    std::getline(std::cin, filepath);
    std::fstream file;
    file.open(filepath.c_str());
    std::cout << file.is_open();
    std::string name;
    while(file >> name){
        std::cout << name << "\n";
    }
    file.close();
    */
   /*
   ScaryLinkedList list = ScaryLinkedList();
   list.insert("nancy");
   list.insert("Shabong");
   list.insert("abby"); //breaks jerry joe case
   list.insert("albert"); //breaks jerry joe case
   list.insert("bobby"); //breaks jerry joe case
   list.insert("zzz");
   list.insert("george");
   list.insert("pablo");
   list.insert("keith");
   //list.insert("mark");
   list.insert("denmark");
   list.insert("joe");
   //list.insert("bob");
   list.insert("harry");
   list.insert("mary");
   list.insert("brian");
   list.insert("tom");
   list.insert("jerry");
   list.insert("bullwinkle");
   list.insert("pam");
   list.showList();
   //list.remove("albert");
   //list.showList();
   //list.make();
   //list.indexer();
   //std::cout << list.Length('a');
   //list.showList('a');
    */
   
   ScaryLinkedList list = ScaryLinkedList();
   std::string filepath;
   std::getline(std::cin, filepath);
   std::ifstream file;
   file.open(filepath.c_str());
   if(file.is_open() == 0){
        std::cout << "Invalid filepath";
        return 1;
   }
   std::string name;
   while(file >> name){
        list.insert(name.c_str());
   }
   file.close();
   list.showList();
   list.remove("zack");
   std::cout << '\n';
   //list.indexes();
   //list.showList('v');
   list.showList('z');

   return 0;
}