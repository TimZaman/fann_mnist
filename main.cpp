#include "fann.h"
#include <iostream>
#include <vector>
#include <fstream>     
 
// clang++ main.cpp -o main -lfann -I/opt/local/include/ -L/opt/local/lib/ && ./main

using namespace std;
int ReverseInt (int i){
	unsigned char ch1, ch2, ch3, ch4;
	ch1=i&255;
	ch2=(i>>8)&255;
	ch3=(i>>16)&255;
	ch4=(i>>24)&255;
	return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
}
int n_rows;
int n_cols;

void ReadMNIST(int NumberOfImages, int DataOfAnImage, vector<vector<int> > &arr, vector<int> &arr_labels){
	//Images
	arr.resize(NumberOfImages, vector<int>(DataOfAnImage));

	ifstream file ("t10k-images-idx3-ubyte", ios::binary);
	ifstream file_labels ("t10k-labels-idx1-ubyte", ios::binary);

	//ifstream file ("train-images-idx3-ubyte", ios::binary);
	//ifstream file_labels ("train-labels-idx1-ubyte", ios::binary);

	if (file.is_open()) {
	    int magic_number=0;
	    int number_of_images=0;
	    n_rows=0;
	    n_cols=0;
	    file.read((char*)&magic_number,sizeof(magic_number));
	    magic_number= ReverseInt(magic_number);
	    file.read((char*)&number_of_images,sizeof(number_of_images));
	    number_of_images= ReverseInt(number_of_images);
	    file.read((char*)&n_rows,sizeof(n_rows));
	    n_rows= ReverseInt(n_rows);
	    file.read((char*)&n_cols,sizeof(n_cols));
	    n_cols= ReverseInt(n_cols);
	    for(int i=0;i<number_of_images;++i){
	        for(int r=0;r<n_rows;++r){
	            for(int c=0;c<n_cols;++c){
	                unsigned char temp=0;
	                file.read((char*)&temp,sizeof(temp));
	                arr[i][(n_rows*r)+c]= (int)temp;
	            }
	        }
	    }
	} else {
		cout << "file (images) read error" << endl;
	}

	if (file_labels.is_open()){
	    //Labels
		arr_labels.resize(NumberOfImages);
	    int label=0;
		int magic_number=0;
		int number_of_images=0;
		n_rows=0;
		n_cols=0;
		file_labels.read((char*)&magic_number,sizeof(magic_number));
		magic_number= ReverseInt(magic_number);
		file_labels.read((char*)&number_of_images,sizeof(number_of_images));
		number_of_images= ReverseInt(number_of_images);

	    for(int i=0;i<number_of_images;++i){

            unsigned char temp=0;
            file_labels.read((char*)&temp, sizeof(temp));
            arr_labels[i] = (int)temp;

	    }

	} else {
		cout << "file (labels) read error" << endl;
	}



}

 

int main(){
	/*
    int k=0;
    for (int x=-100;x<100;x++){
        for (int y=-100;y<100;y++){
            if (y==x) continue;
            printf("%d %d\n%d\n", x,y, x>y ? -1 : 1);
            k++;
        }
    }

    printf("k=%d\n", k);*/


    const unsigned int num_input = 28*28; //784
    const unsigned int num_output = 10;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden1 = 30;
    //const unsigned int num_neurons_hidden2 = 20;
    //const unsigned int num_neurons_hidden3 = 20;
    const float desired_error = (const float) 0.001;
    const unsigned int max_epochs = 10000;
    const unsigned int epochs_between_reports = 10;

	int numsets = 10000;

    vector< vector<int> > ar;
    vector<int> ar_labels;
    ReadMNIST(numsets, num_input, ar, ar_labels);

    
    

	FILE *fp;
	fp=fopen("handwriting.data", "w");
	fprintf(fp, "%d %d %d\n", numsets, num_input, num_output );

	bool printstd=true;

    for (int k=0; k<numsets; k++){
    	//if (k<3){ printstd=false;}
    	if (printstd) { cout << " === NUM = " << ar_labels[k] << " ===" << endl; }
	    for (int i=0;i<ar[k].size();i++){
	    	fprintf(fp, "%d ", (int)ar[k][i] );
	    	
	    	if (printstd){
		    	printf("%3d ", (int)ar[k][i]);
		    	if (i%28==0 && i>1) cout <<  endl;
	    	}
	    }
	    int numnow = ar_labels[k];
	    fprintf(fp, "\n" );
	    for (int i=0;i<10;i++){
			fprintf(fp, "%d ", i==numnow ? 1 : -1 );
	    }
	    fprintf(fp, "\n" );
	    if (printstd){ cout << endl; }
	    
	}

	fclose(fp);







    struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden1, num_output);
    

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC); //Symmetric means it's between -1 and 1
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_file(ann, "handwriting.data", max_epochs, epochs_between_reports, desired_error);
    //int max_neurons=200;
    //int neurons_between_reports=1;
    //fann_cascadetrain_on_file(ann, "handwriting.data", max_neurons, neurons_between_reports, desired_error);

    fann_save(ann, "xor_float.net");

    fann_destroy(ann);

    return 0;
}