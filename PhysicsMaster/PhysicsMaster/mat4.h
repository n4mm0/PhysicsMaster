#ifndef MAT4_H
#define MAT4_H

#include <math.h>
#include "vec4.h"

typedef float scalar;

/*La classe Mat4 permette di costruire oggetti che rappresentano
  Matrici 4x4 con elementi di tipo scalar(float)*/
class Mat4
{
    //CAMPI
private:
    scalar mat[4][4];

public:
    //COSTRUTTORE
    Mat4()
    {}

    //METODI
    //Restituisce l'i-esima Riga della Matrice che Invoca il Metodo
    Vec4 getRow(int i) const
    {
        return Vec4(mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
    }
    //Restituisce la j-esima Colonna della Matrice che Invoca il Metodo
    Vec4 getColumn(int j) const
    {
        return Vec4(mat[0][j], mat[1][j], mat[2][j], mat[3][j]);
    }
    //Accesso ai Singoli Elementi in Lettura
    scalar getElement(int i, int j)
    {
        return mat[i][j];
    }
    //Accesso ai Singoli Elementi in Scrittura
    void setElement(int i, int j, scalar val)
    {
        mat[i][j]=val;
    }
    //Prodotto Matrice per Vettore
    Vec4 operator *(Vec4 v) const
    {
        scalar x=Vec4::dot(getRow(0), v);
        scalar y=Vec4::dot(getRow(1), v);
        scalar z=Vec4::dot(getRow(2), v);
        scalar w=Vec4::dot(getRow(3), v);
        return Vec4(x, y, z, w);
    }
    //Prodotto Matrice per Matrice(Riga per Colonna)
    Mat4 operator *(Mat4 a) const
    {
        //Contiene il Risultato del Prodotto Riga per Colonna
        //tra la Matrice che Invoca il Metodo e la Matrice m
        Mat4 b;
        //Contiene il Risultato del Prodotto DOT tra la Riga i della
        //Matrice che Invoca il Metodo e la Colonna j della Matrice m
        scalar res;
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
            {
                res=Vec4::dot(getRow(i), a.getColumn(j)); //Prodotto Riga per Colonna
                b.setElement(i, j, res);
            }
        return b;
    }

    //METODI STATICI
    //Da Gradi a Radianti
    static scalar toRad(scalar deg)
    {
        return deg*M_PI/180;
    }

    //Restituisce la Matrice Identità
    static Mat4 identity()
    {
        Mat4 m;
        int i,j;
        for(i=0;i<4;i++)
            for(j=0;j<4;j++)
            {
                if(i==j)
                   m.setElement(i, j, 1);
                else
                   m.setElement(i, j, 0);
            }
        return m;
    }
    static Mat4 translation(scalar dx, scalar dy, scalar dz)
    {
        Mat4 m=Mat4::identity();
        //Assegnamo all'ultima colonna(Origine-P0) i Valori di Traslazione
        m.setElement(0, 3, dx);
        m.setElement(1, 3, dy);
        m.setElement(2, 3, dz);
        return m;
    }
    //Rotazione in gradi sull'Asse X
    static Mat4 rotationX(scalar angle)
    {
        scalar s=sin(toRad(angle));
        scalar c=cos(toRad(angle));
        Mat4 m=Mat4::identity();
        m.setElement(1, 1, c);
        m.setElement(1, 2,-s);
        m.setElement(2, 1, s);
        m.setElement(2, 2, c);
        return m;
    }
    //Rotazione in gradi sull'Asse Y
    static Mat4 rotationY(scalar angle)
    {
        scalar s=sin(toRad(angle));
        scalar c=cos(toRad(angle));
        Mat4 m=Mat4::identity();
        m.setElement(0, 0, c);
        m.setElement(0, 2, s);
        m.setElement(2, 0,-s);
        m.setElement(2, 2, c);
        return m;
    }
    //Rotazione in gradi sull'Asse Z
    static Mat4 rotationZ(scalar angle)
    {
        scalar s=sin(toRad(angle));
        scalar c=cos(toRad(angle));
        Mat4 m=Mat4::identity();
        m.setElement(0, 0, c);
        m.setElement(0, 1,-s);
        m.setElement(1, 0, s);
        m.setElement(1, 1, c);
        return m;
    }
    //Scalatura
    static Mat4 scaling(scalar sx, scalar sy, scalar sz)
    {
        Mat4 m=Mat4::identity();
        m.setElement(0, 0, sx);
        m.setElement(1, 1, sy);
        m.setElement(2, 2, sz);
        return m;
    }
    //Scalatura Uniforme
    static Mat4 uniformScaling(scalar s)
    {
        return Mat4::scaling(s, s, s);
    }
};

#endif // MAT4_H
