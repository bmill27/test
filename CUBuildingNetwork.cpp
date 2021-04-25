/*******************************************************************/
/*                CUBuildingNetwork Implementation                 */
/*******************************************************************/
/* TODO: Implement the member functions of class CUBuildingNetwork */
/*       This class uses a linked-list of CUBuilding nodes to      */
/*       represent communication paths between buildings           */
/*******************************************************************/

#include "CUBuildingNetwork.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

/*
 * Purpose: Constructer for empty linked list
 * @param none
 * @return none
 */
CUBuildingNetwork::CUBuildingNetwork() {
    /* finished. do not touch. */
    head = NULL;
}

/*
 * Purpose: Check if list is empty
 * @return true if empty; else false
 */
bool CUBuildingNetwork::isEmpty()
{
/* finished. do not touch. */
    return (head == NULL);
}

/*
 * Purpose: Add a new building to the network
 *   between the building *previous and the Building that follows it in the network.
 * @param previous name of the Building that comes before the new Building
 * @param buildingName name of the new Building
 * @param numOfroom
 * @return none
 */
void CUBuildingNetwork::addBuildingInfo(CUBuilding* previous, string buildingName, int numOfroom) {
    
    CUBuilding* newBuilding = NULL;
    newBuilding = new CUBuilding();

    if(previous == NULL){
        
        newBuilding->next = head;
        head = newBuilding;
        // cout<< "adding: " << buildingName << " (HEAD)" << endl;
    }
    else{
        newBuilding -> next = previous->next;
        previous -> next = newBuilding;
        // cout<< "adding: " << buildingName << " (prev: " << previous->name << ")" << endl;
    }
    newBuilding -> name = buildingName;
    newBuilding -> totalRoom = numOfroom;

}


/*
 * Purpose: populates the network with the predetermined buildings
 * @param none
 * @return none
 */
void CUBuildingNetwork::loadDefaultSetup()
{
    addBuildingInfo(NULL,"FLMG",2);
    addBuildingInfo(searchForBuilding("FLMG"),"DLC",10);
    addBuildingInfo(searchForBuilding("DLC"),"ECOT",6);
    addBuildingInfo(searchForBuilding("ECOT"),"CASE",5);
    addBuildingInfo(searchForBuilding("CASE"),"AERO",4);
    addBuildingInfo(searchForBuilding("AERO"),"RGNT",9);
}

/*
 * Purpose: Search the network for the specified building and return a pointer to that node
 * @param buildingName name of the building to look for in network
 * @return pointer to node of buildingName, or NULL if not found
 *
 */
CUBuilding* CUBuildingNetwork::searchForBuilding(string buildingName)
{
    CUBuilding* ptr = head;
    while (ptr != NULL){
        if(ptr->name == buildingName){
            break;
        }
        ptr = ptr->next;
    }
    return ptr;
}


/*
 * Purpose:
 * @param string receiver
 * @param string msg
 */
void CUBuildingNetwork::transmitRoomInfo(string receiver) {
    CUBuilding* ptr = head;
    if(head == NULL){
        cout<< "Empty list" << endl;
        return;
    }
    if(searchForBuilding(receiver) == NULL){
        cout<< "Building not found" << endl;
        return;
    }
    while(ptr != NULL){
        ptr->message = "available room at " + ptr->name + " is " + to_string(ptr->totalRoom);
        ptr->numberMessages++;
        cout<< ptr->name << " [# messages received: " << ptr->numberMessages << "] received: " << ptr->message << endl;
        if(ptr->name == receiver){
            break;
        }
        ptr = ptr->next;
    }
    
}

/*
 * Purpose: prints the current list nicely
 * @param ptr head of list
 */
void CUBuildingNetwork::printNetwork() {
    CUBuilding* ptr = head;
    
    cout<< "== CURRENT PATH ==" << endl;
    if(head == NULL){
        cout<< "nothing in path" << endl;
    }
    else{
        while(ptr != NULL){
            cout<< ptr->name << "(" << ptr->totalRoom << ") -> ";
            ptr = ptr->next;
        }
        cout<< "NULL" << endl;
    }
    cout<< "===" << endl;
}

/*
 * Purpose: Delete the CUBuilding in the network with the specified name.
 * @param buildingName name of the CUBuilding to delete in the network
 * @return none
 */
