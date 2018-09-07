#include <bits/stdc++.h>
#include "shared.cpp"
using namespace std;



void decRound(Key &data,Key &rkey,bool mix=true){
    data.xorKey(rkey);
    uint8_t state[4][4];
    for (int c = 0; c < 4; ++c)
    {
        for (int r = 0; r < 4; ++r)
        {
            state[r][c] = data.words[c]>>(8*(3-r)) &255;
        }
    } 
    //cout<<"state conv"<<endl;
    if(mix){
        for (int i=0;i<4;i++) mixColumn(state,i,true);
    }
    //cout<<"mix col conv"<<endl;

    shiftRows(state,true);
    //cout<<"shift rows conv"<<endl;

    for (int c = 0; c < 4; ++c)
    {
        data.words[c] = state[0][c];
        for (int r = 1; r < 4; ++r)
        {
            data.words[c] <<= 8;
            data.words[c] |= state[r][c];
        }
    }
    for (int i = 0; i < 4; ++i) {data.words[i] = subWord(data.words[i],true);} //cout<<hex<<data.words[i]<<" ";}
}


void decrypt(Key &block,Key &key){
    decRound(block,GKeys[10],false);
    for (int i = 9; i>=1 ; --i) decRound(block,GKeys[i]);
    block.xorKey(OKey);
}

int main(int argc, char const *argv[])
{
    string keyFilePath = "key.k",encFilePath = "encrypted.enc",deFilePath = "out.txt";
    for (int i = 1; i < argc-1; ++i)
    {
        if(string(argv[i])=="-k") keyFilePath = argv[i+1];
        if(string(argv[i])=="-i") encFilePath = argv[i+1];
        if(string(argv[i])=="-o") deFilePath = argv[i+1];
    }

    ifstream keyFile(keyFilePath,ios::binary);
    ifstream enFile (encFilePath,ios::binary);
    ofstream deFile (deFilePath,ios::binary);

    string buffer;
    uint64_t fSize;

    enFile.seekg(0, ios::end);
    fSize = enFile.tellg();
    buffer.resize(fSize);

    enFile.seekg(0, ios::beg);
    enFile.read(&buffer[0],fSize);
    enFile.close();

    for(int i=0;i<fSize;i+=16){
        Key data;
        data.copy((uint32_t*)&buffer[i]);
        decrypt(data,OKey);
        char* bytes = (char*) data.words;
        for(int j=0;j<16;j++) buffer[i+j] = bytes[j];
    }
    cout<<buffer<<endl;
    uint32_t* mSize = (uint32_t*)&buffer[fSize-8];
    cout<<fSize<< " Size:"<<*mSize<<endl;
    deFile.write(&buffer[0],*mSize);
    deFile.close();
    return 0;
}