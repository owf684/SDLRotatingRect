/*  Program Name: Rotating Rect
*
*   Purpose: This is an SDL comptabible 
*   RECT that allows rotation and collision
*   detection.
*
*   Author: Christopher Corona
*
*   Version: 1.0.0-alpha
*
*   Date: 25 Aug 2024
*/

#ifndef ROTATINGRECT_HPP
#define ROTATINGRECT_HPP
#pragma once
#include<SDL2/SDL.h>
#define ABOVE 0
#define BELOW 1
#define TO_RIGHT 2
#define TO_LEFT 3

namespace RotatingRect
{

struct Vertex2D {
    float x;
    float y;
    Vertex2D(){}
    Vertex2D(float xin, float yin)
    {
        x = xin; y = yin;
    }
};

struct Vector2D{
    float x;
    float y;
    Vector2D(){}
    Vector2D(Vertex2D P1, Vertex2D P2)
    {
        x = P2.x - P1.x;
        y = P2.y - P1.y;
    }
    Vector2D(float x_in, float y_in)
    {
        x = x_in;
        y = y_in;
    }
    Vector2D get_orthoganal()
    {
        return Vector2D(-y, x);
    }

    float dot_product(Vector2D vector_in)
    {
        return (x*vector_in.x + y*vector_in.y);
    }
};

struct Dimension2D{
    float w;
    float h;
};

class Rect2D {

public:
    Rect2D(){}
    Rect2D(float x, float y, float width, float height)
    {
        // Defining top verticies
        top_left.x = x;
        top_left.y = y;
        top_right.x = x + width;
        top_right.y = y;
        // Defining bottom verticies
        bottom_left.x = x;
        bottom_left.y = y + height;
        bottom_right.x = x + width;
        bottom_right.y = y + height;
        // Defining dimension
        size.w = width;
        size.h = height;
        // Defining destination verticies
        top_left_dst = top_left;
        top_right_dst = top_right;
        bottom_left_dst = bottom_left;
        bottom_right_dst = bottom_right;
        // Define origin
        origin.x = top_left.x + width / 2.0;
        origin.y = top_left.y + height / 2.0;

        top_segment = false; bottom_segment = false; left_segment = false; right_segment = false;
    }

    /* 
    * Function: is_intersect()
    * 
    * Purpose: returns a boolean of the local
    * collision variables ORed together
    */
    bool is_intersect(){ return top_segment || bottom_segment || left_segment || right_segment;}

    /* 
    * Functions: collision(int DIRECTION)
    *
    * Purpose: returns the relevant global collision variable
    *
    * Arguments: DIRECTION -- must be an integer value between 0-3
    */
   bool collision(int DIRECTION)
   {
    switch(DIRECTION){
        case ABOVE:
            return top_global_collision;
            break;
        case BELOW:
            return bottom_global_collision;
            break;
        case TO_RIGHT:
            return right_global_collision;
            break;
        case TO_LEFT:
            return left_global_collision;
            break;
    }
   }

