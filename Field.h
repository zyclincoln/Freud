#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include <QObject>
#include <algorithm>

template<typename T, size_t dim>
class Field
{
public:
    Field(){
        dim_ = 0;
    };

    Field(int w, int h):w_(w), h_(h), dim_(dim), data_(w*h*dim, 0), wh_(w*h){
    }

    T sum() const{
        return std::accumulate(data_.begin(), data_.end(), 0.);
    }

    inline
    T operator()(int x, int y, int dim) const {
        return data_[dim * wh_ + y * w_ + x];
    }

    inline
    T& operator()(int x, int y, int dim) {
        return data_[dim * wh_ + y * w_ + x];
    }

    inline
    T operator()(int pos, int dim) const {
        return data_[dim * wh_ + pos];
    }

    inline
    T& operator()(int pos, int dim){
        return data_[dim * wh_ + pos];
    }

    inline
    T interp(double x, double y, int dim) {
        if (x < 0.5) x = 0.5; if (x > w_ - 1.5) x = w_ - 1.5;
        if (y < 0.5) y = 0.5; if (y > h_ - 1.5) y = h_ - 1.5;
        int i0 = int(x);
        int i1 = i0 + 1;
        int j0 = int(y);
        int j1 = j0 + 1;
        double rx1 = x - i0;
        double ry1 = y - j0;
        return (1 - rx1) * ((1 - ry1) * this->operator()(i0, j0, dim) + ry1 * this->operator()(i0, j1, dim)) +
                     rx1 * ((1 - ry1) * this->operator()(i1, j0, dim) + ry1 * this->operator()(i1, j1, dim));
    }

    inline
    void clear(){
        data_.assign(data_.size(), 0);
    }

    inline
    int get_width() const{
        return w_;
    }

    inline
    int get_height() const{
        return h_;
    }

    inline
    int get_size() const {
        return w_*h_;
    }

//private:
public:
    std::vector<T> data_;
    int w_, h_;
    int dim_;
    int wh_;
};

#endif // FIELD_H
