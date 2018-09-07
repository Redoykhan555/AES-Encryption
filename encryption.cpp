#include <bits/stdc++.h>
#include "shared.cpp"
using namespace std;

void encRound(Key &data,Key &rkey,bool mix=true){
    for (int i = 0; i < 4; ++i) {data.words[i] = subWord(data.words[i]);} //cout<<hex<<data.words[i]<<" ";}
    uint8_t state[4][4];
    for (int c = 0; c < 4; ++c)
    {
        for (int r = 0; r < 4; ++r)
        {
            state[r][c] = data.words[c]>>(8*(3-r)) &255;
        }
    }           
	shiftRows(state);
    if(mix){
        for (int i=0;i<4;i++) mixColumn(state,i);
    }

    for (int c = 0; c < 4; ++c)
    {
        data.words[c] = state[0][c];
        for (int r = 1; r < 4; ++r)
        {
            data.words[c] <<= 8;
            data.words[c] |= state[r][c];
        }
    }
    data.xorKey(rkey);
}



void encrypt(Key &block,Key &key){
	block.xorKey(OKey);
    for (int i=1;i<10;i++) encRound(block,GKeys[i]);
    encRound(block,GKeys[10],false);
}

int main(int argc, char const *argv[])
{
	string keyFilePath = "key.k",inpFilePath = "input.txt",outFilePath = "encrypted.enc";
    for (int i = 1; i < argc-1; ++i)
    {
        if(string(argv[i])=="-k") keyFilePath = argv[i+1];
        if(string(argv[i])=="-i") inpFilePath = argv[i+1];
        if(string(argv[i])=="-o") outFilePath = argv[i+1];
    }

    string keyBuffer;
    readFile(keyFilePath,keyBuffer);
    OKey.copy((uint32_t*)&keyBuffer[0]);

	string buffer;
	uint64_t mSize,to_pad;

    mSize = readFile(inpFilePath,buffer);
    to_pad = (mSize+8)%16==0?0:16-(mSize+8)%16;
    buffer.resize(mSize+to_pad+8,0x01); //padding with 1

    char* size_bytes = (char*)&mSize;
    for (int i = 0; i <8 ; ++i)         //8 bytes for size
    	buffer[mSize+to_pad+i] =  size_bytes[i];   

    ofstream outFile (outFilePath,ios::binary);
    for(int i=0;i<mSize+to_pad+8;i+=16){
    	Key data;
    	data.copy((uint32_t*)&buffer[i]);
    	encrypt(data,OKey);
    	outFile.write((char*)data.words,16);
    }
    outFile.close();
	return 0;
}