    void set_rect(float x, float y, float width, float height)
    {
        // Defining top verticies
        top_left.x = x;
        top_left.y = y;
        top_right.x = x + width;
        top_right.y = y;
        // Defining bottom verticies
        bottom_left.x = x;
        bottom_left.y = y + height;
        bottom_right.x = x + width;
        bottom_right.y = y + height;
        // Defining dimension
        size.w = width;
        size.h = height;
        // Defining destination verticies
        top_left_dst = top_left;
        top_right_dst = top_right;
        bottom_left_dst = bottom_left;
        bottom_right_dst = bottom_right;
        // Define origin
        origin.x = top_left.x + width / 2.0;
        origin.y = top_left.y + height / 2.0; 

        top_segment = false; bottom_segment = false; left_segment = false; right_segment = false;
      
    }
    void draw_rect(SDL_Renderer* renderer)
    {
        // draw top edge
        if (top_segment) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer,top_left_dst.x, top_left_dst.y, top_right_dst.x, top_right_dst.y);
        // draw right edge
        if (right_segment) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, top_right_dst.x, top_right_dst.y, bottom_right_dst.x, bottom_right_dst.y);
        // draw bottom edge
        if (bottom_segment) SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);        
        SDL_RenderDrawLine(renderer, bottom_right_dst.x, bottom_right_dst.y,bottom_left_dst.x,bottom_left_dst.y);
        // draw left edge
        if (left_segment) SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer,bottom_left_dst.x, bottom_left_dst.y,top_left_dst.x, top_left_dst.y);
 
    }

    void rotate_rect(float theta)
    {
        top_left_dst = rotate_vertex(top_left,theta,origin);
        top_right_dst = rotate_vertex(top_right,theta,origin);
        bottom_left_dst = rotate_vertex(bottom_left,theta,origin);
        bottom_right_dst = rotate_vertex(bottom_right,theta,origin);
        rotation_angle = theta;
    }

    void set_position(float x, float y)
    {
         // Defining top verticies
        top_left.x = x;
        top_left.y = y;
        top_right.x = x + size.w;
        top_right.y = y;
        // Defining bottom verticies
        bottom_left.x = x;
        bottom_left.y = y + size.h;
        bottom_right.x = x + size.w;
        bottom_right.y = y + size.h;
        // Defining destination verticies
        top_left_dst = top_left;
        top_right_dst = top_right;
        bottom_left_dst = bottom_left;
        bottom_right_dst = bottom_right;
        // Define origin
        origin.x = top_left.x + size.w / 2.0;
        origin.y = top_left.y + size.h / 2.0; 
        // update dst points
        rotate_rect(rotation_angle);      
    }
    RotatingRect::Vertex2D getTopLeft()       { return top_left_dst;}
    RotatingRect::Vertex2D getTopRight()      { return top_right_dst;}
    RotatingRect::Vertex2D getBottomLeft()    { return bottom_left_dst;}
    RotatingRect::Vertex2D getBottomRight()   { return bottom_right_dst;}
    RotatingRect::Dimension2D getSize()       { return size;}

    bool Rect2DIntersect(Rect2D rect_2)
    {
        // reset global collision flags
        left_global_collision = false; right_global_collision = false; top_global_collision = false; bottom_global_collision = false;

        // calculate local intersections
        top_segment = Segment2DInterect(getTopLeft(),getTopRight(),rect_2);

        left_segment = Segment2DInterect(getTopLeft(),getBottomLeft(),rect_2);

        bottom_segment = Segment2DInterect(getBottomLeft(),getBottomRight(),rect_2);

        right_segment = Segment2DInterect(getTopRight(),getBottomRight(),rect_2);

        // calculate global collision 
        if (top_segment)
        {
            determine_global_collision(Vector2D(getTopLeft(), getTopRight()));
        }
        if (left_segment){
            determine_global_collision(Vector2D(getBottomLeft(),getTopLeft()));
        }
        if (bottom_segment){
            determine_global_collision(Vector2D(getBottomRight(),getBottomLeft()));
        }
        if (right_segment)
        {
            determine_global_collision(Vector2D(getTopRight(), getBottomRight()));
        }        
    
        return top_segment || left_segment || bottom_segment || right_segment;
    }

