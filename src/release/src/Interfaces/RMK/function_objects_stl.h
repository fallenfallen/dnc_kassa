#include "receipt.h" 
#include <qcheckbox.h>
#include <qlabel.h>



#ifndef STL_OBJECS_FOR_CDFORM
#define STL_OBJECS_FOR_CDFORM
//typedef map<int, double> redSum_map;
typedef map<int, Discount> discount_map;

typedef vector<QCheckBox*> checkBox_vector;
typedef vector<QLabel *> textLabel_vector;
typedef map<int, QCheckBox *> focus_map;

/*struct less_s: public binary_function <redSum_map::value_type, redSum_map::value_type, bool>
{
public :
  //bool operator () (redSum_map::iterator & it1, redSum_map::iterator & it2)
  bool operator () (redSum_map::value_type & p1, redSum_map::value_type & p2)
  {
    return p1.second < p2.second;
  }
} less_sum;
*/
/*
struct less_d : public binary_function <discount_map::value_type, discount_map::value_type, bool>
{
    bool operator () (discount_map::value_type p1, discount_map::value_type p2);
} less_discount;*/


class less_d //: public binary_function <discount_map::value_type, discount_map::value_type, bool>
{
public :
    bool operator () (discount_map::value_type p1, discount_map::value_type p2);
  /*{
    return p1.second.dsum < p2.second.dsum;
  }*/
};


#endif
