// This file contains the operators used in simulation
// 1. Advection operator
// 2. Projection operator
// 3. Diffuse operator
// 4. Vorticity operator
// 5. Dissipation operator

#ifndef OPERATOR_HPP
#define OPERATOR_HPP
#include <functional>
#include <QDebug>
#include <omp.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Sparse>

#include "Field.h"

template<typename T>
void curl2D(const Field<T, 2>& ops, Field<T, 1>& res){
    T grid_size = 1.0 / ops.get_height();
    int xN = ops.get_width(), yN = ops.get_height();
    
    #pragma omp parallel for
    for(int y = 1; y < yN-1; ++y){
        for(int x = 1; x < xN - 1; ++x){
            res(x, y, 0) = ((ops(x+1, y, 1) - ops(x-1, y, 1))-(ops(x, y+1, 0) - ops(x, y-1, 0)))/(2*grid_size);
        }
    }
};

template<typename T, size_t Dim>
void normalize(const Field<T, Dim>& ops, Field<T, Dim>& res){
    int xN = ops.get_width(), yN = ops.get_height();

#pragma omp parallel for
    for(int y = 0; y < yN; ++y){
        for(int x = 0; x < xN; ++x){
            T denom = 0;
            for(int d = 0; d < Dim; ++d){
                denom += ops(x, y, d) * ops(x, y, d);
            }
            if(denom > 1e-12){
                T sqrt_denom = sqrt(denom);
                for(int d = 0; d < Dim; ++d){
                    res(x, y ,d) = ops(x, y, d)/sqrt_denom;
                }
            }
            else{
                for(int d = 0; d < Dim; ++d){
                    res(x, y, d) = 0;
                }
            }
        }
    }
}

template<typename T>
void normalize(const Field<T, 1>& ops, Field<T, 1>& res){
    int xN = ops.get_width(), yN = ops.get_height();
    for(int y = 0; y < yN; ++y){
        for(int x = 0; x < xN; ++x){
            res(x, y, 0) = fabs(ops(x, y, 0));
        }
    }
}

template<typename T>
void gradient(const Field<T, 1>& ops, Field<T, 2>& res){
    T grid_size = 1.0 / ops.get_height();
    int xN = ops.get_width(), yN = ops.get_height();

#pragma omp parallel for
    for(int y = 1; y < yN-1; ++y){
        for(int x = 1; x < xN-1; ++x){
            res(x, y, 0) = (ops(x+1, y, 0) - ops(x-1, y, 0)) * 0.5 / grid_size;
            res(x, y, 1) = (ops(x, y+1, 0) - ops(x, y-1, 0)) * 0.5 / grid_size;
        }
    }
}

template <typename T>
void vorticity_confinement(Field<T, 2>& u, double ratio){
    int width = u.get_width(), height = u.get_height();
    T grid_size = 1.0 / height;

    Field<T, 1> w(width, height);
    curl2D<T>(u, w);
    Field<T, 1> norm_w(width, height);
    normalize(w, norm_w);

    Field<T, 2> gra(width, height);
    gradient(norm_w, gra);
    normalize(gra, gra);

#pragma omp parallel for
    for(int y = 1; y < height-1; ++y){
        for(int x = 1; x < width-1; ++x){
            u(x, y, 0) += ratio * grid_size * gra(x, y, 1) * w(x, y, 0);
            u(x, y, 1) -= ratio * grid_size * gra(x, y, 0) * w(x, y, 0);
        }
    }
};

template <typename T, size_t Dim>
void add_source(Field<T, Dim>& target, Field<T, Dim>& source, double dt){
    Q_UNUSED(dt)

#pragma omp parallel for
    for(int d = 0; d < Dim; ++d){
        for(int i = 0; i < target.get_size(); ++i){
            target(i, d) += source(i, d);
        }
    }
}

template <typename T, size_t Dim>
void dissipate(Field<T, Dim>& ops, double ratio){

    for(int d = 0; d < Dim; ++d){
        for(int i = 0; i < ops.get_size(); ++i){
            ops(i, d) = ops(i, d) / (1+ratio);
        }
    }
}

template <typename T, size_t Dim>
void set_boundary(Field<T, Dim>& ops);