private:
    // Reference Vertex
    RotatingRect::Vertex2D top_left;
    RotatingRect::Vertex2D top_right;
    RotatingRect::Vertex2D bottom_left;
    RotatingRect::Vertex2D bottom_right;
    RotatingRect::Vertex2D origin;
    RotatingRect::Dimension2D size;

    // Active Vertex
    RotatingRect::Vertex2D top_left_dst;
    RotatingRect::Vertex2D top_right_dst;
    RotatingRect::Vertex2D bottom_left_dst;
    RotatingRect::Vertex2D bottom_right_dst;

    // Relative Intersect variables
    bool top_segment;
    bool left_segment;
    bool bottom_segment;
    bool right_segment;

    // Absolute Intersect_variables
    bool top_global_collision;
    bool left_global_collision;
    bool bottom_global_collision;
    bool right_global_collision;

    float rotation_angle;

    bool Segment2DInterect(Vertex2D rect_1_v1, Vertex2D rect_1_v2, Rect2D rect_2)
    {
        // Top Segment Rect 2
        if (line_intersect(rect_1_v1,rect_1_v2,rect_2.getTopLeft(),rect_2.getTopRight()))
        {
            return true;
        }
    
        // Left Segment Rect 2
    
        if (line_intersect(rect_1_v1,rect_1_v2,rect_2.getTopLeft(),rect_2.getBottomLeft()))
        {
            return true;
        }
    
        // Bottom Segment Rect 2
        if (line_intersect(rect_1_v1,rect_1_v2,rect_2.getBottomLeft(),rect_2.getBottomRight()))
        {
            return true;
        }

        // Right Segment Rect2
        if (line_intersect(rect_1_v1,rect_1_v2,rect_2.getTopRight(),rect_2.getBottomRight()))
        {
            return true;
        }    

        return false;
    }

    Vertex2D rotate_vertex(Vertex2D v, float theta, Vertex2D o)
    {
        // convert to radians
        theta = theta * 3.14159 / 180.0;
        // translate to origin
        v.x -= o.x;
        v.y -= o.y;

        // calculate rotated vertex
        Vertex2D v_rot (v.x * cos(theta) - v.y * sin(theta), 
                    v.x * sin(theta) + v.y * cos(theta));
    
        // translate away from origin
        v_rot.x += o.x;
        v_rot.y += o.y;

    return v_rot;

    }

    bool line_intersect(Vertex2D A, Vertex2D B, Vertex2D C, Vertex2D D)
    {
        /*
         *--------------------
         *   BAG OF FORMULAS
         *--------------------
         *------------------------------------
         *   THE LINEAR FORM
         * -----------------------------------
         *   y = m1 * x + b1
         *
         *   y = m2 * x + b2
         *
         * -----------------------------------
         *   X INTERCEPT FORMULA
         * -----------------------------------
         *   x = ( b2 - b1 ) / ( m1 - m2 )
         *
         * -----------------------------------
         *   SLOPE INTERCEPT FORMULA
         * -----------------------------------
         *   b1 = y - m1 * x
         *
         *   b2 = y - m2 * x
         *
         * -----------------------------------
         *   SLOPE FORMULA
         * -----------------------------------
         *   m_n = ( y1 - y2 ) / ( x1 - x2 )
         */
        if ((A.x - B.x != 0) && (C.x - D.x != 0))
        {
            // calculate slopes m1 & m2
            float m1 = (A.y - B.y) / (A.x - B.x);
            float m2 = (C.y - D.y) / (C.x - D.x);

            // calculates slope intercepts b1 & b2
            float b1 = A.y - (m1 * A.x);
            float b2 = C.y - (m2 * C.x);

            // find x intersection
            float numerator = b2 - b1;
            float denominator = m1 - m2;

            // two rotated segments
            if (int(denominator) != 0)
            {

                float x_intercept = numerator / denominator;

                if (((A.x <= x_intercept && x_intercept <= B.x) || (B.x <= x_intercept && x_intercept <= A.x)) &&
                    ((C.x <= x_intercept && x_intercept <= D.x) || (D.x <= x_intercept && x_intercept <= C.x)))
                    {
                    return true;

                    }
                // two horizontal segments
            }
            else if (denominator >= -0.1 && denominator <= 0.1 && numerator >= -0.1 && numerator <= 0.1)
            {

                if ((A.x >= C.x && A.x <= D.x) || (B.x >= C.x && B.x <= D.x))
                {
                    return true;
                }
            }
        }
        // two vertical segments
        else if ((A.x - B.x == 0) && (C.x - D.x == 0))
        {
            if (A.x == C.x && ((A.y >= C.y && A.y <= D.y) || (B.y >= C.y && B.y <= D.y)))
                return true;
        }
        // one vertical segment
        // segment AB non-rotated :: segment CD rotated or horizontal
        else if ((A.x - B.x == 0) && abs(C.x - D.x) > 0)
        {
            // get equation for vertical line
            float x = A.x;

            // calclate slope of segment CD
            float m = (C.y - D.y) / (C.x - D.x);

            // calculate slope intercept of segment CD
            float b = C.y - (m * C.x);

            // test vertical line in segment CD line-equation
            float y = m * x + b;

            // vertical line AB intercepts rotated CD
            if (((y >= C.y && y <= D.y) || (y >= D.y && y <= C.y)) &&
                ((y >= A.y && y <= B.y) || (y >= B.y && y <= A.y)) &&
                ((A.x >= C.x && A.x <= D.x) || (A.x >= D.x && A.x <= C.x)))
            {
                return true;
            }
        }
        // one vertical segment
        // segment AB rotated :: segment CD non-rotated
        else if ((C.x - D.x == 0) && abs(A.x - B.x) > 0)
        {
            // get equation for vertical line
            float x = C.x;

            // calclate slope of segment CD
            float m = (A.y - B.y) / (A.x - B.x);

            // calculate slope intercept of segment CD
            float b = A.y - (m * A.x);

            // test vertical line in segment CD line-equation
            float y = m * x + b;

            // vertical line AB intercepts rotated CD
            if (((y >= C.y && y <= D.y) || (y >= D.y && y <= C.y)) &&
                ((y >= A.y && y <= B.y) || (y >= B.y && y <= A.y)) &&
                ((C.x >= A.x && C.x <= B.x) || (C.x >= B.x && C.x <= A.x)))
            {
                return true;
            }
        }

        return false;
    }

    void determine_global_collision(Vector2D rect_edge){

        Vector2D up(0,1);
        Vector2D down(0,-1);
        Vector2D left(-1,0);
        Vector2D right(1,0);

        float up_dot = rect_edge.get_orthoganal().dot_product(up);
        float down_dot = rect_edge.get_orthoganal().dot_product(down);
        float right_dot = rect_edge.get_orthoganal().dot_product(right);
        float left_dot = rect_edge.get_orthoganal().dot_product(left);

        if (up_dot > down_dot)
        {
            top_global_collision = true;
        }
        if (down_dot > up_dot)
        {
            bottom_global_collision = true;
        }
        if (left_dot > right_dot)
        {
            right_global_collision = true;
        }

        if (right_dot > left_dot)
        {
            left_global_collision = true;
        }
    

    }
};
} // namespace RotatingRect

#endif