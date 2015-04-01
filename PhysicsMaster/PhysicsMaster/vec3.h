#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef float scalar;

/*La classe Vec3 permette di costruire oggetti che rappresentano
  vettori di 3 elementi di tipo scalar(float)*/
class Vec3
{
    //CAMPI
private:
    scalar v[3];

public:
    //COSTRUTTORI
    Vec3()
    {}
    Vec3(scalar x,scalar y,scalar z)
    {
        v[0]=x;
        v[1]=y;
        v[2]=z;
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
    //Scrittura Coordinate (riferimento_all'oggetto.nomeMetodo()=value)
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

    //Operatore + BINARIO (X+Y)
    Vec3 operator +(Vec3 altro) const
    {
        scalar x=this->x()+altro.x();
        scalar y=this->y()+altro.y();
        scalar z=this->z()+altro.z();
        return (Vec3(x, y, z));
    }
    //Operatore - BINARIO (X-Y)
    Vec3 operator -(Vec3 altro) const
    {
        scalar x=this->x()-altro.x();
        scalar y=this->y()-altro.y();
        scalar z=this->z()-altro.z();
        return (Vec3(x, y, z));
    }
    //Operatore - UNARIO (-X)
    Vec3 operator -() const
    {
        return Vec3(-this->x(), -this->y(), -this->z());
    }
    //Prodotto per uno Scalare
    Vec3 operator *(scalar k) const
    {
        scalar x=this->x()*k;
        scalar y=this->y()*k;
        scalar z=this->z()*k;
        return (Vec3(x, y, z));
    }
    //Divisione per uno Scalare
    Vec3 operator /(scalar k) const
    {
        scalar x=this->x()/k;
        scalar y=this->y()/k;
        scalar z=this->z()/k;
        return (Vec3(x, y, z));
    }
    //Prodotto Scalare(DOT)
    scalar operator *(Vec3 v) const
    {
        scalar x=this->x()*v.x();
        scalar y=this->y()*v.y();
        scalar z=this->z()*v.z();
        return x+y+z;
    }
    //Prodotto Vettoriare(CROSS)
    Vec3 operator ^(Vec3 v) const
    {
        scalar x=this->y()*v.z()-this->z()*v.y();
        scalar y=this->z()*v.x()-this->x()*v.z();
        scalar z=this->x()*v.y()-this->y()*v.x();
        return (Vec3(x, y, z));
    }

    //Operatore =
    bool operator ==(Vec3 v) const
    {
        if(this->x()==v.x() && this->y()==v.y() && this->z()==v.z())
            return true;
        else
            return false;
    }
    //Operatore !=
    bool operator !=(Vec3 v) const
    {
        if(this->x()!=v.x() || this->y()!=v.y() || this->z()!=v.z())
            return true;
        else
            return false;
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
    Vec3 normalized() const
    {
        return (*this)/this->norm();
    }
    //Rotazione in gradi intorno all'asse Y
    Vec3 rotateYDeg(int degree) const
    {
        float rad=degree*M_PI/180;
        float c=cos(rad);
        float s=sin(rad);
        scalar x=dot(*this, Vec3( c, 0,-s));//Vec3( c, 0, s)
        scalar z=dot(*this, Vec3( s, 0, c));//Vec3(-s, 0, c) cambiando devo ottenere la stessa stanza riflessa
        return Vec3(x, 0, z);
    }

    bool isZero() const
    {
        return(x()==0 && y()==0 && z()==0);
    }

    //METODI STATICI
    //Prodotto Scalare(DOT)
    static scalar dot(Vec3 v1,Vec3 v2)
    {
        return v1*v2;
    }
    //Prodotto Vettoriare(CROSS)
    static Vec3 cross(Vec3 v1,Vec3 v2)
    {
        return v1^v2;
    }
};

typedef Vec3 point3;  //Definisco punti 3D come vettori 3D

#endif // VEC3_H
