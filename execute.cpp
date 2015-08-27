#include <stdio.h>
#include "floatfann.h"
#include <opencv2/opencv.hpp>

//clang++ execute.cpp -o execute -lfann -I/opt/local/include/ -L/opt/local/lib/ `pkg-config --cflags --libs opencv` && ./execute

using namespace cv;
using namespace std;

int main()
{
    fann_type *calc_out;
    

    struct fann *ann = fann_create_from_file("xor_float.net");

    cout << endl;
    cout << "reading image" << endl;
    string filename = "5.png";
    Mat matInput = 255-imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

    cout << "image size=" << matInput.cols << "x" << matInput.rows << endl;

    fann_type input[matInput.cols * matInput.rows];

    int i=0;
    for (int x=0;x<matInput.cols; x++){
        for (int y=0;y<matInput.rows;y++){
            int val = (int)matInput.at<unsigned char>(x,y);
            input[i]=val;
            i++;
            printf("%3d ", val);
            //if (i%28==0) cout <<  endl;
        }
        cout << endl;
    }
    cout << endl;

    calc_out = fann_run(ann, input);

    float max=-1;
    int maxid=-1;
    std::string strCalcs = "";
    for (int i=0; i<10; i++){
        strCalcs += std::to_string(calc_out[i]) + " ";
        //cout << calc_out[i]  << endl;
        if (calc_out[i] > max){
           // cout << " HAVE MAX" << endl;
            max = calc_out[i];
            maxid = i;
        }
    }

    cout << "probably number:" << maxid << " " <<  max*100 << "\% confident" <<  endl;

    printf("recognition test (%s) -> %s\n", filename.c_str(), strCalcs.c_str());


    /*
    float num1=100;
    float num2=num1;

    input[0] = num1;
    input[1] = num2;
    calc_out = fann_run(ann, input);
    printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);

    input[0] = num1;
    input[1] = num1+1;
    calc_out = fann_run(ann, input);
    printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);

    input[0] = num1+1;
    input[1] = num1;
    calc_out = fann_run(ann, input);
    printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);
    */

    fann_destroy(ann);
    return 0;
}


