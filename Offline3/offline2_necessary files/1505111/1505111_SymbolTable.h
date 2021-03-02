#include<iostream>
#include<fstream>
#include <cmath>
#include <cstring>
#include<cstdio>
#include<cstdlib>
#include <sstream>
using namespace std;
typedef unsigned long long int ull;
///ofstream output;
 FILE *logout;
///------------------------------------------------------
class SymbollInfo
{
private:
    string name;
    string type;
public:
    SymbollInfo *next;

    SymbollInfo(string name, string type)
    {
        this->name=name;
        this->type=type;
        this->next=0;
        
    }
    SymbollInfo(string name)
    {
        this->name=name;
    }
    
    ~SymbollInfo()
    {

    }
    void setName(string name)
    {
    this->name = name;
}
    string getName(){
    return this->name;
}
    void setType(string type){
    this->type = type;
}
    string getType(){
    return this->type;
}
};

//---------------------------------------------------

///-------------------------------------------------
class ScopeTable
{
public:
    SymbollInfo **hashTable;
    int num_of_buckets;
    ScopeTable *parentScopeTable;
    int scopeId;

    ScopeTable(int n, int id)
    {
        scopeId =id;
        num_of_buckets=n;
        hashTable = new SymbollInfo*[n];
        for(int i=0; i<n; i++)
        {
            hashTable[i]=0;
        }
    }
    ~ScopeTable()
    {
        delete hashTable;
    }
    ull djb2(string s){
    ull h = 5381;
    int l = s.length();

    for (int i = 0; i < l; i++)
        h += ((h * 33) + s[i]);

    return h%num_of_buckets;
}
    bool Insert(string name, string type){
//cout<<num_of_buckets<<endl;
//cout<<scopeId<<endl;
    SymbollInfo *ob = new SymbollInfo(name,type);
    int hash_idx=djb2(name);
    if(hashTable[hash_idx] ==0) //this bucket for hash_idx is empty
    {
        hashTable[hash_idx]=ob;
        //cout<<endl<<" Inserted in ScopeTable# "<<scopeId<<" at position "<<hash_idx<<", 0"<<endl;
       /// output<<endl<<" Inserted in ScopeTable# "<<scopeId<<" at position "<<hash_idx<<", 0"<<endl<<endl;
       // fprintf(logout,"\n Inserted in ScopeTable# %d at position %d, 0\n\n",scopeId,hash_idx);

    }
    else
    {
        SymbollInfo *temp=hashTable[hash_idx];
        int pos_at_row_i = 1;
        while(true)
        {
            if(temp->getName() == name) //name already exists
            {
//                cout<<"name alreadfy exists"<<endl;
                //output<<"name alreadfy exists"<<endl<<endl;
                fprintf(logout,"name alreadfy exists\n\n");

                return false;
            }
            if(temp->next == 0)
                break;
            temp=temp->next;          //insert at end of chain
            pos_at_row_i++;
        }
        temp->next = ob;

//            cout<<endl<<" Inserted in ScopeTable# "<<scopeId<<" at position "<<hash_idx<<", "<<pos_at_row_i<<endl;
            ///output<<endl<<" Inserted in ScopeTable# "<<scopeId<<" at position "<<hash_idx<<", "<<pos_at_row_i<<endl<<endl;
          //  fprintf(logout,"\n Inserted in ScopeTable# %d at position %d, %d\n\n",scopeId,hash_idx,pos_at_row_i);

    }
    return true;
}
    SymbollInfo* LookUp(string name){
    for(int i=0; i<num_of_buckets; i++)
    {
        SymbollInfo *temp=hashTable[i];
        int pos_at_row_i=0;
        while(temp !=0)
        {
            if(temp->getName() == name)
            {
//                cout<<" found in ScopeTable# "<<scopeId<<" at position "<<i<<", "<<pos_at_row_i<<endl;
             ///   output<<" found in ScopeTable# "<<scopeId<<" at position "<<i<<", "<<pos_at_row_i<<endl<<endl;

                return temp;
            }
            pos_at_row_i++;
            temp=temp->next;
        }
    }
//    cout<<" Not Found"<<endl;
    ///output<<" Not Found"<<endl<<endl;
    return 0;
}
    bool Delete(string name){
    SymbollInfo *temp2=0;
    for(int i=0; i<num_of_buckets; i++)
    {
        SymbollInfo *temp=hashTable[i];
        int pos_at_row_i=0;
        while(temp !=0)
        {
            if(temp->getName() == name)
            {
//                cout<<" found in ScopeTable# "<<scopeId<<" at position "<<i<<", "<<pos_at_row_i<<endl;
               /// output<<" found in ScopeTable# "<<scopeId<<" at position "<<i<<", "<<pos_at_row_i<<endl<<endl;
                if(temp2==0)  // first item at row i is deleted
                {
                    hashTable[i]=hashTable[i]->next;
                }
                else
                {
                    temp2->next=temp->next;
                    temp=0;
                    //delete temp;
                }
//                cout<<" Deleted entry at "<<i<<", "<<pos_at_row_i<<" from current ScopeTable"<<endl;
                ///output<<" Deleted entry at "<<i<<", "<<pos_at_row_i<<" from current ScopeTable"<<endl<<endl;
                return true;
            }
            pos_at_row_i++;
            temp2=temp;
            temp=temp->next;
        }
    }
//    cout<<" Not Found"<<endl;
   /// output<<" Not Found"<<endl<<endl;
    return false;
}
    void Print(){
    //cout<<" hashtable for separate chaining: "<<endl;
    for(int i=0; i<num_of_buckets; i++)
    {
//        cout<<""<<i<<" -->  ";
        if(hashTable[i] != 0)
        {
            ///output<<""<<i<<" -->  ";
            fprintf(logout,"%d -->  ",i);
            SymbollInfo *temp=hashTable[i];
            while(temp !=0)
            {
//            cout<<"< "<<temp->getName()<<" : "<<temp->getType()<<" > ";
             ///   output<<"< "<<temp->getName()<<" : "<<temp->getType()<<" > ";
                fprintf(logout,"< %s : %s > ",temp->getName().c_str(),temp->getType().c_str());
                temp=temp->next;
            }
//        cout<<endl;
           /// output<<endl;
           fprintf(logout,"\n");
        }
    }
    ///output<<endl;
    fprintf(logout,"\n");
}

};