template <typename T>
void set_boundary(Field<T, 1>& ops){
    int width = ops.get_width(), height = ops.get_height();
    for(int x = 0; x < width; ++x){
        if (ops(x, 0, 0) == 0) {
            ops(x, 0, 0) = ops(x, 1, 0);
            ops(x, height - 1, 0) = ops(x, height - 2, 0);
        }
        else {
            swap(ops(x, 0, 0), ops(x, 1, 0));
            swap(ops(x, height - 1, 0), ops(x, height - 2, 0));
        }
    }
    for(int y = 0; y < height; ++y){
        if (ops(0, y, 0) == 0) {
            ops(0, y, 0) = ops(1, y, 0);
            ops(width - 1, y, 0) = ops(width - 2, y, 0);
        }
        else {
            swap(ops(0, y, 0), ops(1, y, 0));
            swap(ops(width - 1, y, 0), ops(width - 2, y, 0));
        }
    }

    ops(0, 0, 0) = 0.5 * (ops(1, 0, 0) + ops(0, 1, 0));
    ops(0, height-1, 0) = 0.5 * (ops(1, height-1, 0) + ops(0, height-2, 0));
    ops(width-1, 0, 0) = 0.5 * (ops(width-1, 1, 0) + ops(height-2, 0, 0));
    ops(width-1, height-1, 0) = 0.5 * (ops(width-2, height-1, 0) + ops(width-1, height-2, 0));
}

template <typename T>
void set_boundary(Field<T, 2>& ops){
    int width = ops.get_width(), height = ops.get_height();
    for(int x = 0; x < width; ++x){
        ops(x, 0, 1) = -ops(x, 1, 1);
        ops(x, height-1, 1) = -ops(x, height-2, 1);
        ops(x, 0, 0) = ops(x, 1, 0);
        ops(x, height-1, 0) = ops(x, height-2, 0);
    }
    for(int y = 0; y < height; ++y){
        ops(0, y, 1) = ops(1, y, 1);
        ops(width-1, y, 1) = ops(width-2, y, 1);
        ops(0, y, 0) = -ops(1, y, 0);
        ops(width-1, y, 0) = -ops(width-2, y, 0);
    }
    for(int i = 0; i < 2; ++i){
        ops(0, 0, i) = 0.5 * (ops(1, 0, i) + ops(0, 1, i));
        ops(0, height-1, i) = 0.5 * (ops(1, height-1, i) + ops(0, height-2, i));
        ops(width-1, 0, i) = 0.5 * (ops(width-1, 1, i) + ops(height-2, 0, i));
        ops(width-1, height-1, i) = 0.5 * (ops(width-2, height-1, i) + ops(width-1, height-2, i));
    }
}

template <typename T>
void set_free_boundary(Field<T, 1>& ops){
    int width = ops.get_width(), height = ops.get_height();
    for(int x = 0; x < width; ++x){
        ops(x, 0, 0) = ops(x, 1, 0);
        ops(x, height-1, 0) = ops(x, height-2, 0);
    }
    for(int y = 0; y < height; ++y){
        ops(0, y, 0) = ops(1, y, 0);
        ops(width-1, y, 0) = ops(width-2, y, 0);
    }

    ops(0, 0, 0) = 0.5 * (ops(1, 0, 0) + ops(0, 1, 0));
    ops(0, height-1, 0) = 0.5 * (ops(1, height-1, 0) + ops(0, height-2, 0));
    ops(width-1, 0, 0) = 0.5 * (ops(width-1, 1, 0) + ops(height-2, 0, 0));
    ops(width-1, height-1, 0) = 0.5 * (ops(width-2, height-1, 0) + ops(width-1, height-2, 0));
}

template <typename T, size_t Dim>
void diffuse(Field<T, Dim>& ops, double df, double dt){
    int width = ops.get_width(), height = ops.get_height();
    double ratio = (1.0/width) * (1.0/height) * df * dt;

    // Gauss-Seidel iteration
    Field<T, Dim> temp(width, height);
    for(int i = 0; i < 30; ++i){
        for(int d = 0; d < Dim; ++d){
            for(int y = 1; y < height-1; ++y){
                for(int x = 1; x < width-1; ++x){
                    temp(x, y, d) = (ops(x, y, d)
                                     +ratio*(temp(x+1, y, d) + temp(x-1, y, d) + temp(x, y+1, d) + temp(x, y-1, d))
                                     )/(1+4*ratio);
                }
            }
        }
        set_boundary(temp);
    }
    ops = std::move(temp);
}

template <typename T, size_t Dim>
void diffuse(Field<T, Dim>& ops, Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Lower | Eigen::Upper>& cg_) {
    T* raw = ops.get_raw();
    Field<T, Dim> temp(ops.get_width(), ops.get_height());

    for (int d = 0; d < Dim; ++d) {
        Eigen::VectorXd p, b;
        b = Eigen::Map<Eigen::VectorXd>(raw + d * ops.get_width() * ops.get_height(), ops.get_width() * ops.get_height());
        p = cg_.solve(b);
        copy( p.data(), p.data() + temp.get_width()*temp.get_height(), temp.get_raw() + d * temp.get_width() * temp.get_height());
    }
    ops = std::move(temp);
}

