#ifndef VEC4_H
#define VEC4_H

#include <math.h>
#include "vec3.h"

typedef float scalar;

/*La classe Vec4 permette di costruire oggetti che rappresentano
  vettori di 4 elementi di tipo scalar(float)*/
class Vec4
{
    //CAMPI
private:
    scalar v[4];

public:
    //COSTRUTTORI
    Vec4()
    {}
    Vec4(scalar x,scalar y,scalar z, scalar w)
    {
        v[0]=x;
        v[1]=y;
        v[2]=z;
        v[3]=w;
    }
    //Aggiunge Coordinata Affine ad un Vec3
    Vec4(Vec3 a, scalar w)
    {
        v[0]=a.x();
        v[1]=a.y();
        v[2]=a.z();
        v[3]=w;
    }

    //METODI
    //Lettura Coordinate
    scalar x() const
    {
        return v[0];
    }
    scalar y() const
    {
        return v[1];
    }
    scalar z() const
    {
        return v[2];
    }
    scalar w() const
    {
        return v[3];
    }
    //Scrittura Coordinate
    scalar &x()
    {
        return v[0];
    }
    scalar &y()
    {
        return v[1];
    }
    scalar &z()
    {
        return v[2];
    }
    scalar &w()
    {
        return v[3];
    }

    //Operatore + BINARIO (X+Y)
    Vec4 operator +(Vec4 altro) const
    {
        scalar x=this->x()+altro.x();
        scalar y=this->y()+altro.y();
        scalar z=this->z()+altro.z();
        scalar w=this->w()+altro.w();
        return (Vec4(x, y, z, w));
    }
    //Operatore - BINARIO (X-Y)
    Vec4 operator -(Vec4 altro) const
    {
        scalar x=this->x()-altro.x();
        scalar y=this->y()-altro.y();
        scalar z=this->z()-altro.z();
        scalar w=this->w()-altro.w();
        return (Vec4(x, y, z, w));
    }
    //Operatore - UNARIO (-X)
    Vec4 operator -() const
    {
        return Vec4(-this->x(), -this->y(), -this->z(), -this->w());
    }
    //Prodotto per uno Scalare
    Vec4 operator *(scalar k) const
    {
        scalar x=this->x()*k;
        scalar y=this->y()*k;
        scalar z=this->z()*k;
        scalar w=this->w()*k;
        return (Vec4(x, y, z, w));
    }
    //Divisione per uno Scalare
    Vec4 operator /(scalar k) const
    {
        scalar x=this->x()/k;
        scalar y=this->y()/k;
        scalar z=this->z()/k;
        scalar w=this->w()/k;
        return (Vec4(x, y, z, w));
    }
    //Prodotto Scalare(DOT)
    scalar operator *(Vec4 v) const
    {
        scalar x=this->x()*v.x();
        scalar y=this->y()*v.y();
        scalar z=this->z()*v.z();
        scalar w=this->w()*v.w();
        return x+y+z+w;
    }

    //Norma di un Vettore
    //Radice del Prodotto DOT fra le componenti x,y,z di un Vettore
    scalar norm() const
    {
        return sqrt(squaredNorm());
    }
    //Prodotto DOT del Vettore che Invoca il Metodo per se Stesso
    //x()*x() + y()*y() + z()*z()
    scalar squaredNorm() const
    {
        return dot((*this),(*this));
    }

    //Normalizzazione IN PLACE
    //Rende il Modulo del Vettore che Invoca il Metodo Unitario
    void normalize()
    {
        (*this)=(*this)/this->norm();
    }
    //Normalizzazione OUT PLACE
    //Restituisce un Vettore di Modulo Unitario
    Vec4 normalized() const
    {
        return (*this)/this->norm();
    }

    //METODI STATICI
    //Prodotto Scalare(DOT)
    static scalar dot(Vec4 v1,Vec4 v2)
    {
        return v1*v2;
    }
};
#endif // VEC4_H
