#ifndef WEIGHT_H
#define WEIGHT_H

class Weight
{
  public:
    Weight();
    ~Weight();

    double Curr();    
    double Prev();

    void Update(double new_weight);
    void Randomize();


  private:
    double current_weight;
    double previous_weight;
};

#endif
