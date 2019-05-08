#include <iostream>
using namespace std;
class Vector2D{
public:
    int i;
    int j;
    bool is_valid;
    Vector2D(int _i, int _j){
        i = _i;
        j = _j;
        is_valid = true;
    }
    Vector2D(){
        i = 0;
        j = 0;
        is_valid = true;
    }
    static Vector2D invalid(){
        Vector2D res;
        res.is_valid = false;
        return res;
    }
    bool isvalid(){
        return is_valid;
    }
    friend ostream& operator<<(ostream& os, const Vector2D& dt){
        os << "("<<dt.i<<","<<dt.j<<")";
        return os;
    }
    bool operator==(const Vector2D& dt){
        if(i == dt.i && j == dt.j)
            return true;
        return false;
    }
    Vector2D operator+(const Vector2D& dt){
        return Vector2D(i + dt.i, j + dt.j);
    }
    int dist(const Vector2D& dt){
        return abs(i - dt.i) + abs(j - dt.j);
    }
};
