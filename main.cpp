#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include <filesystem>
#include <sys/stat.h>
#include<sys/types.h>
using namespace std;

string ToString(long long int input){//int to string
    stringstream ss;
    ss<<input;
    string str;
    ss>>str;
    return str;
}

int main(int argc,char* argv[]){
    double start_time=0;//紀錄開始時間
    double end_time=0;//紀錄結束時間
    start_time=clock();
    string command,value,file_name;//存指令、value、file_name是暫存檔名稱
    long long int key,key1,key2;//key存key，key1、key2分別是scan的頭跟尾
    map <long long int,string> tmp_map; //宣告一個tmp_map用來暫存讀取到的map
    map<long long int,string>::iterator it;//迭代器用來暫存尋找map的情況
    fstream file,input_file,output_file;//開三個流，file用來開tmp檔案的流 input_file用來存讀入指令key value output_file用來存取提取的值
    long long int tmp1;//暫存key
    string tmp2;//暫存value
    string output_file_tmp=argv[1];//存讀入的檔名
    int found=output_file_tmp.rfind(".");//計算到./之前的位數
    string output_file_name=output_file_tmp.substr(2,found-1)+"output";//建立新的output name
    int get_or_scan_flag=0;//flag是否get、scan
    input_file.open(argv[1],ios::in);//開啟讀入的檔案
    const char *pathname;//資料夾名
    string directory="./storage";//資料夾名
    pathname=directory.c_str();//資料夾名
    mkdir(pathname,0755);//開啟資料夾


    while (!input_file.eof())//讀取讀入的檔案
    {
        input_file>>command;//檔案指令輸入
        tmp_map.clear();//每次都清空tmp_map
        if (command=="PUT")
        {
            input_file>>key;
            input_file>>value;
            //每一個檔案存100000000
            file_name="./storage/tmp"+ToString(key/10000000)+".txt";//合成tmp name
            file.open(file_name.c_str(),ios::in);//如果有的話就開檔，沒有的話就自己新增檔案
            if (!file)
            {
                file.open(file_name,ios::out|ios::trunc);
                tmp_map[key]=value;
                for (it=tmp_map.begin(); it!=tmp_map.end(); it++)
                {
                    file<<it->first<<" ";
                    file<<it->second<<endl;
                }
                file.close();

            }
            else
            {
                while (!file.eof())//將已經有的檔案讀到tmp_map裡面
                {
                    file>>tmp1;
                    file>>tmp2;
                    tmp_map[tmp1]=tmp2;
                }
                file.close();
                //寫入tmp_map
                tmp_map[key]=value;
                file.open(file_name,ios::out|ios::trunc);//檔案開啟為寫入狀態|如果檔案存在就清除檔案內容的資料
                for (it=tmp_map.begin(); it!=tmp_map.end(); it++)
                {
                    file<<it->first<<" ";
                    file<<it->second<<endl;
                }    
                file.close();
            }
        }
        else if (command=="GET")
        {
            if (get_or_scan_flag==0)//若等於0就存資料
            {
                output_file.open(output_file_name,ios::out);
            }
            
            get_or_scan_flag=1;
            input_file>>key;
            //每一個檔案存100000000
            file_name="./storage/tmp"+ToString(key/10000000)+".txt";
            file.open(file_name,ios::in);
            if (!file)
            {
                output_file<<"EMPTY"<<endl;
            }
            else
            {
                while (!file.eof())
                {
                    file>>tmp1;
                    file>>tmp2;
                    tmp_map[tmp1]=tmp2;
                }
                //read 部分
                it=tmp_map.find(key);
                if(it==tmp_map.end()){
                    output_file<<"EMPTY"<<endl;
                }
                else
                {
                    output_file<<it->second<<endl;
                }
                file.close();
            }
        }
        else if (command=="SCAN")
        {
            if (get_or_scan_flag==0)
            {
                output_file.open(output_file_name,ios::out);
            }
            get_or_scan_flag=1;
            input_file>>key1;
            input_file>>key2;
            long long int last_file_label=-1;
            long long int file_label=-1;
            for (long long int i =key1; i <=key2; i++)
            {
                file_label=i/10000000;
                if (file_label==last_file_label&&file_label!=-1&&last_file_label!=-1)
                {
                    //read 部分 
                    it=tmp_map.find(i);
                    if(it==tmp_map.end()){
                        output_file<<"EMPTY"<<endl;
                    }
                    else
                    {
                        output_file<<it->second<<endl;
                    }
                }
                else
                {
                    tmp_map.clear();
                    if (file_label!=-1)
                    {
                        file.close();
                    }
                    file_name="./storage/tmp"+ToString(file_label)+".txt";
                    file.open(file_name,ios::in);
                    if (!file)
                    {
                        output_file<<"EMPTY"<<endl;
                    }
                    else
                    {
                        while (!file.eof())
                        {
                            file>>tmp1;
                            file>>tmp2;
                            tmp_map[tmp1]=tmp2;
                        }
                        //read 部分
                        it=tmp_map.find(i);
                        if(it==tmp_map.end()){
                            output_file<<"EMPTY"<<endl;
                        }
                        else
                        {
                            output_file<<it->second<<endl;
                        }
                    }
                    last_file_label=file_label;
                }
            }
            file.close();
        }
        else
        {
            cout<<"Please enter again"<<endl;
        }
    }
    input_file.close();
    if (get_or_scan_flag==1)
    {
        output_file.close();
    }    
    end_time=clock();
    cout<<"Time:"<<(end_time-start_time)/CLOCKS_PER_SEC<<"sec"<<endl;
    return 0;
}