template <typename T, size_t Dim>
void advect_rk4(Field<T, Dim> & target, Field<T, Dim>& source, Field<T, 2>& driver, double dt) {
    int width = source.get_width(), height = source.get_height();
    dt = dt * width;

#pragma omp parallel for
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            // k1
            double k1_x = driver(x, y, 0), k1_y = driver(x, y, 1);
            // k2
            double pos2_x = x - 0.5 * dt * driver(x, y, 0),
                   pos2_y = y - 0.5 * dt * driver(x, y, 1);
            double k2_x = driver.interp(pos2_x, pos2_y, 0), 
                   k2_y = driver.interp(pos2_x, pos2_y, 1);
            // k3
            double pos3_x = x - 0.75 * dt * driver.interp(k2_x, k2_y, 0),
                   pos3_y = y - 0.75 * dt * driver.interp(k2_x, k2_y, 1);
            double k3_x = driver.interp(pos3_x, pos3_y, 0), 
                   k3_y = driver.interp(pos3_x, pos3_y, 1);
            // integrate
            double kx = x - 2.0 / 9 * dt * k1_x - 1.0 / 3 * dt * k2_x - 4.0 / 9 * dt * k3_x;
            double ky = y - 2.0 / 9 * dt * k1_y - 1.0 / 3 * dt * k2_y - 4.0 / 9 * dt * k3_y;
            for (int d = 0; d < Dim; ++d) {
                target(x, y, d) = source.interp(kx, ky, d);
            }
        }
    }
    set_boundary(target);
}

template <typename T, size_t Dim>
void advect1(Field<T, Dim>& target, Field<T, Dim>& source, Field<T, 2>& driver, double dt) {
    int width = source.get_width(), height = source.get_height();
    dt = dt * width;
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            // k1
            double k1_x = driver(x, y, 0), k1_y = driver(x, y, 1);
           // integrate
            double kx = x - dt * k1_x;
            double ky = y - dt * k1_y;
            for (int d = 0; d < Dim; ++d) {
                target(x, y, d) = source.interp(kx, ky, d);
            }
        }
    }
    set_boundary(target);
}


template <typename T, size_t Dim>
void advect(Field<T, Dim>& target, Field<T, Dim>& source, Field<T, 2>& driver, double dt){
    int i0, i1, j0, j1;
    double rx0, rx1, ry0, ry1;
    double new_x, new_y;
    int width = source.get_width(), height = source.get_height();
    dt = dt * width;
    for(int y = 1; y < height-1; ++y){
        for(int x = 1; x < width-1; ++x){
            // bilinear interpolate
            new_x = x - dt*driver(x, y, 0); new_y = y - dt*driver(x, y, 1);
            if(new_x < 0.5) new_x = 0.5; if(new_x > width-1.5) new_x = width-1.5;
            if(new_y < 0.5) new_y = 0.5; if(new_y > height-1.5) new_y = height-1.5;
            i0 = (int)new_x; i1 = i0+1;
            j0 = (int)new_y; j1 = j0+1;
            rx1 = new_x - i0; rx0 = 1 - rx1;
            ry1 = new_y - j0; ry0 = 1 - ry1;
            for(int d = 0; d< Dim; ++d){
                target(x, y, d) = rx0*(ry0*source(i0, j0, d) + ry1*source(i0, j1, d)) +
                                  rx1*(ry0*source(i1, j0, d) + ry1*source(i1, j1, d));
            }
        }
    }
    set_boundary(target);
}

template <typename T>
void divergence(Field<T, 2>& ops, Field<T, 1>& div){
    int width = ops.get_width(), height = ops.get_height();
    double h = 1.0 / (width-2.0);

#pragma omp parallel for
    for(int y = 1; y < height-1; ++y){
        for(int x = 1; x < width-1; ++x){
            div(x, y, 0) = -0.5 * h * (ops(x+1, y, 0) - ops(x-1, y, 0) + ops(x, y+1, 1) - ops(x, y-1, 1));
        }
    }
    set_boundary(div);
}

template <typename T>
void project(Field<T, 2>& ops){
    int width = ops.get_width(), height = ops.get_height();
    double ratio = width-2.0;

    Field<T, 1> div(width, height);
    divergence(ops, div);

    Field<T, 1> pressure(width, height);
    // Gauss-Seidel Iteration
    for(int i = 0; i < 30; ++i){
        for(int y = 1; y < height-1; ++y){
            for(int x = 1; x < width-1; ++x){
                pressure(x, y, 0) = (div(x, y, 0) + pressure(x+1, y, 0) + pressure(x-1, y, 0) +
                                     pressure(x, y+1, 0) + pressure(x, y-1, 0))/4;
            }
        }
        set_boundary(pressure);
    }

#pragma omp parallel for
    for(int y = 1; y < height-1; ++y){
        for(int x = 1; x < width-1; ++x){
            ops(x, y, 0) -= 0.5 * ratio * (pressure(x+1, y, 0) - pressure(x-1, y, 0));
            ops(x, y, 1) -= 0.5 * ratio * (pressure(x, y+1, 0) - pressure(x, y-1, 0));
        }
    }
}

#endif // OPERATOR_HPP