void CUBuildingNetwork::deleteCUBuilding(string buildingName) 
{
    CUBuilding* ptr = NULL;
    ptr = searchForBuilding(buildingName);

//EDGE CASE: HEAD
    if(ptr == head){
        head = ptr->next;
        delete ptr;
        return;
    }

//STANDARD CASE
    CUBuilding* searchPtr = head;
    if(ptr == nullptr){
        cout<< "Building does not exist." << endl;
    }
    else{
        while(searchPtr != NULL){
            if(searchPtr->next->name == buildingName){
            
                searchPtr->next = searchPtr->next->next;
            
                break;
            }
            searchPtr = searchPtr->next;
        }
    }

}

/*
 * Purpose: deletes all CUBuildings in the network starting at the head CUBuilding.
 * @param: none
 * @return: none
 */
void CUBuildingNetwork::deleteEntireNetwork()
{
    CUBuilding* ptr = head;
    if(head == nullptr){
        return;
    }
    
    while(ptr != nullptr){
        head = ptr->next;
        cout<< "deleting: " << ptr->name << endl;
        ptr = ptr->next;
    }
    cout<< "Deleted network" << endl;

}

/*
* Purpose: Creates a loop from last node to the CUBuilding specified.
* @param buildingName name of the CUBuilding to loop back
* returns the last node before loop creation (to break the loop)
*/
CUBuilding* CUBuildingNetwork::createLoop(string buildingName) 
{
    int count = 0;
    CUBuilding* ptr = head;

    while(ptr != nullptr){
        count++;
        ptr = ptr->next;
    }

    ptr = head;
    int newCount = 0;

    while(ptr != nullptr){
        newCount++;
        if(newCount == count){
            ptr->next = searchForBuilding(buildingName);

            break;
        }
        ptr = ptr->next;
        
    }
    return ptr;
}

/*
 * Purpose: to detect loop in the linked list
 * @return: true if loop is detected, else false
 */
bool CUBuildingNetwork::detectLoop() 
{
    CUBuilding* temp[100];


    CUBuilding* ptr = head;

    int count = 0;
    while(ptr != nullptr){
        temp[count] = ptr;  

    for(int i=0;i < count;i++){ 
        if(ptr->name == temp[i]->name){
            return true;    
        }   
    }   

        count++;
        ptr = ptr->next;
    }
    return false;
}

/*
 * Purpose: Take the chunk of nodes from start index to end index
 *          Move that chunk to the end of the List
 * @param: start_index, end_index
 * @return: none
 */
void CUBuildingNetwork::readjustNetwork(int start_index, int end_index)
{
//EMPTY LIST CASE
    if(head == nullptr){
        cout<< "Linked List is Empty" << endl;
        return;
    }


    CUBuilding* ptr = head;
    int count = 0;
    
    while(ptr != nullptr){
        count++;
        ptr = ptr->next;
    }

//ERROR CASES
    if(end_index >= count || end_index < 0){
        cout<< "Invalid end index" << endl;
        return;
    }

    if(start_index >= count || start_index < 0){
        cout<< "Invalid start index" << endl;
        return;
    }

    if(start_index > end_index){
        cout<< "Invalid indices" << endl;
        return;
    }
    
//CREATES ARRAY OF NODES
    CUBuilding* temp[count];

    CUBuilding* tempPtr = head;
    for(int i=0;i<count;i++){
        temp[i] = tempPtr;
        tempPtr = tempPtr->next;
    }

//EDGE CASE: START INDEX AT HEAD
    if(start_index == 0){
        head = temp[end_index + 1];
        for(int i=start_index;i <= end_index;i++){
        
            if(i == start_index){
                addBuildingInfo(searchForBuilding(temp[count - 1]->name),temp[i]->name,temp[i]->totalRoom);
            }
            else{
                addBuildingInfo(searchForBuilding(temp[i-1]->name),temp[i]->name,temp[i]->totalRoom);
            }
        }
        return;
    }
    
//STANDARD CASE
    for(int i=start_index;i <= end_index;i++){

        if(i == start_index){
            addBuildingInfo(searchForBuilding(temp[count - 1]->name),temp[i]->name,temp[i]->totalRoom);
            deleteCUBuilding(temp[i]->name);
        }
        else{
            addBuildingInfo(searchForBuilding(temp[i-1]->name),temp[i]->name,temp[i]->totalRoom);
            deleteCUBuilding(temp[i]->name);
        }
    }
}

/*
 * Purpose: Destructor to delete the entire list on program termination
 * @param none
 * @return none
 */
CUBuildingNetwork::~CUBuildingNetwork()
{
    CUBuilding* current = head;

    while(current != nullptr){
        CUBuilding* next = current->next;

        delete current;
        current = next;
    }
    head = nullptr;

}
