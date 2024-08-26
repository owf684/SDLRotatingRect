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

struct Dimension2D{
    float w;
    float h;
};

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
    if ( (A.x - B.x != 0) && ( C.x - D.x != 0 ) )
    {
        // calculate slopes m1 & m2
        float m1 = ( A.y - B.y ) / ( A.x - B.x );
        float m2 = ( C.y - D.y ) / ( C.x - D.x );

        // calculates slope intercepts b1 & b2
        float b1 = A.y - ( m1 * A.x );
        float b2 = C.y - ( m2 * C.x );
   
        // find x intersection
        float numerator = b2 - b1;
        float denominator = m1 - m2;

        // two rotated segments
        if (denominator != 0) {

            float x_intercept = numerator / denominator;

            if ( ( ( A.x <= x_intercept && x_intercept <= B.x ) || (B.x <= x_intercept && x_intercept <= A.x) )  &&
             ( ( C.x <= x_intercept && x_intercept <= D.x ) || (D.x <= x_intercept && x_intercept <= C.x) ) )
                return true;

        // two horizontal segments
        } else if(denominator == 0 && numerator == 0){

            if ( ( A.x >= C.x && A.x <= D.x )|| (B.x >= C.x && B.x <= D.x) )
            {
                return true;
            } 
        } 
   }
   // two vertical segments
   else if ( ( A.x - B.x == 0 ) && ( C.x - D.x == 0 ) )
   {
        if ( A.x == C.x && ( ( A.y >= C.y && A.y <= D.y )  || ( B.y >= C.y && B.y <= D.y ) ) )
            return true;
   }
   // one vertical segment
   // segment AB non-rotated :: segment CD rotated
   else if ( (A.x - B.x == 0) && abs(C.x - D.x) > 0 )
   {
        // get equation for vertical line
        float x = A.x;

        // calclate slope of segment CD
        float m = (C.y - D.y) / (C.x - D.x);

        // calculate slope intercept of segment CD
        float b = C.y - ( m * C.x);

        // test vertical line in segment CD line-equation
        float y = m * x + b;

        // vertical line AB intercepts rotated CD
        if ( ( ( y >= C.y && y <= D.y ) || ( y >= D.y && y <= C.y) )  &&
             ( ( y >= A.y && y <= B.y ) || ( y >= B.y && y <= A.y) )
            )
        {
            return true;
        }   
   }   
   // one vertical segment
   // segment AB rotated :: segment CD non-rotated
   else if ( (C.x - D.x == 0) && abs(A.x - B.x) > 0 )
   {
        // get equation for vertical line
        float x = C.x;

        // calclate slope of segment CD
        float m = (A.y - B.y) / (A.x - B.x);

        // calculate slope intercept of segment CD
        float b = A.y - ( m * A.x);

        // test vertical line in segment CD line-equation
        float y = m * x + b;

        // vertical line AB intercepts rotated CD
        if ( ( ( y >= C.y && y <= D.y ) || ( y >= D.y && y <= C.y) )  &&
             ( ( y >= A.y && y <= B.y ) || ( y >= B.y && y <= A.y) )
            )
        {
            return true;
        }   
   }
    
    return false;  
}

class Rect2D {

public:

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
    }

    void draw_rect(SDL_Renderer* renderer)
    {
        // draw top edge
        SDL_RenderDrawLine(renderer,top_left_dst.x, top_left_dst.y, top_right_dst.x, top_right_dst.y);
        // draw right edge
        SDL_RenderDrawLine(renderer, top_right_dst.x, top_right_dst.y, bottom_right_dst.x, bottom_right_dst.y);
        // draw bottom edge
        SDL_RenderDrawLine(renderer, bottom_right_dst.x, bottom_right_dst.y,bottom_left_dst.x,bottom_left_dst.y);
        // draw left edge
        SDL_RenderDrawLine(renderer,bottom_left_dst.x, bottom_left_dst.y,top_left_dst.x, top_left_dst.y);

    }

    void rotate_rect(float theta)
    {
        top_left_dst = RotatingRect::rotate_vertex(top_left,theta,origin);
        top_right_dst = RotatingRect::rotate_vertex(top_right,theta,origin);
        bottom_left_dst = RotatingRect::rotate_vertex(bottom_left,theta,origin);
        bottom_right_dst = RotatingRect::rotate_vertex(bottom_right,theta,origin);
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

    float rotation_angle;
};

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

bool Rect2DIntersect(Rect2D rect_1, Rect2D rect_2)
{

    bool top_segment = Segment2DInterect(rect_1.getTopLeft(),rect_1.getTopRight(),rect_2);

    bool left_segment = Segment2DInterect(rect_1.getTopLeft(),rect_1.getBottomLeft(),rect_2);

    bool bottom_segment = Segment2DInterect(rect_1.getBottomLeft(),rect_1.getBottomRight(),rect_2);

    bool right_segment = Segment2DInterect(rect_1.getTopRight(),rect_1.getBottomRight(),rect_2);
    

    return top_segment || left_segment || bottom_segment || right_segment;
}


} // namespace RotatingRect

#endif