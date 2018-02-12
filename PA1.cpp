#include "PA1.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <bitset>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// http://en.cppreference.com/w/cpp/language/type_alias
using byte = unsigned char ;

// http://en.cppreference.com/w/cpp/types/numeric_limits
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits ;

// http://en.cppreference.com/w/cpp/utility/bitset
using bits_in_byte = std::bitset<BITS_PER_BYTE> ;



using namespace std;

int main(int argc, char* argv[]){



	//ENSURE THAT ALL ARGUMENTS ARE VALID
        if (argc > 6) {
                cerr << "Too many arguments entered. Exiting " << argv[0] << "\n";
                return -1;
        }

        else if(argc < 6) {
                 cerr << "Not enough arguments entered. Exiting " << argv[0] << "\n";
                 return -1;
        }


	string argv1 = argv[1];

	if(argv1 == ("B")){
		//Use Block Cypher
	}

	else if(argv1 == "S"){
		//Use Stream Cypher
	}
	
	else{
		cerr << "Invalid first argument. Only a B or an S will be accepted. Exiting " << argv[0] << "\n";
               	return -1;
	}
	

	string file = "";	

	ifstream infile(argv[2]); //open the file

        //Read the file into the words vector
        if (infile.is_open() && infile.good()) {
		infile.close();
		file = readfile(argv[2]);

	}
	
	else{
                cerr << "Invalid file name for second argument: " << argv[2] << " Exiting "<< argv[0] << "\n";
        	return -1;
	}


	
	ifstream outfile(argv[3]); //open the file

        //Read the file into the words vector
        if (outfile.is_open() && outfile.good()) {
               outfile.close(); 
        }

        else{
                cerr << "Invalid file name for third argument: " << argv[3] << " Exiting "<< argv[0] << "\n";
        	return -1;
	}


	string key = "";

	
	ifstream keyfile(argv[4]); //open the file

        //Read the file into the words vector
        if (keyfile.is_open() && keyfile.good()) {
                keyfile.close();
                key = readfile(argv[4]);
        }

        else{
                cerr << "Invalid file name for fourth argument: " << argv[4] << " Exiting "<< argv[0] << "\n";
        	return -1;
	}


	string argv5 = argv[5];
	if(argv5 == "E"){
                //Use Block Cypher
        }

        else if(argv5 == "D"){
                //Use Stream Cypher
        }

        else{
                cerr << "Invalid fifth argument. Only an E or a D will be accepted. Exiting " << argv[0] << "\n";
                return -1;
        }


	string result = "";


	if(argv1 == "B" && argv5 == "E"){
		if(key.length() < 64){
			cerr << "Key is not 64 bits as required for block cypher. Exiting " 
			<< argv[0] << "\n";
			return -1;
		}
		result = blockencrypt(key, file);
	}

	else if(argv1 == "B" && argv5 == "D"){
		if(key.length() < 64){
                        cerr << "Key is not 64 bits as required for block cypher. Exiting " 
			<< argv[0] << "\n";
                        return -1;
                }
                result = blockdecrypt(key, file);
        }

	else if(argv1 == "S"){
                result = stream(key, file);
        }

	
	writefile(argv[3], result);


	return 0;

}


//TODO FIX SWAP
string blockencrypt(string key, string file){
	//PADDING WORKS AS DESCRIBED
	//cout << "BLOCK ENCRYPT\n";
	//PAD DATA IF NECESSARY
	//cout << file << "\n";
	int padding = file.length() % 64;
	//cout << "padding: " << padding << "\n";
	padding = padding/8;
	//cout << "padding: " << padding << "\n";
	if(padding != 0){
		for(int i = 0; i < padding; i++){
			file = file + "10000000";
		}
		//cout << "file after padding: " << file << "\n";
	}
	
	//cout << "DONE PADDING\n";
	
	//PASS TO XOR
	//XOR WORKS AS DESCRIBED
	string paddedXORed = stringxor(key, file);
	
	//cout << "After xor: " << paddedXORed.length() << "\n";

	//PASS TO SWAP AND RETURiN
	//BROKEN
	string swapped = swap(key, paddedXORed);
	//cout << "After swap: " << swapped.length() << "\n";
	return swapped;
}


