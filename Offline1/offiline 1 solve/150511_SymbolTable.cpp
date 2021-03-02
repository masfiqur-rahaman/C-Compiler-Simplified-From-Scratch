#include<iostream>
#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long int ull;


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
    ~SymbollInfo()
    {

    }
    void setName(string name);
    string getName();
    void setType(string type);
    string getType();
};
void SymbollInfo::setName(string name)
{
    this->name = name;
}
string SymbollInfo::getName()
{
    return this->name;
}
void SymbollInfo::setType(string type)
{
    this->type = type;
}
string SymbollInfo::getType()
{
    return this->type;
}
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
    ull djb2(string s);
    bool Insert(string name, string type);
    SymbollInfo* LookUp(string name);
    bool Delete(string name);
    void Print();

};

ull ScopeTable:: djb2(string s)
{
    ull h = 5381;
    int l = s.length();

    for (int i = 0; i < l; i++)
        h += ((h * 33) + s[i]);

    return h%num_of_buckets;
}

bool ScopeTable::Insert(string name, string type)
{
//cout<<num_of_buckets<<endl;
//cout<<scopeId<<endl;
    SymbollInfo *ob = new SymbollInfo(name,type);
    int hash_idx=djb2(name);
    if(hashTable[hash_idx] ==0) //this bucket for hash_idx is empty
    {
        hashTable[hash_idx]=ob;
        cout<<endl<<" Inserted in ScopeTable# "<<scopeId<<" at position "<<hash_idx<<", 0"<<endl;
    }
    else
    {
        SymbollInfo *temp=hashTable[hash_idx];
        int pos_at_row_i = 0;
        while(true)
        {
            if(temp->getName() == name) //name already exists
            {
                cout<<"name alreadfy exists"<<endl;
                return false;
            }
            if(temp->next == 0)
                break;
            temp=temp->next;          //insert at end of chain
            cout<<endl<<" Inserted in ScopeTable# "<<scopeId<<" at position "<<hash_idx<<", "<<pos_at_row_i<<endl;
            pos_at_row_i++;
        }
        temp->next = ob;
    }
    return true;
}
SymbollInfo* ScopeTable::LookUp(string name)
{
    for(int i=0; i<num_of_buckets; i++)
    {
        SymbollInfo *temp=hashTable[i];
        int pos_at_row_i=0;
        while(temp !=0)
        {
            if(temp->getName() == name)
            {
                cout<<" found in ScopeTable# "<<scopeId<<" at position "<<i<<", "<<pos_at_row_i<<endl;
                return temp;
            }
            pos_at_row_i++;
            temp=temp->next;
        }
    }
    cout<<" Not Found"<<endl;
    return 0;
}
bool ScopeTable::Delete(string name)
{
    SymbollInfo *temp2=0;
    for(int i=0; i<num_of_buckets; i++)
    {
        SymbollInfo *temp=hashTable[i];
        int pos_at_row_i=0;
        while(temp !=0)
        {
            if(temp->getName() == name)
            {
                cout<<" found in ScopeTable# "<<scopeId<<" at position "<<i<<", "<<pos_at_row_i<<endl;
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
                cout<<" Deleted entry at "<<i<<", "<<pos_at_row_i<<" from current ScopeTable"<<endl;
                return true;
            }
            pos_at_row_i++;
            temp2=temp;
            temp=temp->next;
        }
    }
    cout<<" Not Found"<<endl;
    return false;
}
void ScopeTable:: Print()
{
    //cout<<" hashtable for separate chaining: "<<endl;
    for(int i=0; i<num_of_buckets; i++)
    {
        cout<<""<<i<<" -->  ";
        SymbollInfo *temp=hashTable[i];
        while(temp !=0)
        {
            //cout<<"here"<<endl;
            cout<<"< "<<temp->getName()<<" : "<<temp->getType()<<" > ";
            temp=temp->next;
        }
        cout<<endl;
    }
}
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
    void EnterScope();
    void ExitScope();
    bool Insert(string name, string type);
    bool Remove(string name);
    SymbollInfo* LookUp(string name);
    void PrintCurrentScopeTable();
    void PrintAllScopeTable();
};
void SymbollTable::EnterScope()
{
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
void SymbollTable::ExitScope()
{
    if(currentScopeTable !=0 )
    {
        currentScopeTable=currentScopeTable->parentScopeTable;
    }
}
bool SymbollTable::Insert(string name, string type)
{
    if(currentScopeTable == 0){
        EnterScope();
    }
    //cout<<currentScopeTable->num_of_buckets<<endl;
    //cout<<currentScopeTable->scopeId<<endl;
    if(currentScopeTable->Insert(name,type) == true)
        return true;
    return false;
}
bool SymbollTable::Remove(string name)
{
    if(currentScopeTable->Delete(name) == true)
        return true;
    return false;
}
SymbollInfo* SymbollTable::LookUp(string name)
{
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
void SymbollTable::PrintCurrentScopeTable(){
    currentScopeTable->Print();
}
void SymbollTable::PrintAllScopeTable(){
    ScopeTable *table = currentScopeTable;
    while(table!=0)
    {
        cout<<"ScopeTable # "<<table->scopeId<<endl;
        table->Print();
        table=table->parentScopeTable;
    }
}
//-----------------------------------------------------
int main()
{
    ifstream in_file;
    in_file.open("input.txt");

//    ofstream out_file;
  //  out_file.open("outputMy.txt");

    int n;
    in_file>>n;
    out_file<<n;
    //scanf("%d",&n);
    SymbollTable tables(n);

    string str;
    while(true)
    {
        char ch;
        in_file>>ch;
        out_file<<ch;
        if(ch == 'I')
        {
            string name;
            string type;
            in_file>>name>>type;
            out_file<<name<<type;
            tables.Insert(name,type);
            //tables.Print();

            //cout<<name<<" "<<type<<endl;
        }
        else if(ch == 'L')
        {
            string name;
            in_file>>name;
            SymbollInfo* ob = tables.LookUp(name);
           // if(ob != 0)
             //   cout<<endl<<ob->getName()<<" "<<ob->getType()<<endl;
        }
        else if(ch == 'D')
        {
            string name;
            in_file>>name;
            tables.Remove(name);
        }
        else if(ch == 'P')
        {
            in_file>>ch;
            if(ch == 'A')
            tables.PrintAllScopeTable();///only handled print all
        }
        else if(ch == 'S'){
            tables.EnterScope();
        }
        else if(ch == 'E'){
            tables.ExitScope();
        }

    }
    out_file.close();
    /*
    ScopeTable hashTable1(n);
    string str;
    while(true)
    {
        char ch;
        in_file>>ch;
        if(ch == 'I')
        {
            string name;
            string type;
            in_file>>name>>type;
            hashTable1.Insert(name,type);
            //hashTable1.Print();

            //cout<<name<<" "<<type<<endl;
        }
        else if(ch == 'L')
        {
            string name;
            in_file>>name;
            SymbollInfo* ob = hashTable1.LookUp(name);
            if(ob != 0)
                cout<<endl<<ob->getName()<<" "<<ob->getType()<<endl;
        }
        else if(ch == 'D')
        {
            string name;
            in_file>>name;
            hashTable1.Delete(name);
        }
        else if(ch == 'P')
        {
            hashTable1.Print();
        }
    }*/


}
