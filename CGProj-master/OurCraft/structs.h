#ifndef OURCRAFT_STRUCTS_H
#define OURCRAFT_STRUCTS_H

#include <tkDecls.h>
#include <tclDecls.h>

/**
 * 坐标
 */
class Coords {
private:
    int x, y, z;
public:
    Coords(int x, int y, int z) : x(x), y(y), z(z) {}

    int getX() const {
        return x;
    }

    void setX(int x) {
        Coords::x = x;
    }

    int getY() const {
        return y;
    }

    void setY(int y) {
        Coords::y = y;
    }

    int getZ() const {
        return z;
    }

    void setZ(int z) {
        Coords::z = z;
    }
};

/**
 * 正方形, 由四个顶点确定
 */
class Square {
private:
    Coords *coords[4];
public:
    Square(Coords *pCoords0, Coords *pCoords1, Coords *pCoords2, Coords *pCoords3) {
        coords[0] = pCoords0;
        coords[1] = pCoords1;
        coords[2] = pCoords2;
        coords[3] = pCoords3;
    }

    Coords *const *getCoords() const {
        return coords;
    }
};

/**
 * 立方体的6个面
 */
class CubeFaces {
private:
    /**
     * 6个正方形分别对应上下左右前后6个面
     */
    Square *square[6];
public:
    /**
     * @param top       顶面
     * @param bottom    底面
     * @param left      左面
     * @param right     右面
     * @param front     前面
     * @param back      后面
     * @return
     */
    CubeFaces(Square *top, Square *bottom, Square *left, Square *right, Square *front, Square *back) {
        square[0] = top;
        square[1] = bottom;
        square[2] = left;
        square[3] = right;
        square[4] = front;
        square[5] = back;
    }

    /**
     * @param top       顶面
     * @param bottom    底面
     * @param side      前后左右4个面
     * @return
     */
    CubeFaces(Square *top, Square *bottom, Square *side) {
        square[0] = top;
        square[1] = bottom;
        square[2] = side;
        square[3] = side;
        square[4] = side;
        square[5] = side;
    }

    Square *const *getSquare() const {
        return square;
    }
};

CubeFaces *cube_vertices(int x, int y, int z, int n) {
    Coords *coords0 = new Coords(x - n, y + n, z - n);
    Coords *coords1 = new Coords(x - n, y + n, z + n);
    Coords *coords2 = new Coords(x + n, y + n, z + n);
    Coords *coords3 = new Coords(x + n, y + n, z - n);
    Square *square0 = new Square(coords0, coords1, coords2, coords3);

    coords0 = new Coords(x - n, y - n, z - n);
    coords1 = new Coords(x + n, y - n, z - n);
    coords2 = new Coords(x + n, y - n, z + n);
    coords3 = new Coords(x - n, y - n, z + n);
    Square *square1 = new Square(coords0, coords1, coords2, coords3);

    coords0 = new Coords(x - n, y - n, z - n);
    coords1 = new Coords(x - n, y - n, z + n);
    coords2 = new Coords(x - n, y + n, z + n);
    coords3 = new Coords(x - n, y + n, z - n);
    Square *square2 = new Square(coords0, coords1, coords2, coords3);

    coords0 = new Coords(x + n, y - n, z + n);
    coords1 = new Coords(x + n, y - n, z - n);
    coords2 = new Coords(x + n, y + n, z - n);
    coords3 = new Coords(x + n, y + n, z + n);
    Square *square3 = new Square(coords0, coords1, coords2, coords3);

    coords0 = new Coords(x - n, y - n, z + n);
    coords1 = new Coords(x + n, y - n, z + n);
    coords2 = new Coords(x + n, y + n, z + n);
    coords3 = new Coords(x - n, y + n, z + n);
    Square *square4 = new Square(coords0, coords1, coords2, coords3);

    coords0 = new Coords(x + n, y - n, z - n);
    coords1 = new Coords(x - n, y - n, z - n);
    coords2 = new Coords(x - n, y + n, z - n);
    coords3 = new Coords(x + n, y + n, z - n);
    Square *square5 = new Square(coords0, coords1, coords2, coords3);

    CubeFaces *cubeFaces = new CubeFaces(square0, square1, square2, square3, square4, square5);

    return cubeFaces;
}
#endif //OURCRAFT_STRUCTS_H
