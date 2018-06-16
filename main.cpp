#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <random>

using namespace std;

struct newron;
struct Synaps {
    newron* link;
    double weight;
    double DWeight;
    Synaps() {
        DWeight = 0;
    }
};
struct newron {

    double input;
    double output;
    double delta;
    Synaps* synaps;
    void getInput(double data) {
        input = data;
    }
    void activate() {
        output = 1/(1+exp(-1*input));
    }

    newron() {
        input = output = 0;
    }
};


class net {
    newron* layer1;
    newron* layer2;
    newron result;
    int Epoch[4][2] = {{1,0}, {0,1}, {0, 0}, {1, 1}};


public:
    net();
    void forwardPass(double input[2]);
    double error(double idealAnswer);
    double outputDelta(double error);
    double hiddenDelta(newron& hidden);
    double inputDelta(newron& input);

    void backpropagation(double answer);

};


net::net() {
    //build net
    layer1 = new newron[2];
    layer2 = new newron[2];
    //first layer
    for (int i = 0; i < 2; i++) {
    layer1[i].synaps = new Synaps[2];

    layer1[i].synaps[0].link = &layer2[0];
    layer1[i].synaps[1].link = &layer2[1];

    layer1[i].synaps[0].weight = 1.0/(rand()%100);
    layer1[i].synaps[1].weight = 1.0/(rand()%100);


   }
    cout << "begin: " << endl;
    cout << "layer1-1-1 weight =  " << layer1[0].synaps[0].weight << endl;
    cout << "layer1-1-2 weight =  " << layer1[0].synaps[1].weight << endl;

    cout << "layer1-2-1 weight =  " << layer1[1].synaps[0].weight << endl;
    cout << "layer1-2-2 weight =  " << layer1[1].synaps[1].weight << endl;




    for (int i = 0; i < 2; i++) {
        layer2[i].synaps = new Synaps;

        layer2[i].synaps->link = &result;
        layer2[i].synaps->weight = 1.0/(rand()%100);
    }
    cout << "layer2-1 weight =  " << layer2[0].synaps->weight << endl;
    cout << "layer2-1 weight =  " << layer2[1].synaps->weight << endl;




}
void  net::forwardPass(double input[2]) {
    int i;
    for (i = 0; i < 2; i++) {
        layer1[i].input = input[i];
        layer1[i].output = input[i];
    }

    for (i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
        layer2[i].input += layer1[j].synaps[i].weight * layer1[j].output;
        layer2[i].activate();
        }
    }

    result.input = (layer2[0].output*layer2[0].synaps->weight) + (layer2[1].output*layer2[1].synaps->weight);
    result.activate();
    cout << "result " << result.output << endl;

}
double net::error(double idealAnswer){
    return pow((idealAnswer - result.output),2);
}


double net::outputDelta(double ideal)
{
    result.delta = (ideal-result.output)*(1-result.output)*result.output;
    return result.delta;

}
double net::hiddenDelta(newron& hidden){
    hidden.delta = (1-hidden.output)*hidden.output*hidden.synaps->weight*result.delta;
    return hidden.delta;
}

double net::inputDelta(newron &input){
    input.delta = (1-input.output)*input.output*((input.synaps[0].weight*layer2[0].delta)+input.synaps[1].weight*layer2[1].delta);
    return input.delta;
}




void net::backpropagation( double answer) {
    outputDelta(answer);
    for (int i = 0; i < 2; i++) {
    double Gradient = layer2[i].output*result.delta;
    layer2[i].synaps->DWeight = 0.7*Gradient + 0.3*layer2[i].synaps->DWeight;
    layer2[i].synaps->weight += layer2[i].synaps->DWeight;
    }

    hiddenDelta(layer2[0]);
    hiddenDelta(layer2[1]);

    for (int i = 0; i < 2;i++) {
        for (int j = 0; j < 2; j++) {
        double Gradient = layer1[i].output*layer2[j].delta;
        layer1[i].synaps[j].DWeight = 0.7*Gradient + 0.3*layer1[i].synaps->DWeight;
        layer1[i].synaps[j].weight += layer2[i].synaps->DWeight;
        }
    }


}






int main()
{
    srand(time(0));
    net Xorer;
    double a[2] = {1.0, 0.0};
    double b[2]= {0.0, 0.0};
    double c[2] = {0.0, 1.0};
    double d[2] = {1.0,1.0};
    //Xorer.forwardPass(a);
    for (int i = 0; i < 50; i++) {
        Xorer.forwardPass(a);
        cout << endl;
        cout << Xorer.error(1.0) << endl;
        Xorer.backpropagation(1.0);

        Xorer.forwardPass(b);
        cout << endl;
        cout << Xorer.error(0.0) << endl;
        Xorer.backpropagation(0.0);

        Xorer.forwardPass(c);
        cout << endl;
        cout << Xorer.error(1.0) << endl;
        Xorer.backpropagation(1.0);

        Xorer.forwardPass(d);
        cout << endl;
        cout << Xorer.error(0.0) << endl;
        Xorer.backpropagation(0.0);
    }

    return 0;
}
