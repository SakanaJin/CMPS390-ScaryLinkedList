#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <math.h>

class Node{
public:
    int data;
    std::string name;
    std::shared_ptr<Node> next;

    Node(int x, std::string y){
        data = x;
        name = y;
        next = NULL;
    }

};

class ScaryLinkedList{
private:
    std::shared_ptr<Node> front;
    std::shared_ptr<Node> indexArray[26];
    int indexLengths[26];
    int length;

    int nametonum(std::string name){
        int temp, sum = 0;
        for(int i = 0; i < 3; i++){
            temp = (name.at(i) - 'a') * pow(26, (2-i));
            sum = sum + temp;
        }
        return sum;
    }

    std::string toLower(std::string name){
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

public:
    ScaryLinkedList(){
        front = NULL;
        length = 0;
        for(int i = 0; i < 26; i++){
            indexLengths[i] = 0;
        }
    }

    void insert(std::string name){
        std::string loweredName = toLower(name);
        char first = loweredName.at(0);
        int index = first - 'a';
        int x = nametonum(loweredName);
        if(front == NULL){
            std::shared_ptr<Node> n = std::make_shared<Node>(x, name);
            front = n;
            indexArray[index] = n;
            length++;
        }
        else if(x < front->data){
            std::shared_ptr<Node> n = std::make_shared<Node>(x, name);
            n->next = front;
            front = n;
            indexArray[index] = n;
            length++;
        }
        else{
            std::shared_ptr<Node> curr = front;
            //if(indexArray[index] != NULL){//get this to work
            //    curr = indexArray[index];
            //}
            std::shared_ptr<Node> prev = curr;
            bool searching = true;
            while(searching){
                if(curr->data < x){
                    if(curr->next == NULL){
                        std::shared_ptr<Node> n = std::make_shared<Node>(x, name);
                        curr->next = n;
                        length++;
                        if(indexArray[index] == NULL || indexArray[index]->data > x){
                            indexArray[index] = n;
                        }
                        searching = false;
                    }
                    else{
                        prev = curr;
                        curr = curr->next;
                    }
                }
                else if(curr->data > x){
                    std::shared_ptr<Node> n = std::make_shared<Node>(x, name);
                    //if(curr == prev){//this might be needed 
                    //    std::shared_ptr<Node> temp = prev->next;
                    //    while(temp->name != name){
                    //        prev = prev->next;
                    //        temp = prev->next;
                    //    }
                    //}
                    n->next = curr;
                    prev->next = n;
                    if(indexArray[index] == NULL || indexArray[index]->data > x){
                        indexArray[index] = n;
                    }
                    length++;
                    searching = false;
                }
                else if(curr->data == x){
                    std::cout << "\nduplicate " << name << '\n';
                    searching = false;
                    return;
                }
            }
        }
        indexLengths[index]++;
    }

    void remove(std::string name){
        std::string lowered = toLower(name);
        std::shared_ptr<Node> curr = front;
        std::shared_ptr<Node> indexptr = indexArray[lowered.at(0) - 'a'];
        if(indexptr != NULL){
            curr = indexptr;
        }
        std::shared_ptr<Node> prev = curr;
        while(curr != NULL){
            if(curr->name == name){
                if(curr == indexptr && curr->next != NULL){
                    indexArray[lowered.at(0) - 'a'] = curr->next;
                }
                else{
                    indexArray[lowered.at(0) - 'a'] = NULL;
                }
                prev->next = curr->next;
                curr = NULL;
                curr.reset();
                return;
            }
            else{
                prev = curr;
                curr = curr->next;
            }
        }
        std::cout << "Name not found";
    }

    int Length(){
        return length;
    }

    int Length(char x){
        return indexLengths[x - 'a'];
    }

    void indexer(){
        for(int i = 0; i < 26; i++){
            if(indexArray[i] == NULL){
                std::cout << "null\n";
            }
            else{
                std::cout << indexArray[i]->name << '\n';
            }
        }
    }

    void showList(){
        std::shared_ptr<Node> curr = front;
        while(curr != NULL){
            std::cout << curr->name << '\n';
            curr = curr->next;
        }
    }

    void showList(char x){
        if(indexArray[x - 'a'] == NULL){
            std::cout << "Index doesn't exist";
            return;
        }
        std::shared_ptr<Node> curr = indexArray[x - 'a'];
        for(int i = 0; i < indexLengths[x - 'a']; i++){
            std::cout << curr->name << '\n';
            curr = curr->next;
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
   return 0;
   
}