//----------------------------------------------------
///symbol table----------------------------------------
class SymbollTable
{
public:
    ScopeTable* tables;
    ScopeTable* currentScopeTable;
    int num_of_buckets;
    SymbollTable(int n)
    {
        num_of_buckets=n;
        currentScopeTable=0;
    }
    void EnterScope(){
    if(currentScopeTable == 0) //no scope is created yet
    {
        currentScopeTable = new ScopeTable(num_of_buckets,  1);
        currentScopeTable->parentScopeTable = 0;
    }
    else
    {
        ScopeTable *temp = new ScopeTable(num_of_buckets, currentScopeTable->scopeId + 1);
        (temp->parentScopeTable) = currentScopeTable;
        currentScopeTable = temp;
//        currentScopeTable = new ScopeTable(num_of_buckets, currentScopeTable->scopeId + 1);
//        currentScopeTable->parentScopeTable = temp;
    }
}
    void ExitScope(){
    if(currentScopeTable !=0 )
    {
        currentScopeTable=currentScopeTable->parentScopeTable;
    }
}
    bool Insert(string name, string type){
    if(currentScopeTable == 0){
        EnterScope();
    }
    //cout<<currentScopeTable->num_of_buckets<<endl;
    //cout<<currentScopeTable->scopeId<<endl;
    if(currentScopeTable->Insert(name,type) == true)
        return true;
    return false;
}
    bool Remove(string name){
    if(currentScopeTable->Delete(name) == true)
        return true;
    return false;
}
    SymbollInfo* LookUp(string name){
    ScopeTable *table = currentScopeTable;
    SymbollInfo *info = table->LookUp(name);
    while(table!=0)
    {
        if( info!=0 )
        {
            return info;
        }
        table=table->parentScopeTable;
    }
    return 0;
}
    void PrintCurrentScopeTable(){
    currentScopeTable->Print();
}
    void PrintAllScopeTable(){
    ScopeTable *table = currentScopeTable;
    while(table!=0)
    {
//        cout<<"ScopeTable # "<<table->scopeId<<endl;
        ///output<<"ScopeTable # "<<table->scopeId<<endl;
        fprintf(logout, "ScopeTable # %d\n",table->scopeId);
        table->Print();
        table=table->parentScopeTable;
    }
    ///output<<endl;
    fprintf(logout,"\n");
}
};
//-----------------------------------------------------

