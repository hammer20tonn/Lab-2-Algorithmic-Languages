#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Pipeline {
    int Length;
};

int main() {
    Pipeline p1; p1.Length = 49;
    Pipeline p2; p2.Length = 30;
    Pipeline p3; p3.Length = 21;
    Pipeline p4; p4.Length = 88;
    Pipeline p5; p5.Length = 90;
    
    vector<Pipeline> Pipelines = {p1, p2, p3, p4, p5};
    
    vector<size_t> indices(Pipelines.size());
    for (size_t i = 0; i < Pipelines.size(); ++i) {
        indices[i] = i;
    }
    
    /**
    // Захватываем Pipelines по ссылке [&Pipelines]
    sort(indices.begin(), indices.end(),
          [&Pipelines](size_t i, size_t j) {
              return Pipelines[i].Length < Pipelines[j].Length;
          });
    
    for (size_t idx : indices) {
        cout << idx << " " << Pipelines[idx].Length << endl;
    }
    **/
    return 0;
}