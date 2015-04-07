#include "vec3.h"
/*Classe AABB (Axis Aligned Bounding Box) per le Mesh
  NB: Per costruire un bb ho bisogno sempre e solo di due punti, inquanto, le coordinate si ripetono. Quindi, se conosco il
      punto più piccolo e quello più grande posso ricavare facilmente tutti gli altri punti del bb */

class AABB
{
private:
    //CAMPI
    point3 min;             //Punto minimo del Bounding Box
    point3 max;             //Punto massimo del Bounding Box
    bool empty;

public:
    //COSTRUTTORE
    AABB();

    //METODI
    void clear();           //Set Bounding Box come vuoto(senza punti)
    void add(point3 p);     //Aggiunge un punto al Bounding Box. Il bounding box viene modificato se il punto cade fuori da esso
    bool isEmpty() const;   //Restituisce true se il bounding box e' vuoto, false altrimenti
    point3 center() const;  //Restituisce il centro del bb
    point3 minP() const;    //Restituisce il punto min del bb
    point3 maxP() const;    //Restituisce il punto max del bb
};