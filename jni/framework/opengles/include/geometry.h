#ifndef _ANTS360_VIDEO_GEOMETRY_H_
#define _ANTS360_VIDEO_GEOMETRY_H_
/*!
 * \brief   Simple geometry calculations related with video capture and
 *          video renderering.
 * \author  huangxuefeng
 * \date    2013-11-21
 */
namespace ants360 {

template <typename TYPE>
class Dimension
{
public:
    Dimension() { }
    Dimension(const Dimension & d) : width(d.width), height(d.height) { }
    Dimension(const TYPE & w, const TYPE & h) : width(w), height(h) { }

    Dimension transposeOf() const
    {
        return Dimension(height, width);
    }

    Dimension & operator =(const Dimension & d)
    {
        width = d.width;
        height = d.height;
        return *this;
    }

    TYPE width;
    TYPE height;
};

template <typename TYPE>
class Point
{
public:
    Point() { }
    Point(const TYPE & _x, const TYPE & _y) : x(_x), y(_y) { }
    Point & operator =(const Point & point)
    {
        x = point.x;
        y = point.y;
        return *this;
    }

    TYPE x;
    TYPE y;
};

template <typename TYPE>
class Quadrangle
{
public:
    Point<TYPE> p[4];
};

/*!
 * \brief   calculate the interest range of an overlapped virtual window
 *
 * \scenario When draws a frame into a view port with different dimension,
 *          regardless with all scales, it is reasonable to keep objects'
 *          aspect ratio unchanged.
 *
 * \note    The view port is placed over the frame,
 *          scale the frame that can fill the view port with no blank,
 *          make the largest use of view port to display contents of frame,
 *          keep the aspect ratio of contents in frame unchanged,
 *          align the view port and the frame center.
 *
 *          The output is the overlapped interest rectangle range of frame
 *          in canonical relative ratio, regardless of scales.
 *          The whole frame(w x h) is encoded as:
 *              p0(0, 0) - p1(1, 0) - p2(1, 1) - p3(0,1)
 *
 *          The half portrait frame(w x h) is encoded as:
 *              p0(0, 0.25) - p1(1, 0.25) - p2(1, 0.75) - p3(0, 0.75)
 *
 * \param [output] interest - interest range in relative ratio to crop frame
 *                            and fit into view port.
 *
 * \algorithm
 *
 * conditions:
 *          view port dimension: vw x vh
 *          frame dimension:     fw x fh
 *          aspect dimension:    aw x ah (scaled frame to fit view port)
 *          interest dimension:  iw x ih (cropped aspect dimension)
 *          interest range:      p0, p1, p2, p3 (relative positions)
 *
 * expectations:
 *          fw : fh = aw : ah;
 *          aw >= vw; ah >= vh;
 *          (aw = vw) or (ah = vh);
 *          iw = min(vw, aw); ih = min(vh, ah);
 *
 * conduction:
 *          let r = fw : fh = aw : ah;
 *          let t = vw : vh;
 *          if r = t:                    // ie. fw * vh = vw * fh
 *              aw := vw; ah := vh;
 *          else if r > t:               // ie. fw * vh > vw * fh
 *              ah := vh; aw := vh * r;
 *          else:
 *              aw := vw; ah := vw / r;
 *
 *          exception:
 *              aw := fw; ah := fh;
 *
 * \author  huangxuefeng
 * \date    2013-11-09
 */
template <typename INT, typename DECIMAL>
bool get_viewport_interest(
        const Dimension<INT> & view,
        const Dimension<INT> & frame,
        Quadrangle<DECIMAL> & interest)
{
    if (view.width <= 0 || view.height <= 0
            || frame.width <= 0 || frame.height <= 0)
    {
        return false;
    }

    INT fw_x_vh = frame.width * view.height;
    INT vw_x_fh = frame.height * view.width;
    if (fw_x_vh >= vw_x_fh)
    {
        // aspect.height = view.height;
        // aspect.width = fw_x_vh / frame.height;
        DECIMAL r = static_cast<DECIMAL>(vw_x_fh)
                  / static_cast<DECIMAL>(fw_x_vh);
        DECIMAL d1 = 0.5 * (1.0 - r);
        DECIMAL d2 = 0.5 * (1.0 + r);
        interest.p[0] = Point<DECIMAL>(d1, 0.0);
        interest.p[1] = Point<DECIMAL>(d2, 0.0);
        interest.p[2] = Point<DECIMAL>(d2, 1.0);
        interest.p[3] = Point<DECIMAL>(d1, 1.0);
    }
    else
    {
        // aspect.width = view.width;
        // aspect.height = vw_x_fh / frame.width;
        DECIMAL r = static_cast<DECIMAL>(fw_x_vh)
                  / static_cast<DECIMAL>(vw_x_fh);
        DECIMAL d1 = 0.5 * (1.0 - r);
        DECIMAL d2 = 0.5 * (1.0 + r);
        interest.p[0] = Point<DECIMAL>(0.0, d1);
        interest.p[1] = Point<DECIMAL>(1.0, d1);
        interest.p[2] = Point<DECIMAL>(1.0, d2);
        interest.p[3] = Point<DECIMAL>(0.0, d2);
    }
    return true;
}

// r x frame = view - d
// dw x fh - dh x fw = vw x fh - vh x fw; dw >= 0; dh >= 0; dw x dh = 0;
// if (vw x fh >= vh x fw): dh = 0; dw = vw - (vh x fw / fh)
// if (vw x fh <  vh x fw): dw = 0; dh = vh - (vw x fh / fw)
//
// | vw |  F:   | -1   1 |
// | vh | ----> | -1   1 |
//
// | dw |  F:   | -dw/vw   dw/vw |
// | dh | ----> | -dh/vh   dh/vh |
template <typename INT, typename DECIMAL>
bool get_viewport_interest2(
        const Dimension<INT> & view,
        const Dimension<INT> & frame,
        Quadrangle<DECIMAL> & interest)
{
    if (view.width <= 0 || view.height <= 0
            || frame.width <= 0 || frame.height <= 0)
    {
        return false;
    }

#if 0

    interest.p[0] = Point<DECIMAL>(-1.0f,  1.0f);
    interest.p[1] = Point<DECIMAL>( 1.0f,  1.0f);
    interest.p[2] = Point<DECIMAL>( 1.0f, -1.0f);
    interest.p[3] = Point<DECIMAL>(-1.0f, -1.0f);

#else

    INT fw_x_vh = frame.width * view.height;
    INT vw_x_fh = frame.height * view.width;
    if (fw_x_vh >= vw_x_fh)
    {
        // dw = 0; dh = vh - (vw x fh / fw)
        //  | vw |  F:   | -1   1 |
        //  | vh | ----> | -1   1 |
        //
        //  | dw |  F:   |  0   0  |
        //  | dh | ----> | r-1 1-r |
        //
        //  r =  (vw x fh) / (fw x vh)
        DECIMAL r = static_cast<DECIMAL>(vw_x_fh)
                  / static_cast<DECIMAL>(fw_x_vh);
        interest.p[0] = Point<DECIMAL>(-1.0f,  r);
        interest.p[1] = Point<DECIMAL>( 1.0f,  r);
        interest.p[2] = Point<DECIMAL>( 1.0f, -r);
        interest.p[3] = Point<DECIMAL>(-1.0f, -r);
    }
    else
    {
        // dh = 0; dw = vw - (vh x fw / fh)
        //  | vw |  F:   | -1   1 |
        //  | vh | ----> | -1   1 |
        //
        //  | dw |  F:   | r-1 1-r |
        //  | dh | ----> |  0   0  |
        //
        //  r =  (fw x vh) / (vw x fh)
        DECIMAL r = static_cast<DECIMAL>(fw_x_vh)
                  / static_cast<DECIMAL>(vw_x_fh);
        interest.p[0] = Point<DECIMAL>(-r,  1.0f);
        interest.p[1] = Point<DECIMAL>( r,  1.0f);
        interest.p[2] = Point<DECIMAL>( r, -1.0f);
        interest.p[3] = Point<DECIMAL>(-r, -1.0f);
    }

#endif

    return true;
}

}
#endif // _ANTS360_VIDEO_GEOMETRY_H_