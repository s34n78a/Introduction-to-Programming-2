#ifndef FUNCTION_H
#define FUNCTION_H

namespace oj {
    class Nut {
        double volume;

    public:
        double getVolume();

    protected:
        const double PI = 3.141592653589793;
        void setVolume(double volume);
    };

    class CuboidNut : public Nut {
        double height, width, length;

    public:
        CuboidNut(double, double, double);
    };

    class SphereNut : public Nut {
        double radius;

    public:
        SphereNut(double);
    };

    class ConeNut : public Nut {
        double radius, height;

    public:
        ConeNut(double, double);
    };
    
    class CylinderNut : public Nut {
        double radius, height;

    public:
        CylinderNut(double, double);
    };

    class CubeNut : public CuboidNut {
    public:
        CubeNut(double);
    };
}

#endif