//TODO FIX SWAP
string blockdecrypt(string key, string file){
	//cout<< "file: " << file << "\n";
	string swapped = swap(key, file);

	string bits = "";
	for (std::size_t i = 0; i < swapped.size(); ++i){
      		bits += bitset<8>(swapped.c_str()[i]).to_string();
  	}
	
	//cout<< "swapped file: " << bits.length() << "\n";

	string swapXOR = stringxor(key, bits);

	//cout << "file before unpadding: " << swapXOR.length() << "\n";

	//UNPAD DATA
	for(int i = 0; i < swapXOR.length(); i+=8){
		string bytestring = swapXOR.substr(i, 8);
		if(bytestring == "10000000"){
			swapXOR = swapXOR.substr(0, i-8);
		}
	}

	//cout << "file after unpadding: " << swapXOR.length() << "\n";


	char c;
	string result = "";
        for(int i = 0; i < swapXOR.length(); i+=8){
                c = static_cast<char>(std::bitset<8>(swapXOR.substr(i, 8)).to_ulong());
                result += c;
        }
	
	//cout << "End: " << result.length() << "\n";

        return result;
}

//FIXME
string swap(string key, string file){
	//cout << "SWAP\n";
	//cout << "Before swap: " << file.length() << "\n";
	string readytoswap = "";

	//TURNING BINARYY INTO CHARS SEEMS TO WORK AS DEMONSTRATED IN THE STREAM CYPHER
	char c;
	for(int i = 0; i < file.length(); i+=8){
		c = static_cast<char>(std::bitset<8>(file.substr(i, 8)).to_ulong());
		readytoswap += c;
	}

	//cout << "During Swap: " << readytoswap << "\n";

	string keychars = "";
	for(int i = 0; i < key.length(); i+=8){
                c = static_cast<char>(std::bitset<8>(key.substr(i, 8)).to_ulong());
                keychars += c;
        }


	//FIXME
	//THE ISSUE LIES IN THIS SECTION
	//IT IS SOMEHOW GRABBING THE WRONG CHARACTERS FOR THE SWAP
	int j = 0;
	int end = readytoswap.length() - 1;
	string a = "";
	string b = "";
	for(int start = 0; start < end; start++){
		if(((int)keychars[j] % 2) == 1){
			//cout << "Before Swap: " << readytoswap << "\n";
			//std::swap<char>(readytoswap[start], readytoswap[end]);
			a += readytoswap[start];
			//cout << a << "\n";
			b += readytoswap[end];
			//cout << b << "\n";
			
			readytoswap.replace(end, 1, a);
			readytoswap.replace(start, 1, b);
			//cout << "After Swap: " << readytoswap << "\n";
			end--;
			a = "";
			b = "";
		}
		j++;
		if(j == keychars.length()){
			j = 0;
		}
	}

	//cout << file << "\n";
	//cout << "After Swap: " << result.length() << "\n";
	
	return readytoswap;

}

//WORKS PERFECTLY
string stream(string key, string file){
	string XORed = stringxor(key, file);
	string result = "";
	char c;
        for(int i = 0; i < file.length(); i+=8){
                c = static_cast<char>(std::bitset<8>(file.substr(i, 8)).to_ulong());
                result += c;
        }
        return result;
}


//READS FILE INTO A STRING OF BITS.
//WORKS AS TESTED WITH STREAM CYPHER
std::string readfile(string filename){
	std::string bitstring;
    	std::ifstream file(filename, std::ios::binary) ; // open in binary mode

    	char c ;
	int i = 0;
    	while(file.get(c)){
		i++;
		 // read byte by byte
        	bitstring += bits_in_byte(byte(c)).to_string() ; // append as string of '0' '1'
	}
	//cout << "original file: " << i << "\n";
    	return bitstring ;
}


//TESTED WITH STREAM CYPHER AND WORKS
std::string stringxor(string key, string file){
	//cout << "STRINGXOR\n";
	//cout << "key: " << key << "\n";
	//cout << "file: " << file.length() << "\n";
	string result = "";
	int j = 0;
	for(int i = 0; i < file.length(); i++){
		result += (((key[j]-'0') ^ (file[i]-'0')) + '0');
		if(j+1 == key.length()){
			j = 0;
		}
		else{
			j++;
		}
		//cout << "result: " << result <<"\n";
        }

	//cout << "result: " << result.length() <<"\n";
	//cout << "DONE WITH XOR\n";
	return result;
}



void writefile(string filename, string result){
	ofstream writer;
	writer.open(filename);
	writer << result;
	writer.close();
}
